#include "AprsPacket.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

#include "ReturnValues.h"

using namespace std;

AprsPacket::AprsPacket() {
    this->SrcSSID = 0;
    this->DstSSID = 0;
    this->Data = 0;
    //memset(this->Data,0x00,sizeof(this->Data));

    this->ToISOriginator.Call = "UNINITIALIZED";
    this->protocol = 0;
    this->ui = 0;
}

bool AprsPacket::SeparateCallSsid(const std::string& input, std::string& call,
		uint8_t& ssid, bool exception, const uint8_t defaultSsid) {

	// copying the input string to editable local object
	std::string in = input;

	bool out = false;
	bool hasSsid = (in.find("-") != std::string::npos) ? true : false;

	// removing any '*' from input string - we don't need to preserve H-bits from packet path
	boost::erase_all(in, "*");

	if (hasSsid) {
		std::vector<std::string> callAndSsid;

		boost::split(callAndSsid, in, boost::is_any_of("-"));

		// check if Ssid has a valid leght, not greater than 6 characters
		if (callAndSsid.at(0).size() > 6)
			return false;

		call = callAndSsid.at(0);

		try {
			ssid = boost::lexical_cast<int>(callAndSsid.at(1));
		}
		catch (std::bad_cast &e) {
			if (exception)
				// if an user want to have an exception on bad_cast
				throw e;
			else
				// if an user rather prefer to receive only false return value
				ssid = defaultSsid;
		}
	}
	else {
		call = in;
		ssid = 0;
	}

	// validating SSID value
	if (ssid > 15)
		out = false;

	// validating lenght of string
	if (input.size() > 6)
		out = false;

	out = true;
	return out;
}

bool AprsPacket::SeparateCallSsid(const std::string& input, char (&call)[7],
		uint8_t& ssid, bool exception) {

	std::string intermediate;

	bool separatingReturn;

	separatingReturn = AprsPacket::SeparateCallSsid(input, intermediate, ssid, exception);

	if (!separatingReturn)
		return false;

	if (intermediate.size() > 6)
		return false;

	std::copy(intermediate.begin(), intermediate.end(), call);

	return true;
}

AprsPacket::~AprsPacket()
{
}

void AprsPacket::PrintPacketData() {
	int i = 0;
    cout << "---------------- PACKET DATA ----------------" << endl;
    cout << "-- Callsign: " << this->SrcAddr << "-" << (int)this->SrcSSID << endl;
    cout << "-- Device identifier: " << this->DestAddr << endl;
    cout << "-- Packet path length: " << this->Path.size() << endl;
    for (PathElement elem: this->Path) {
        cout << "------ Element number: " << ++i << " = " << elem.Call << "-" << (int)elem.SSID << endl;
    }
    cout << "-- APRS originator type: " << this->qOrigin << endl;
    cout << "-- APRS originator callsign: " << this->ToISOriginator.Call << endl;
    cout << "-- Frame payload: " <<  this->DataAsStr << endl;
    cout << "---------------------------------------------- \r\n";
}


int AprsPacket::ParseAPRSISData(const char* tInputBuffer, int buff_len, AprsPacket* cTarget) {

	// simple regex to match most of callsign systems
	boost::regex callsignPattern("^[A-Z0-9]{3}[A-Z]{1,3}", boost::regex::icase);

	// q-construct regex
	boost::regex qc("q[A-Z]{2}");

	// frame payload
	std::string payload = "";

	// a flag set to true when q-construct is detected in path, to signalize
	// that the next element will consist originator callsign
	bool parseOrginator = false;

	// a value returned from call separating method
	bool separatingReturn = true;

	// checkig if input buffer is valid
	if (tInputBuffer == nullptr)
		return NOT_VALID_APRS_PACKET;

	// check if input buffer is valid
    if (*tInputBuffer == '#' || *tInputBuffer == 0x00 || buff_len > 1000 || buff_len < 5)
		return NOT_VALID_APRS_PACKET;

    // converting to string
	std::string input(tInputBuffer, buff_len);

	// vector which will hold source call separated from the rest of frame
	std::vector<std::string> sepratedBySource;

	// vector to keep path elements separated from data (frame payload)
	std::vector<std::string> pathAndData;

	// vector to keep separated path elements
	std::vector<std::string> pathElements;

	// spltting input frame basing on '>' character
	boost::split(sepratedBySource, input, boost::is_any_of(">"));

	// there must be at least one '>' character which will separate soruce from
	// the rest of frame
	if (sepratedBySource.size() < 2)
		return NOT_VALID_APRS_PACKET;

	// separating a callsign from the SSID
	separatingReturn = AprsPacket::SeparateCallSsid(sepratedBySource.at(0), cTarget->SrcAddr, cTarget->SrcSSID, false);

	if (!separatingReturn)
		return CALL_AND_SSID_SEPARATION_FAILED;

	// checking if the callsign match with regex
	cTarget->IsSrcAddrValidationOk = boost::regex_match(cTarget->SrcAddr, callsignPattern);

	// the 'sepratedBySource' vector may consist more than 2 elements if received frame consist
	// status message which is identified by '>' character before it. All in all the APRS2RRD
	// is focused to work on wx frames, so we can just ignore everything after second element.
	std::string path = sepratedBySource.at(1);

	// split path and data, assuming that data doesn't contain any ':'
	boost::split(pathAndData, path, boost::is_any_of(":"));

	// splitting path elements and the frame payload
	boost::split(pathElements, pathAndData.at(0), boost::is_any_of(","));

	// checking if after splitting there are enough elements to thread this as valid frame
	if (pathElements.size() == 0)
		return APRS_IS_PATH_EMPTY;

	// checking if source identifier is valid
	if (pathElements.at(0).size() > 6)
		return DESTINATION_CALL_INVALID;

	AprsPacket::SeparateCallSsid(pathElements.at(0), cTarget->DestAddr, cTarget->DstSSID, false);

	// removing the first element which which consist target address
	pathElements.erase(pathElements.begin());

	// splitting
	for (size_t i = 0; i < pathElements.size(); i++) {

		std::string e = pathElements.at(i);

		try {
			// object to store path element
			PathElement pelem;

			// separating callsign from the ssid or WIDEx from 'remaining hops' value
			AprsPacket::SeparateCallSsid(e, pelem.Call, pelem.SSID, false, 99);

			// storing element in Path vector
			cTarget->Path.push_back(pelem);
		}
		catch (const boost::bad_lexical_cast& ex) {
			return NOT_VALID_APRS_PACKET;
		}
		catch (const std::bad_cast& ex) {
			return NOT_VALID_APRS_PACKET;
		}

	}

	// copying the payload to the output object. There is no need to check the lenght of
	// payload due to 'buff_len > 1000' check done just at the begining of this method
	cTarget->DataAsStr = std::move(pathAndData.at(1));

	//cTarget->DataAsStr = std::string(cTarget->Data);

	cTarget->Data = cTarget->DataAsStr.c_str();

	boost::trim(cTarget->DataAsStr);

	return OK;
}

void AprsPacket::clear() {

    this->DestAddr = "";
    this->DstSSID = 0;

    this->SrcAddr = "";
    this->SrcSSID = 0;

    this->Path.clear();
    this->qOrigin = "";

    this->Data = 0;
    //memset(this->Data,0x00,sizeof(this->Data));

    this->ToISOriginator.Call = "UNINITIALIZED";

}
