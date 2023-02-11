/*
 * AprxLogParser_test.cpp
 *
 *  Created on: Dec 3, 2022
 *      Author: mateusz
 */

#include "AprxLogParser.h"

#define BOOST_TEST_MODULE AprxLogParser_test
#include <boost/test/included/unit_test.hpp>
#include <fstream>

struct MyConfig
{
  MyConfig() : test_log( "./test_reports/aprxlogparser_test.log" )
  {
    boost::unit_test::unit_test_log.set_stream( test_log );
    boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_level::log_successful_tests);

  }
  ~MyConfig()
  {
    boost::unit_test::unit_test_log.set_stream( std::cout );
  }

  std::ofstream test_log;
};

BOOST_GLOBAL_FIXTURE (MyConfig);

BOOST_AUTO_TEST_CASE (with_ssid)
{
	const std::string expected = "2022-12-03 14:05:29.678 SR9NSK    R SQ5BUJ-7>APAT81,SR7NST*,WIDE1*,WIDE2-1:!5042.54N/02024.29E>198/066/A=000902145.500MHz TG2605 FMLINK JANEK";

	AprxLogParser parser("./test_input/aprs-rf-2.log");

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

	AprxLogParser parser("./test_input/aprs-rf-2.log");

	parser.openFile();

	std::optional<std::string> result = parser.getNextLine("SR6WXP", 0, true);
	BOOST_CHECK(result);
	BOOST_CHECK_EQUAL(result.value(), first_expected);

	std::optional<std::string> result2 = parser.getNextLine("SR6WXP", 0, true);
	BOOST_CHECK(result2);
	BOOST_CHECK_EQUAL(result2.value(), second_expected);
}

//2022-12-03 14:06:07.521 SR9NSK    R SR9NCI>AKLPRZ,WIDE2-1::SR9NCI   :EQNS.0,1,0,0,1,0,0,1,0,0,1,0,0,0.25,-25
BOOST_AUTO_TEST_CASE (cieszyn_find_first_no_wx_only)
{
	const std::string expected = "2022-12-03 14:06:07.521 SR9NSK    R SR9NCI>AKLPRZ,WIDE2-1::SR9NCI   :EQNS.0,1,0,0,1,0,0,1,0,0,1,0,0,0.25,-25";

	AprxLogParser parser("./test_input/aprs-rf-2.log");

	parser.openFile();

	std::optional<std::string> result = parser.getNextLine("SR9NCI", 0, false);

	BOOST_CHECK(result);
	BOOST_CHECK_EQUAL(result.value(), expected);
}

BOOST_AUTO_TEST_CASE (cieszyn_find_first_wx_only)
{
	const std::string expected = "2022-12-03 14:09:11.801 SR9NSK    R SR9NCI>AKLPRZ,WIDE2-1:!4944.22N/01838.20E_013/000g002t037r...p...P...b00000";

	AprxLogParser parser("./test_input/aprs-rf-2.log");

	parser.openFile();

	std::optional<std::string> result = parser.getNextLine("SR9NCI", 0, true);

	BOOST_CHECK(result);
	BOOST_CHECK_EQUAL(result.value(), expected);
}

BOOST_AUTO_TEST_CASE (cieszyn_convert_to_wxdata)
{
	const std::string expected = "2022-12-03 14:29:22.996 SR9NSK    R SR9NCI>AKLPRZ,SR9GM-2*,WIDE2*:!4944.22N/01838.20E_045/005g007t036r...p...P...b00000";

	BOOST_TEST_MESSAGE("2022-12-03 14:29:22.996 SR9NSK    R SR9NCI>AKLPRZ,SR9GM-2*,WIDE2*:!4944.22N/01838.20E_045/005g007t036r...p...P...b00000");


	AprxLogParser parser("./test_input/aprs-rf-2.log");

	parser.openFile();
	parser.rewindFile();

	const std::optional<AprsWXData> result = parser.getLastPacketForStation("SR9NCI", 0);

	BOOST_CHECK(result.has_value());
	BOOST_CHECK(result->valid);
	BOOST_CHECK_EQUAL(result->call, "SR9NCI");
	BOOST_CHECK(result->wind_direction == 45);
}

