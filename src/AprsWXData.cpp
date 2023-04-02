#include "AprsWXData.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <regex>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>


#include "main.h"

AprsWXData::AprsWXData() {
	ssid = 0;
	call = "";

    wind_speed = 0.0;
    wind_gusts = 0.0;
    wind_direction = 0;
    humidity = 0;
    temperature = 0;
    pressure = 0;
    rain60 = 0;
    rain24 = 0;
    rain_day = 0;
    valid = false;

	convertPressure = false;

    useHumidity = false;
    usePressure = false;
    useTemperature = false;
    useWind = false;

    is_secondary = false;
    is_primary = false;

    //DebugOutput = false;
}

AprsWXData::~AprsWXData() {
}

int AprsWXData::ParseData(AprsPacket& input, AprsWXData* output) {

    int i = 0;
    int conv_temp;
    //char *src;
    std::vector<std::string> extractedWx;	// this vector will be used as an output from boost::split method

    output->valid = false;

    if (*(input.Data) != '!' && *(input.Data) != '@') {
        output->valid = false;
        return -1;     // to nie sa dane pogodowe
    }
    //src = input.Data;

    // converting char array to copy of string class for convinence
    std::string source(input.Data);

    // the 'split' method doesn't copy a character which was used as split point!
    boost::split(extractedWx, source, boost::is_any_of("_"));

    // if the extracted vector has less than 2 elements it means that this is not
    // valid frame
    if (extractedWx.size() < 2)
    	return -1;

    // Underscore '_' divides an APRS wx frame to part with position data (first one) and
    // the second one which holds measurement data
    std::string wxData = extractedWx.at(1);

    output->wind_speed = 0.0;
    output->wind_gusts = 0.0;
    output->wind_direction = 0;
    output->humidity = 0;
    output->temperature = 0;
    output->pressure = 0;
    output->rain60 = 0;
    output->rain24 = 0;
    output->rain_day = 0;
    output->valid = false;

    //i++;    // przeskoczenie na pierwszy znak danych meteo
    if (AprsWXData::CopyConvert('/',wxData,conv_temp,i) == 0)   // kierunek    this->wind_direction = conv_temp;
    	output->wind_direction = conv_temp;
	else
		return -1;
    i++;
    if (AprsWXData::CopyConvert('g',wxData,conv_temp,i) == 0)   // siła
    	output->wind_speed = (float)conv_temp * 0.44;
	else
		return -1;
    i++;
    if (AprsWXData::CopyConvert('t',wxData,conv_temp,i) == 0)       // porywy
    	output->wind_gusts = (float)conv_temp * 0.44;
	else
		return -1;
    i++;
    if (AprsWXData::CopyConvert('r',wxData,conv_temp,i) == 0)   // temperatura
    	output->temperature = ((float)conv_temp - 32) / 9 * 5;
	else
		return -1;
    i++;
    if (AprsWXData::CopyConvert('p',wxData,conv_temp,i) == 0)   // deszcz przez ostania godzine
    	output->rain60 = conv_temp;
	else
		return -1;
	i++;
    if (AprsWXData::CopyConvert('P',wxData,conv_temp,i) == 0)   // deszcz przez ostania godzine
    	output->rain24 = conv_temp;
	else
		return -1;
	i++;
    if (AprsWXData::CopyConvert('b',wxData,conv_temp,i) == 0)   // deszcz przez ostania godzine
    	output->rain_day = conv_temp;
	else
		return -1;
	i++;
    if (AprsWXData::CopyConvert((unsigned)5,wxData,conv_temp,i) == 0)   // ciśnienie
    	output->pressure = conv_temp / 10;
	else;
	i++;
    if (AprsWXData::CopyConvert((unsigned)3,wxData,conv_temp,i) == 0)
    	output->humidity = conv_temp;
	else;
    output->valid = true;

    output->ssid = input.SrcSSID;
    output->call = input.SrcAddr;

    return 0;
}

/**
 * This method shall read an input string, starting from position given by value of 'counter'
 * parameter and ends at first presence of 'sign' character. Read numerical value shall be
 * lexical casted to int and stored at 'output' value
 */
int AprsWXData::CopyConvert(char sign, std::string& input, int& output, int& counter) {
    size_t position;

    // finding a position of character 'sign' in input string starting from given position 'counter'
    position = input.find_first_of(sign, counter);

    // checking if character 'sign' is present in an input string or not
    if (position == std::string::npos)
    	return -1;

    // creating aux substring with value for converrsion
    std::string valueToConv = input.substr(counter, position - counter);

    try {
    	// converting value
    	output = boost::lexical_cast<int>(valueToConv);
    }
    catch (const boost::bad_lexical_cast& ex) {
    	std::cout << ex.what() << std::endl;
    	output = 0;
    	//return -1;
    }
    catch (const std::bad_cast& ex) {
    	std::cout << ex.what() << std::endl;
    	output = 0;
    	//return -1;
    }

    // storing a position where 'sign' was found
    counter = (int)position;

	return 0;
}

