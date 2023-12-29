/*
 * AprxLogParser_test.cpp
 *
 *  Created on: Dec 3, 2022
 *      Author: mateusz
 */

#include "AprxLogParser.h"
#include "AprxLogParserStaticStuff.h"

#define BOOST_TEST_MODULE AprxLogParser_test
#include <boost/test/included/unit_test.hpp>
#include <fstream>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

struct MyConfig
{
  MyConfig() : test_log( "./test_reports/aprxlogparser_test.log" )
  {
    boost::unit_test::unit_test_log.set_stream( test_log );
    boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_level::log_successful_tests);

	spdlog::set_level(spdlog::level::debug);

  }
  ~MyConfig()
  {
    boost::unit_test::unit_test_log.set_stream( std::cout );
  }

  std::ofstream test_log;
};

BOOST_GLOBAL_FIXTURE (MyConfig);

BOOST_AUTO_TEST_CASE (between_two_epoch_timestamps)
{
	/**
	 * 2023-08-13 17:30:04.032 SR9NSK-4  d *OK2ILA-6>APDWX0,TCPIP,OK8UOF*:!4954.81N/01754.63E_.../...g...t080b10182h61; 27,0C OpenWRT+WX Opava
2023-08-13 17:30:04.837 SR9NSK-4  d *OK0DSK-6>APDWX0,TCPIP,OK8UOF*:!4937.97N/01815.87E_.../...g...t076; 24,6C WX Kubankov, ASL 660m
2023-08-13 17:30:05.632 SR9NSK-4  d *OK2ULQ-1>APMI06,TCPIP,OK8UOF*:@131729z4926.19N/01801.17E-WX3in1Plus2.0 U=13.5V,T=23.2C/73.7F
2023-08-13 17:30:19.254 SR9NSK-4  R SP1NKF>UP0U38,SR9NSK-1*,WIDE1*,WIDE2-1:`.ChqSk>/`"6[}_%
2023-08-13 17:30:30.769 SR9NSK-4  R SR9NWC>APMI03:=4939.19NL01959.53E#PHG2620 W3,SPn 80km Lubon W. alt=1040 Jerzy SP9FBT
2023-08-13 17:30:33.213 APRSIS    R SR9WXM>AKLPRZ,TCPIP*,qAC,T2CSNGRAD:!4946.58N/01907.84E_269/006g010t071r...p...P...b00000h00
2023-08-13 17:30:33.818 SR9NSK-4  R SR9WXM>AKLPRZ:!4946.58N/01907.84E_269/006g010t071r...p...P...b00000h00
2023-08-13 17:30:35.218 SR9NSK-4  R SR9WXM>AKLPRZ:!4946.58N/01907.84E_269/006g010t071r...p...P...b00000h00
2023-08-13 17:30:40.674 SR9NSK-4  R SQ9CWI-9>UP4P38,SR9DZB*,SR9KFZ*,WIDE2*:`.Cnl!Y>/>"6s}=
2023-08-13 17:30:48.821 APRSIS    R SQ9MPK>APRX29,TCPIP*,qAC,T2POLAND:T#597,4.0,0.5,16.0,0.0,3.0,00000000
2023-08-13 17:30:54.592 SR9NSK-4  R SR9NDJ>APRX28:!5033.45ND01927.02E#PHG3280 Digi & IGate Jurajski
2023-08-13 17:30:56.449 SR9NSK-4  R SR9WXG>AKLPRZ:!4935.92N/02059.77E_079/009g013t032r...p...P...b09516h00
2023-08-13 17:30:59.083 SR9NSK-4  R SQ3SHC-9>UPRY01,SR9NSK-1*,WIDE1*,WIDE2-1:`.ZZ"Tf>/]"6&}=
	 *
	 *
	 */

	AprxLogParser parser("./test_input/aprx-rf.log", true);

	parser.openFile();

	const std::vector<AprsWXData> result = parser.getAllWeatherPacketsInTimerange(1691940600, 1691940660);
	//																								^
	//																		Sunday, August 13, 2023 5:31:00 PM GMT+02:00
	//																		  			^
	//															Sunday, August 13, 2023 5:30:00 PM GMT+02:00

	BOOST_CHECK(result.size() == 6);
}