// 2022-12-03 14:27:29.321 SR9NSK    R SP9UVG-13>APMI06,SR9GM-2*,SP9DLM*,WIDE2*:@031427z5010.48N/01913.46E_000/007g007t030r000p000P...h91b10213 WX Myslowice
BOOST_AUTO_TEST_CASE (myslowice_convert_to_wxdata)
{
	AprxLogParser parser("./test_input/aprs-rf-2.log");

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
}

BOOST_AUTO_TEST_CASE (nonexistent)
{
	AprxLogParser parser("./test_input/aprs-rf-2.log");

	parser.openFile();
	parser.rewindFile();

	const std::optional<AprsWXData> result = parser.getLastPacketForStation("dupa", 13);

	BOOST_CHECK(!result.has_value());

}


BOOST_AUTO_TEST_CASE (one)
{
	const std::string expected = "2022-12-03 14:05:08.760 SR9NSK    R SR9NWC>APMI03:=4939.19NL01959.53E#PHG2620 W3,SPn 80km Lubon W. alt=1040 Jerzy SP9FBT";

	AprxLogParser parser("./test_input/aprs-rf-2.log");

	parser.openFile();

	std::optional<std::string> result = parser.getNextLine("SR9NWC", 0, false);

	BOOST_CHECK(result);
	BOOST_CHECK_EQUAL(result.value(), expected);

}

BOOST_AUTO_TEST_CASE (one_only_wx)
{
	AprxLogParser parser("./test_input/aprs-rf-2.log");

	parser.openFile();

	std::optional<std::string> result = parser.getNextLine("SR9NWC", 0, true);

	BOOST_CHECK(result.has_value() == false);

}

// 2022-12-03 14:05:29.678 SR9NSK    R SQ5BUJ-7>APAT81,SR7NST*,WIDE1*,WIDE2-1:!5042.54N/02024.29E>198/066/A=000902145.500MHz TG2605 FMLINK JANEK


BOOST_AUTO_TEST_CASE (two)
{
	const std::string expected = "2022-12-03 14:05:28.139 SR9NSK    R SR3NZG>APMI06,SR6NWS*,WIDE1*:@292217z5156.05N/01531.88E#Digi/I-Gate Lubuskiego OT PZK nr 32";

	AprxLogParser parser("./test_input/aprs-rf-2.log");

	parser.openFile();

	std::optional<std::string> result = parser.getNextLine("SR3NZG", 0, false);

	BOOST_CHECK(result);
	BOOST_CHECK_EQUAL(result.value(), expected);

}

// 2022-12-03 14:18:31.667 SR9NSK    R SR9WXM>AKLPRZ:!4946.58N/01907.84E_200/004g006t036r...p...P...b00000h00

BOOST_AUTO_TEST_CASE (fast_forwarded) {
	const std::string expected = "2022-12-03 14:18:33.117 SR9NSK    R SR9WXM>AKLPRZ:!4946.58N/01907.84E_200/004g006t036r...p...P...b00000h00";

	AprxLogParser parser("./test_input/aprs-rf-2.log");

	parser.openFile();
	parser.rewindFile();

	std::optional<std::string> result = parser.getNextLine("SR9WXM", 0, true);

	BOOST_CHECK(result);
	BOOST_CHECK_EQUAL(result.value(), expected);

}

BOOST_AUTO_TEST_CASE (parse) {
	//const std::string expected = "2022-12-03 14:18:33.117 SR9NSK    R SR9WXM>AKLPRZ:!4946.58N/01907.84E_200/004g006t036r...p...P...b00000h00";

	AprxLogParser parser("./test_input/aprs-rf-2.log");

	parser.openFile();
	parser.rewindFile();
	parser.getLastPacketForStation("SR9WXM", 0);


	//BOOST_CHECK(result);
	//BOOST_CHECK_EQUAL(result.value(), expected);

}

