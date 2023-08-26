#ifndef APRSWXDATA_H
#define APRSWXDATA_H

#include "AprsPacket.h"
#include <exception>
#include <queue>
#include <string>
#include "exception/NotValidWXDataEx.h"

#include <boost/date_time.hpp>

#define APRSWXDATA_MISSING_PARAMETER	-1

enum class WXDataSource {
	APRSIS,
	SERIAL,
	HOLFUY,
	ZYWIEC,
	UNKNOWN
};

class AprsWXData
{
    public:
		std::string call;
		unsigned ssid;

		// these flags were added because they helps in DiffCalculator distinguish between pri and sec
		// 'copy' methods doen't rely on them as they were developed before
		bool is_primary;
		bool is_secondary;

        float wind_speed;
        float wind_gusts;
        int wind_direction;
        float temperature;
        short pressure;
        short rain60;
        short rain24;
        short rain_day;
        unsigned short humidity;

        bool useTemperature;	// ustawianie na true jeżeli program ma użyć zapisanej wartości temperatury
        bool useWind;
        bool usePressure;
        bool useHumidity;

        boost::posix_time::ptime packetTimestmp;

        int packetAgeInSecondsUtc;
        int packetAgeInSecondsLocal;

	bool convertPressure;
    public:
        bool valid;

        // default constructor
        AprsWXData();

        // copy constructor
        AprsWXData(const AprsWXData& in);

        // move constructor
        AprsWXData(AprsWXData&& in);

        // assigment operator
        AprsWXData& operator= (AprsWXData);

        // assigment operator
        AprsWXData& operator= (AprsWXData&);

        // subtraction operator
        AprsWXData& operator-(AprsWXData&);

        // add
        AprsWXData& operator+(AprsWXData&);

        ~AprsWXData();

		void NarrowPrecisionOfWindspeed();
		void NarrowPrecisionOfTemperature();

		void copy(AprsWXData & source, bool withoutTemperature, bool onlyTemperature);
		void copy(float temperature, bool onlyTemperature);

		static int ParseData(AprsPacket& input, AprsWXData* output);
        static int CopyConvert(char sign, std::string& input, int& output, int& counter);
		static int CopyConvert(unsigned num, std::string& input, int& output, int& counter);
		static short DirectionCorrection(AprsWXData& packet, short direction, short correction);
		static void DirectionCorrection(AprsWXData& packet, short correction);

};

#endif // APRSWXDATA_H