BOOST_AUTO_TEST_CASE (with_ssid_aprsrflog2)
{
	const std::string expected = "2022-12-03 14:05:29.678 SR9NSK    R SQ5BUJ-7>APAT81,SR7NST*,WIDE1*,WIDE2-1:!5042.54N/02024.29E>198/066/A=000902145.500MHz TG2605 FMLINK JANEK";

	AprxLogParser parser("./test_input/aprs-rf-2.log", true);

	parser.openFile();

	std::optional<std::string> result = parser.getNextLine("SQ5BUJ", 7, false);

	BOOST_CHECK(result);
	BOOST_CHECK_EQUAL(result.value(), expected);

}

BOOST_AUTO_TEST_CASE (find_double_wx_frames)
{
	const std::string first_expected =
			"2022-12-03 14:05:11.117 SR9NSK    R SR6WXP>AKLPRZ,WIDE2-1:!5038.00N/01747.00E_070/004g007t032r...p...P...b09995h36";

	const std::string second_expected =
			"2022-12-03 14:05:12.561 SR9NSK    R SR6WXP>AKLPRZ,SR9LC-1*,WIDE2*:!5038.00N/01747.00E_070/004g007t032r...p...P...b09995h36";

	AprxLogParser parser("./test_input/aprs-rf-2.log", true);

	parser.openFile();

	std::optional<std::string> result = parser.getNextLine("SR6WXP", 0, true);
	BOOST_CHECK(result);
	BOOST_CHECK_EQUAL(result.value(), first_expected);

	std::optional<std::string> result2 = parser.getNextLine("SR6WXP", 0, true);
	BOOST_CHECK(result2);
	BOOST_CHECK_EQUAL(result2.value(), second_expected);
}

//2022-12-03 14:06:07.521 SR9NSK    R SR9NCI>AKLPRZ,WIDE2-1::SR9NCI   :EQNS.0,1,0,0,1,0,0,1,0,0,1,0,0,0.25,-25
BOOST_AUTO_TEST_CASE (cieszyn_find_first_no_wx_only_aprsrflog2)
{
	const std::string expected = "2022-12-03 14:06:07.521 SR9NSK    R SR9NCI>AKLPRZ,WIDE2-1::SR9NCI   :EQNS.0,1,0,0,1,0,0,1,0,0,1,0,0,0.25,-25";

	AprxLogParser parser("./test_input/aprs-rf-2.log", true);

	parser.openFile();

	std::optional<std::string> result = parser.getNextLine("SR9NCI", 0, false);

	BOOST_CHECK(result);
	BOOST_CHECK_EQUAL(result.value(), expected);
}

BOOST_AUTO_TEST_CASE (cieszyn_find_first_wx_only_aprsrflog2)
{
	const std::string expected = "2022-12-03 14:09:11.801 SR9NSK    R SR9NCI>AKLPRZ,WIDE2-1:!4944.22N/01838.20E_013/000g002t037r...p...P...b00000";

	AprxLogParser parser("./test_input/aprs-rf-2.log", true);

	parser.openFile();

	std::optional<std::string> result = parser.getNextLine("SR9NCI", 0, true);

	BOOST_CHECK(result);
	BOOST_CHECK_EQUAL(result.value(), expected);
}

BOOST_AUTO_TEST_CASE (cieszyn_convert_to_wxdata_aprsrflog2)
{
	const std::string expected = "2022-12-03 14:29:22.996 SR9NSK    R SR9NCI>AKLPRZ,SR9GM-2*,WIDE2*:!4944.22N/01838.20E_045/005g007t036r...p...P...b00000";

	BOOST_TEST_MESSAGE("2022-12-03 14:29:22.996 SR9NSK    R SR9NCI>AKLPRZ,SR9GM-2*,WIDE2*:!4944.22N/01838.20E_045/005g007t036r...p...P...b00000");


	AprxLogParser parser("./test_input/aprs-rf-2.log", true);

	parser.openFile();
	parser.rewindFile();

	const std::optional<AprsWXData> result = parser.getLastPacketForStation("SR9NCI", 0);

	BOOST_CHECK(result.has_value());
	BOOST_CHECK(result->valid);
	BOOST_CHECK_EQUAL(result->call, "SR9NCI");
	BOOST_CHECK(result->wind_direction == 45);

	boost::posix_time::ptime ptime = result->packetLocalTimestmp;
	uint64_t timestamp = result->packetUtcTimestamp;

	BOOST_CHECK_EQUAL(ptime.date().year(), 2022);
	BOOST_CHECK_EQUAL(ptime.date().month(), 12);
	BOOST_CHECK_EQUAL(ptime.date().day(), 3);

	BOOST_CHECK_EQUAL(ptime.time_of_day().hours(), 14);
	BOOST_CHECK_EQUAL(ptime.time_of_day().minutes(), 29);

	BOOST_CHECK_EQUAL(timestamp, 1670074162ULL);
}