/**
 * This method shall read an input string, starting from position given by value of 'counter'
 * parameter and ends at position 'counter' + 'num'. Read numerical value shall be
 * lexical casted to int and stored at 'output' value
 */
int AprsWXData::CopyConvert(unsigned num, std::string& input, int& output, int& counter) {
    char tempbuff[9];
    memset(tempbuff, 0x00, sizeof(tempbuff));

    std::size_t input_size = input.length();

    // if the input string counter exceed its size
    if ((unsigned)counter > input_size)
    	return -1;

    // if caler want to read more characters than input string stores
    if (input_size < num + counter) {
    	num = input.length() - counter;
    }

    // creating aux substring with value for converrsion
    std::string valueToConv = input.substr(counter, num);

    // removing any non digits
    valueToConv = std::regex_replace(valueToConv, std::regex(R"([\D])"), "");

    try {
    	// converting value
    	output = boost::lexical_cast<int>(valueToConv);
    }
    catch (const boost::bad_lexical_cast& ex) {
    	std::cout << ex.what() << std::endl;
    	output = 0;
    }
    catch (const std::bad_cast& ex) {
    	std::cout << ex.what() << std::endl;
    	output = 0;
    }

    // storing a position where 'sign' was found
    counter = (int)num + counter;

	return 0;
}

AprsWXData::AprsWXData(const AprsWXData& in) {

	this->humidity = in.humidity;
	this->pressure = in.pressure;
	this->rain24 = in.rain24;
	this->rain60 = in.rain60;
	this->rain_day = in.rain_day;
	this->temperature = in.temperature;
	this->wind_direction = in.wind_direction;
	this->wind_gusts = in.wind_gusts;
	this->wind_speed = in.wind_speed;

	this->valid = in.valid;

	this->useHumidity = in.useHumidity;
	this->usePressure = in.usePressure;
	this->useWind = in.useWind;
	this->useTemperature = in.useTemperature;

	this->ssid = in.ssid;
	this->call = in.call;

	this->is_primary = in.is_primary;
	this->is_secondary = in.is_secondary;

	this->convertPressure = in.convertPressure;

	this->packetAgeInSecondsLocal = in.packetAgeInSecondsLocal;
	this->packetAgeInSecondsUtc = in.packetAgeInSecondsUtc;

	this->packetTimestmp = in.packetTimestmp;
}

AprsWXData::AprsWXData(AprsWXData &&in) {
	this->humidity = in.humidity;
	this->pressure = in.pressure;
	this->rain24 = in.rain24;
	this->rain60 = in.rain60;
	this->rain_day = in.rain_day;
	this->temperature = in.temperature;
	this->wind_direction = in.wind_direction;
	this->wind_gusts = in.wind_gusts;
	this->wind_speed = in.wind_speed;

	this->valid = in.valid;

	this->useHumidity = in.useHumidity;
	this->usePressure = in.usePressure;
	this->useWind = in.useWind;
	this->useTemperature = in.useTemperature;

	this->ssid = in.ssid;
	this->call = in.call;

	this->is_primary = in.is_primary;
	this->is_secondary = in.is_secondary;

	this->convertPressure = in.convertPressure;

	this->packetAgeInSecondsLocal = in.packetAgeInSecondsLocal;
	this->packetAgeInSecondsUtc = in.packetAgeInSecondsUtc;

	this->packetTimestmp = in.packetTimestmp;
}

AprsWXData& AprsWXData::operator =(AprsWXData& _in) {

	this->humidity = _in.humidity;
	this->pressure = _in.pressure;
	this->rain24 = _in.rain24;
	this->rain60 = _in.rain60;
	this->rain_day = _in.rain_day;
	this->temperature = _in.temperature;
	this->wind_direction = _in.wind_direction;
	this->wind_gusts = _in.wind_gusts;
	this->wind_speed = _in.wind_speed;

	this->valid = _in.valid;

	this->useHumidity = _in.useHumidity;
	this->usePressure = _in.usePressure;
	this->useWind = _in.useWind;
	this->useTemperature = _in.useTemperature;

	this->is_primary = _in.is_primary;
	this->is_secondary = _in.is_secondary;

	this->call = _in.call;
	this->ssid = _in.ssid;

	return * this;
}

