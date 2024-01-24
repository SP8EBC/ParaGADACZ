/*
 * PlaylistAssemblerAirspace_test.cpp
 *
 *  Created on: Jan 17, 2024
 *      Author: mateusz
 */

#define BOOST_TEST_MODULE PlaylistAssemblerAirspace_test
#include <boost/test/included/unit_test.hpp>

#pragma push_macro("U")
#undef U
// pragma required as a workaround of possible conflict with cpprestsdk.
// more info here: https://github.com/fmtlib/fmt/issues/3330
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#pragma pop_macro("U")

#include "PlaylistAssemblerAirspace.h"
#include "PlaylistAssembler.h"
#include "PlaylistSamplerPL.h"
#include "PlaylistSamplerPL_files.h"

#include <fstream>
#include <memory>

#include "ConfigurationFile.h"

std::shared_ptr<ConfigurationFile> config;
std::shared_ptr<PlaylistSampler> playlist_sampler;

std::map<std::string, PansaAirspace_Zone> airspaceReservationsAll, airspaceReservationsFirst;
PansaAirspace_Zone first, second, third;

struct MyConfig
{
  MyConfig() : test_log( "./test_reports/playlistassembler_airspace_test.log" )
  {
    boost::unit_test::unit_test_log.set_stream( test_log );
    boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_level::log_successful_tests);
	//spdlog::set_pattern("[%H:%M:%S.%e %z] [%L] [THR %-5t] [%s:%#] %v" );
	spdlog::set_level(spdlog::level::debug);
	config = std::make_shared<ConfigurationFile>("./test_input/configuration_airspace_for_playlist_assembler_test.conf");
	config->parse();

	playlist_sampler = std::make_shared<PlaylistSamplerPL>(config);

	first.designator = "ATZ EPBA";
	first.centroidLatitudeY = 49.766487f;
	first.centroidLongitudeX = 18.944265f;
	first.distanceFromSetpoint = 0.0f;
	first.type = AIRSPACE_ATZ;
	first.reservations.emplace_back(1706076000ULL, 1706119200ULL, 0, 1000);

	second.designator = "EPTR10A";
	second.centroidLatitudeY = 50.31573f;
	second.centroidLongitudeX = 21.30986f;
	second.distanceFromSetpoint = 150439.0f;
	second.reservations.emplace_back(1706079600ULL, 1706130000ULL, 0, 1000);

	third.designator = "EPTR114";
	third.centroidLatitudeY = 52.77208597936472f;
	third.centroidLongitudeX = 23.815587796821536f;
	third.distanceFromSetpoint = 467344.9f;
	third.reservations.emplace_back(1706140800ULL, 1706162400ULL, 0, 1500);
	third.reservations.emplace_back(1706076000ULL, 1706140740ULL, 0, 1400);

	airspaceReservationsFirst.insert(std::pair<std::string, PansaAirspace_Zone>("ATZ EPBA", first));

	airspaceReservationsAll.insert(std::pair<std::string, PansaAirspace_Zone>("ATZ EPBA", first));
	airspaceReservationsAll.insert(std::pair<std::string, PansaAirspace_Zone>("EPTR10A", second));
	airspaceReservationsAll.insert(std::pair<std::string, PansaAirspace_Zone>("EPTR114", third));

  }
  ~MyConfig()
  {
    boost::unit_test::unit_test_log.set_stream( std::cout );
  }

  std::ofstream test_log;
};

BOOST_GLOBAL_FIXTURE (MyConfig);

BOOST_AUTO_TEST_CASE(first_test)
{
	PlaylistAssemblerAirspace airspace(playlist_sampler, config);
	PlaylistAssembler assembler(playlist_sampler, config);

	assembler.start();

	airspace.setPlaylist(assembler.getPlaylist());

	const ConfigurationFile_Airspace & configAirspace = config->getAirspace();
	const ConfigurationFile_Airspace_AroundPoint & point = configAirspace.aroundPoint.at(0);

	airspace.reservationsAroundPoint(point, airspaceReservationsFirst);

	const std::shared_ptr<std::vector<std::string> > & playlistOut = airspace.getPlaylist();

	std::vector<std::string> playlistVct = *playlistOut;
	int i = 0;

	BOOST_CHECK_EQUAL("intro.ogg", playlistVct[i++]);
	BOOST_CHECK_EQUAL(OGRANICZENIA_LOTOW, playlistVct[i++]);
	BOOST_CHECK_EQUAL(W_PROMIENIU, playlistVct[i++]);
	BOOST_CHECK_EQUAL(NUMBER_12, playlistVct[i++]);
	BOOST_CHECK_EQUAL(KILOMETER_FOUR, playlistVct[i++]);
	BOOST_CHECK_EQUAL(OD_LOKALIZACJI, playlistVct[i++]);
	BOOST_CHECK_EQUAL("beef.mp3", playlistVct[i++]);
	BOOST_CHECK_EQUAL(AIRSPACE_ATZ_PL, playlistVct[i++]);
	BOOST_CHECK_EQUAL(PH_ECHO_PL, playlistVct[i++]);
	BOOST_CHECK_EQUAL(PH_PAPA_PL, playlistVct[i++]);
	BOOST_CHECK_EQUAL(PH_BRAVO_PL, playlistVct[i++]);
	BOOST_CHECK_EQUAL(PH_ALPHA_PL, playlistVct[i++]);

}