// 2023-08-13 17:49:29.598 SR9NSK-4  R SR9WXZ>AKLPRZ:!4943.43N/01912.10E_296/002g003t065r...p...P...b00000h00
BOOST_AUTO_TEST_CASE (jezioro_zywieckie_convert_to_wxdata_aprxrf)
{
	const std::string expected = "2023-08-13 17:49:29.598 SR9NSK-4  R SR9WXZ>AKLPRZ:!4943.43N/01912.10E_296/002g003t065r...p...P...b00000h00";

	BOOST_TEST_MESSAGE(expected);


	AprxLogParser parser("./test_input/aprx-rf.log", true);

	parser.openFile();
	parser.rewindFile();

	const std::optional<AprsWXData> result = parser.getLastPacketForStation("SR9WXZ", 0);

	BOOST_CHECK(result.has_value());
	BOOST_CHECK(result->valid);
	BOOST_CHECK_EQUAL(result->call, "SR9WXZ");
	BOOST_CHECK(result->wind_direction == 296);

	boost::posix_time::ptime ptime = result->packetLocalTimestmp;
	uint64_t timestamp = result->packetUtcTimestamp;

	BOOST_CHECK_EQUAL(ptime.date().year(), 2023);
	BOOST_CHECK_EQUAL(ptime.date().month(), 8);
	BOOST_CHECK_EQUAL(ptime.date().day(), 13);

	BOOST_CHECK_EQUAL(ptime.time_of_day().hours(), 17);
	BOOST_CHECK_EQUAL(ptime.time_of_day().minutes(), 49);

	// huh it depends if this test is run in summer or winter time :/ Yeap, quite a shame
	// but this is how it works with rf-log file not having info about timezone
	BOOST_CHECK_EQUAL(true, (timestamp == 1691941769ULL) || (timestamp == 1691945369));
}

// 2023-08-13 17:43:45.221 SR9NSK-4  R SR9WXL>AKLPRZ,SR9NWC*,WIDE2*:!4947.70N/01926.80E_263/001g003t069r...p...P...b09217h75
BOOST_AUTO_TEST_CASE (leskowiec_convert_to_wxdata_aprxrf)
{
	const std::string expected = "2023-08-13 17:51:48.846 SR9NSK-4  R SR9WXL>AKLPRZ,SP9UVG-13*,WIDE2*:!4947.70N/01926.80E_270/001g004t068r...p...P...b09218h75";

	BOOST_TEST_MESSAGE(expected);

	AprxLogParser parser("./test_input/aprx-rf.log", true);

	parser.openFile();
	parser.rewindFile();

	const std::optional<AprsWXData> result = parser.getLastPacketForStation("SR9WXL", 0);

	BOOST_CHECK(result.has_value());
	BOOST_CHECK(result->valid);
	BOOST_CHECK_EQUAL(result->call, "SR9WXL");
	BOOST_CHECK(result->wind_direction == 270);
}

// 2023-08-13 17:52:57.419 SR9NSK-4  R SP9UVG-13>APMI06,WIDE2-2:@131753z5010.48N/01913.46E_225/000g001t072r000p000P...h68b10306 WX Myslowice
BOOST_AUTO_TEST_CASE (myslowice_convert_to_wxdata_aprxrf)
{
	const std::string expected = "2023-08-13 17:52:57.419 SR9NSK-4  R SP9UVG-13>APMI06,WIDE2-2:@131753z5010.48N/01913.46E_225/000g001t072r000p000P...h68b10306 WX Myslowice";

	BOOST_TEST_MESSAGE(expected);

	AprxLogParser parser("./test_input/aprx-rf.log", true);

	parser.openFile();
	parser.rewindFile();

	const std::optional<AprsWXData> result = parser.getLastPacketForStation("SP9UVG", 13);

	BOOST_CHECK(result.has_value());
	BOOST_CHECK(result->valid);
	BOOST_CHECK_EQUAL(result->call, "SP9UVG");
	BOOST_CHECK(result->wind_direction == 225);
}