AprsWXData& AprsWXData::operator -(AprsWXData& _in) {
	int diff1 = 0, diff2;
	float sin1 = 0.0f, sin2 = 0.0f;

	// scalar values are subtracted directly
	if (_in.humidity && this->humidity) {

		this->humidity = ::abs(this->humidity - _in.humidity);
	}

	if (_in.useTemperature && this->useTemperature) {

		this->temperature -= _in.temperature;
	}

	if (_in.usePressure && this->usePressure) {

		this->pressure = ::abs(this->pressure - _in.pressure);
	}


	if (_in.useWind && this->useWind) {
		// wind direction is a vector value so there are always two
		// differences between them - calculated clockwise and counterclockwise
		diff1 = this->wind_direction - _in.wind_direction;
		diff2 = -this->wind_direction + _in.wind_direction;

		// to retain proper sign of the subtraction result the cosinus needs to be calculated
		sin1 = ::cos(this->wind_direction);
		sin2 = ::cos(_in.wind_direction);

		// adjust distance between direction to 0-359 degs scale, not -180 to 180
		if (diff1 < 0) {
			diff1 += 360;
		}
		if (diff2 < 0) {
			diff2 += 360;
		}

		// always use the smaller result as a distance between two wind direction
		if (diff1 < diff2)
			this->wind_direction = diff1;
		else {
			this->wind_direction = diff2;
		}

		// because wind is alywas calculated clockwise (90 - E; 180 - S; 270 - W) adjust
		// the sign of the difference (distance) between direction. In such case direction
		// of 5 degrees is 15 degrees bigger (more towards east) that 350. Similarly 340 is
		// 30 degrees BEFORE ( -30 ) 10 degrees
		if (sin1 > sin2)
			this->wind_direction *= -1;

		// wind speed is scalar value
		this->wind_speed -= _in.wind_speed;
		this->wind_gusts -= _in.wind_gusts;

	}

	return * this;
}

void AprsWXData::copy(AprsWXData& source, bool withoutTemperature, bool onlyTemperature) {

	if (!source.valid) {
		return;
	}

	this->valid = true;

	if (onlyTemperature) {
		this->useTemperature = true;
		this->usePressure = false;
		this->useWind = false;
		this->useHumidity = false;

		this->temperature = source.temperature;

		return;
	}

	if (withoutTemperature) {
		this->useTemperature = false;
		this->usePressure = true;
		this->useWind = true;
		this->useHumidity = true;
	}
	else {
		this->useTemperature = true;
		this->usePressure = true;
		this->useWind = true;
		this->useHumidity = true;

		this->temperature = source.temperature;
	}

	this->humidity = source.humidity;
	this->pressure = source.pressure;
	this->rain24 = source.rain24;
	this->rain60 = source.rain60;
	this->rain_day = source.rain_day;
	this->wind_direction = source.wind_direction;
	this->wind_gusts = source.wind_gusts;
	this->wind_speed = source.wind_speed;

	return;

}

void AprsWXData::copy(float temperature, bool onlyTemperature) {
	if (onlyTemperature) {
		this->useTemperature = true;
		this->usePressure = false;
		this->useWind = false;
		this->useHumidity = false;
	}

	this->useTemperature = true;
	this->temperature = temperature;

}



short AprsWXData::DirectionCorrection(AprsWXData& packet, short direction, short correction) {
    short out;

	if (!packet.valid)
		return -255;	// in case of error return invalid wind direction

	if (correction == 0)
		return -255;

    if (direction + correction > 360)
    {
        out = (direction + correction) - 360;
    }
    else if (direction + correction < 0)
    {
        out = (direction + correction) + 360;
    }
    else
        out = direction + correction;

    return out;
}

void AprsWXData::DirectionCorrection(AprsWXData& packet, short correction) {
	if (!packet.valid)
		return;

    int direction = packet.wind_direction;

    int out;

	if (correction == 0)
		return;

    if (direction + correction > 360)
    {
        out = (direction + correction) - 360;
    }
    else if (direction + correction < 0)
    {
        out = (direction + correction) + 360;
    }
    else
        out = direction + correction;

    packet.wind_direction = out;
}

void AprsWXData::NarrowPrecisionOfWindspeed() {
	uint32_t temp = 0;

	temp = (uint32_t)(this->wind_speed * 10.0f);
	this->wind_speed = (float)temp / 10.0f;

	temp = (uint32_t)(this->wind_gusts * 10.0f);
	this->wind_gusts = (float)temp / 10.0f;
}

void AprsWXData::NarrowPrecisionOfTemperature() {
	int32_t temp = 0;

	temp = (int32_t)(this->temperature * 10.0f);
	this->temperature = (float)temp / 10.0f;
}
