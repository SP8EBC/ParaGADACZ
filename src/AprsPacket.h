#ifndef APRSPACKET_H
#define APRSPACKET_H

#include <exception>
#include <cstdint>
#include <string>
#include <vector>

#include <boost/date_time.hpp>

class NotValidAprsPacket: public std::exception {
    virtual const char* what() const throw() {
		return "\n--- Niepoprawny pakiet APRS\n";
    }
};

class PacketParsedOK: public std::exception {
    virtual const char* what() const throw() {
		return "\n--- Pakiet przetworzony OK\n";
    }
};

struct PathElement {
    std::string Call;
    uint8_t SSID;
};

class AprsPacket
{
    private:
    public:
        std::string DestAddr;   // Destination address, sliced from packer. In APRS it works as Device-ID
        std::string SrcAddr;    // Source address
        uint8_t SrcSSID;  // SSID of Source
        uint8_t DstSSID;
        std::vector<PathElement> Path;        // Routing Path
        std::string qOrigin;        // APRS-IS originator
        PathElement ToISOriginator;     // APRS-IS originator. It might be Igate callsign or APRS server name, if packet
                                        // was sent directly to Internet from some APRS client.
        const char * Data;         // Frame payload
        std::string DataAsStr;
        void PrintPacketData();     // Function witch print data from processed packet
        
        uint8_t ui;
        uint8_t protocol;

		/**
		 * @brief 	Used by aprx-rf log parser only. Packet timestamp in timezone used by APRX, but 
		 * 			this timezone is NOT stored in this structure
		 * 
		*/
        boost::posix_time::ptime packetLocalTimestmp;

		/**
		 * @brief 	This is UTC timestamp of the frame, calculated by a call to @link{TimeTools::getEpochFromPtime} made 
		 * 			from @link{AprxLogParser::getLastPacketForStation}
		 * 
		 * @attention 	There is an assumption made here that APRS2RRD is run on an account with the same timezone as 
		 * 				what APRX uses. There is no information about TZ stored in aprx-rf.log file, it is just local
		 * 				time. Whatever the local time means.
		*/
		uint64_t packetUtcTimestamp;

		static int ParseAPRSISData(const char* tInputBuffer, int buff_len, AprsPacket* cTarget);
        static int ParseAprxRfLogData(const char* tInputBuffer, int buff_len, AprsPacket* cTarget);
		static bool SeparateCallSsid(const std::string& input, std::string& call, uint8_t& ssid, bool exception);
		static bool SeparateCallSsid(const std::string& input, char (&call)[7], uint8_t& ssid, bool exception);

		void clear();

		AprsPacket();          // Class constructor. It prepares all variables by writing zeros to it
		~AprsPacket();
};

#endif // APRSPACKET_H