BOOST_AUTO_TEST_CASE(leskowiec_staticstuff_parseframe)
{
	const std::string input = "2023-08-13 17:51:48.846 SR9NSK-4  R SR9WXL>AKLPRZ,SP9UVG-13*,WIDE2*:!4947.70N/01926.80E_270/001g004t068r...p...P...b09218h75";
	BOOST_TEST_MESSAGE(input);

	std::vector<std::string> separated;
	AprsPacket out;

	// parse line
	boost::split(separated, input, boost::is_any_of(" "));

	AprxLogParser_StaticStuff::parseFrame(separated, out);

	BOOST_CHECK_EQUAL(out.SrcAddr, "SR9WXL");
	BOOST_CHECK_EQUAL(out.SrcSSID, 0);
}

// 2023-08-13 17:30:04.837 SR9NSK-4  d *OK0DSK-6>APDWX0,TCPIP,OK8UOF*:!4937.97N/01815.87E_.../...g...t076; 24,6C WX Kubankov, ASL 660m
BOOST_AUTO_TEST_CASE (ok0dsk_get_next_line)
{
	AprxLogParser parser("./test_input/aprx-rf-ok0dsk.log", true);

	const std::string expected = "2023-08-13 17:30:04.837 SR9NSK-4  d *OK0DSK-6>APDWX0,TCPIP,OK8UOF*:!4937.97N/01815.87E_.../...g...t076; 24,6C WX Kubankov, ASL 660m";
	BOOST_TEST_MESSAGE(expected);

	parser.openFile();

	std::optional<std::string> packet = parser.getNextLine("OK0DSK", 6, false);

	BOOST_CHECK(packet.has_value());
	BOOST_CHECK_EQUAL(expected, *packet);

}

BOOST_AUTO_TEST_CASE(ok0dsk_convert_to_wxdata)
{
	AprxLogParser parser("./test_input/aprx-rf-ok0dsk.log", true);

	const std::string expected = "2023-08-13 17:30:04.837 SR9NSK-4  d *OK0DSK-6>APDWX0,TCPIP,OK8UOF*:!4937.97N/01815.87E_.../...g...t076; 24,6C WX Kubankov, ASL 660m";
	BOOST_TEST_MESSAGE(expected);

	parser.openFile();

		const std::optional<AprsWXData> result = parser.getLastPacketForStation("OK0DSK", 6);

		BOOST_CHECK(result.has_value());
		BOOST_CHECK(result->valid);
		BOOST_CHECK_EQUAL(result->call, "OK0DSK");
		BOOST_CHECK_EQUAL(result->ssid, 6);
		//BOOST_CHECK(result->wind_direction == 0);
		BOOST_CHECK_CLOSE(result->temperature, 24.4, 5);

		BOOST_CHECK(result->packetAgeInSecondsLocal != 0);
		BOOST_CHECK(result->packetAgeInSecondsUtc != 0);
}

// 2022-12-03 14:27:29.321 SR9NSK    R SP9UVG-13>APMI06,SR9GM-2*,SP9DLM*,WIDE2*:@031427z5010.48N/01913.46E_000/007g007t030r000p000P...h91b10213 WX Myslowice
BOOST_AUTO_TEST_CASE (myslowice_convert_to_wxdata)
{
	AprxLogParser parser("./test_input/aprs-rf-2.log", true);

	BOOST_TEST_MESSAGE("2022-12-03 14:27:29.321 SR9NSK    R SP9UVG-13>APMI06,SR9GM-2*,SP9DLM*,WIDE2*:@031427z5010.48N/01913.46E_000/007g007t030r000p000P...h91b10213 WX Myslowice");

	parser.openFile();
	parser.rewindFile();

	const std::optional<AprsWXData> result = parser.getLastPacketForStation("SP9UVG", 13);

	BOOST_CHECK(result.has_value());
	BOOST_CHECK(result->valid);
	BOOST_CHECK_EQUAL(result->call, "SP9UVG");
	BOOST_CHECK_EQUAL(result->ssid, 13);
	BOOST_CHECK(result->wind_direction == 0);
	BOOST_CHECK_CLOSE(result->temperature, -1.1, 5);

	BOOST_CHECK(result->packetAgeInSecondsLocal != 0);
	BOOST_CHECK(result->packetAgeInSecondsUtc != 0);
}

