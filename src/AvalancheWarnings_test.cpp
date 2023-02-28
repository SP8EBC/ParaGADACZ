/*
 * AvalancheWarnings_test.cpp
 *
 *  Created on: Feb 26, 2023
 *      Author: mateusz
 */

#include "AvalancheWarnings.h"

#define BOOST_TEST_MODULE AvalancheWarning_test
#include <boost/test/included/unit_test.hpp>
#include <fstream>
#include <istream>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/bin_to_hex.h"

class AvalancheWarningsTest : public AvalancheWarnings {

public :
	void parseWrapper(std::string r) {
		this->parseLevel(r);
	}

	void parseExpositionWrapper(std::string r) {
		this->parseDangerousExposition(r);
	}
};

AvalancheWarningsTest test;

std::ifstream html_test_input( static_cast<const char *>("./test_input/lawiny_gopr_topr/gopr_zarzad_babia_gora.html") );

std::string buffer, master_buffer;

struct MyConfig
{
  MyConfig() : test_log( "./test_reports/avalanchewarning_test.log" )
  {
    boost::unit_test::unit_test_log.set_stream( test_log );
    boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_level::log_successful_tests);

	spdlog::set_level(spdlog::level::debug);

	if (html_test_input.is_open()) {
		//html_test_input.get(buffer);
		while (std::getline(html_test_input, buffer)) {
			master_buffer.append(buffer);
		}
	}

  }
  ~MyConfig()
  {
    boost::unit_test::unit_test_log.set_stream( std::cout );
  }

  std::ofstream test_log;

};

BOOST_GLOBAL_FIXTURE (MyConfig);

BOOST_AUTO_TEST_CASE(babia_exposition) {


	BOOST_CHECK_NO_THROW(test.parseExpositionWrapper(master_buffer));

}

BOOST_AUTO_TEST_CASE(babia) {

	BOOST_CHECK_NO_THROW(test.parseWrapper(master_buffer));

	BOOST_CHECK_EQUAL(2, test.getSeverityLevel());
}

BOOST_AUTO_TEST_CASE(babia_real_http) {
	int code = 0;

	BOOST_CHECK_NO_THROW(code = test.download(BABIA_GORA));
	BOOST_CHECK_EQUAL(200, code);

	BOOST_CHECK_NO_THROW(test.parseLevel());

	BOOST_CHECK_GE(test.getSeverityLevel(), 1);

}
