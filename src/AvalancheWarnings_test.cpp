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
		this->parse(r);
	}
};

AvalancheWarningsTest test;

std::ifstream html_test_input( static_cast<const char *>("./test_input/lawiny_gopr_topr/gopr_zarzad_babia_gora.html") );

struct MyConfig
{
  MyConfig() : test_log( "./test_reports/playlistassembler_test.log" )
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

BOOST_AUTO_TEST_CASE(babia) {

	std::string buffer, master_buffer;

	if (html_test_input.is_open()) {
		//html_test_input.get(buffer);
		while (std::getline(html_test_input, buffer)) {
			master_buffer.append(buffer);
		}
	}

	test.parseWrapper(master_buffer);
}