BOOST_AUTO_TEST_CASE (leskowiec_no_weather_in_aprxrfnowxdataleskowiec)
{
	//const std::string expected = "2023-08-13 17:51:48.846 SR9NSK-4  R SR9WXL>AKLPRZ,SP9UVG-13*,WIDE2*:!4947.70N/01926.80E_270/001g004t068r...p...P...b09218h75";

	//BOOST_TEST_MESSAGE(expected);

	AprxLogParser parser("./test_input/aprx-rf-nowxdata-leskowiec.log", true);

	parser.openFile();
	parser.rewindFile();

	const std::optional<AprsWXData> result = parser.getLastPacketForStation("SR9WXL", 0);

	BOOST_CHECK(!result.has_value());
}

BOOST_AUTO_TEST_CASE (nonexistent_aprsrflog2)
{
	AprxLogParser parser("./test_input/aprs-rf-2.log", true);

	parser.openFile();
	parser.rewindFile();

	const std::optional<AprsWXData> result = parser.getLastPacketForStation("dupa", 13);

	BOOST_CHECK(!result.has_value());

}


BOOST_AUTO_TEST_CASE (one_aprsrflog2)
{
	const std::string expected = "2022-12-03 14:05:08.760 SR9NSK    R SR9NWC>APMI03:=4939.19NL01959.53E#PHG2620 W3,SPn 80km Lubon W. alt=1040 Jerzy SP9FBT";

	BOOST_TEST_MESSAGE(expected);

	AprxLogParser parser("./test_input/aprs-rf-2.log", true);

	parser.openFile();

	std::optional<std::string> result = parser.getNextLine("SR9NWC", 0, false);

	BOOST_CHECK(result);
	BOOST_CHECK_EQUAL(result.value(), expected);

}

BOOST_AUTO_TEST_CASE (one_only_wx_aprsrflog2)
{
	AprxLogParser parser("./test_input/aprs-rf-2.log", true);

	parser.openFile();

	std::optional<std::string> result = parser.getNextLine("SR9NWC", 0, true);

	BOOST_CHECK(result.has_value() == false);

}

// 2022-12-03 14:05:29.678 SR9NSK    R SQ5BUJ-7>APAT81,SR7NST*,WIDE1*,WIDE2-1:!5042.54N/02024.29E>198/066/A=000902145.500MHz TG2605 FMLINK JANEK


BOOST_AUTO_TEST_CASE (two_aprsrflog2)
{
	const std::string expected = "2022-12-03 14:05:28.139 SR9NSK    R SR3NZG>APMI06,SR6NWS*,WIDE1*:@292217z5156.05N/01531.88E#Digi/I-Gate Lubuskiego OT PZK nr 32";

	BOOST_TEST_MESSAGE(expected);

	AprxLogParser parser("./test_input/aprs-rf-2.log", true);

	parser.openFile();

	std::optional<std::string> result = parser.getNextLine("SR3NZG", 0, false);

	BOOST_CHECK(result);
	BOOST_CHECK_EQUAL(result.value(), expected);

}

// 2022-12-03 14:18:31.667 SR9NSK    R SR9WXM>AKLPRZ:!4946.58N/01907.84E_200/004g006t036r...p...P...b00000h00

BOOST_AUTO_TEST_CASE (fast_forwarded_aprsrflog2) {
	const std::string expected = "2022-12-03 14:18:33.117 SR9NSK    R SR9WXM>AKLPRZ:!4946.58N/01907.84E_200/004g006t036r...p...P...b00000h00";

	BOOST_TEST_MESSAGE(expected);

	AprxLogParser parser("./test_input/aprs-rf-2.log", true);

	parser.openFile();
	parser.rewindFile();

	std::optional<std::string> result = parser.getNextLine("SR9WXM", 0, true);

	BOOST_CHECK(result);
	BOOST_CHECK_EQUAL(result.value(), expected);

}

BOOST_AUTO_TEST_CASE (parse_aprsrflog2) {
	//const std::string expected = "2022-12-03 14:18:33.117 SR9NSK    R SR9WXM>AKLPRZ:!4946.58N/01907.84E_200/004g006t036r...p...P...b00000h00";

	AprxLogParser parser("./test_input/aprs-rf-2.log", true);

	parser.openFile();
	parser.rewindFile();
	parser.getLastPacketForStation("SR9WXM", 0);


	//BOOST_CHECK(result);
	//BOOST_CHECK_EQUAL(result.value(), expected);

}

BOOST_AUTO_TEST_CASE (getallweatherpackets_epoch_aprsrflog2) {

	AprxLogParser parser("./test_input/aprs-rf-2.log", true);

	const uint64_t epochFrom = 1670074204ULL;	// Your time zone: Saturday, 3 December 2022 14:30:04 GMT+01:00
	const uint64_t epochTo = 1670074504ULL;

	std::vector<AprsWXData>  result = parser.getAllWeatherPacketsInTimerange(epochFrom, epochTo);

	BOOST_CHECK_GE(result.size(), 1);
	BOOST_CHECK_EQUAL("SR9WXM", result[0].call);
	BOOST_CHECK_EQUAL(172, result[0].wind_direction);
	//BOOST_CHECK(result);  // 172
	//BOOST_CHECK_EQUAL(result.value(), expected);	// 97

}

BOOST_AUTO_TEST_CASE (getallpackets_epoch_aprsrflog2) {

	AprxLogParser parser("./test_input/aprs-rf-2.log", true);

	const uint64_t epochFrom = 1670074204ULL;	// Your time zone: Saturday, 3 December 2022 14:30:04 GMT+01:00
	const uint64_t epochTo = 1670074410ULL;		// Date and time (your time zone): Saturday, 3 December 2022 14:33:30 GMT+01:00

	std::vector<AprsPacket>  result = parser.getAllPacketsInTimerange(epochFrom, epochTo);

	BOOST_ASSERT(result.size() > 0);

	const AprsPacket & first = *result.begin();
	const AprsPacket & last = *(result.end() - 1);

	BOOST_CHECK_EQUAL(result.size(), 103);

	// 886 // 2022-12-03 14:30:17.365 APRSIS    R SR9LC-1>APRX29,TCPIP*,qAC,T2ROMANIA:;438.700LC*161707z5041.95N/01838.54Er438.700MHz c071 -760 R60k SR9LC Przemiennik Lubliniecki
	// 988 // 2022-12-03 14:33:25.698 APRSIS    R SR9NDJ>APRX28,TCPIP*,qAC,T2RADOM:;SR9DJK   *000000h5033.45N/01927.01E&DMR Repeater Static TG260/TS1 2609/TS2 operator SP9JKL

	const std::string first_data = ";438.700LC*161707z5041.95N/01838.54Er438.700MHz c071 -760 R60k SR9LC Przemiennik Lubliniecki";
	const std::string second_data = ";SR9DJK   *000000h5033.45N/01927.01E&DMR Repeater Static TG260/TS1 2609/TS2 operator SP9JKL";

	BOOST_CHECK_EQUAL("SR9LC", first.SrcAddr);
	BOOST_CHECK_EQUAL(1, first.SrcSSID);
	BOOST_CHECK_EQUAL("APRX29", first.DestAddr);
	BOOST_CHECK_EQUAL(3, first.Path.size());
	BOOST_CHECK_EQUAL(first_data, first.DataAsStr);

	BOOST_CHECK_EQUAL("SR9NDJ", last.SrcAddr);
	BOOST_CHECK_EQUAL(0, last.SrcSSID);
	BOOST_CHECK_EQUAL("APRX28", last.DestAddr);
	BOOST_CHECK_EQUAL(3, last.Path.size());
	BOOST_CHECK_EQUAL(second_data, last.DataAsStr);
}

