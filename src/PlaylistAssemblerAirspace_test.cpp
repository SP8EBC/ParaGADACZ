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

std::shared_ptr<ConfigurationFile> config_genericairspace_true, config_genericairspace_false, config_genericairspace_futuretime, config_genericairspace_dict;
std::shared_ptr<PlaylistSampler> playlist_sampler;

std::map<std::string, PansaAirspace_Zone> airspaceReservationsAll, airspaceReservationsFirst;
PansaAirspace_Zone first, second, third;

class Test_PlaylistAssemblerAirspace : public PlaylistAssemblerAirspace {

public:
	static int convertMetersToHundretsMeters(int altitude, bool trueToRoundUp) {
		return PlaylistAssemblerAirspace::convertMetersToHundretsMeters(altitude, trueToRoundUp);
	}
};

struct MyConfig
{
  MyConfig() : test_log( "./test_reports/playlistassembler_airspace_test.log" )
  {
    boost::unit_test::unit_test_log.set_stream( test_log );
    boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_level::log_successful_tests);
	//spdlog::set_pattern("[%H:%M:%S.%e %z] [%L] [THR %-5t] [%s:%#] %v" );
	spdlog::set_level(spdlog::level::debug);

	/*
	 * 	ReservationFutureTimeMargin = 600;
	 *	SayPast = true;
	 *	SayAltitudes = true;
	 *	IncludeAirspaceTypeInfo = false;
	 *
	 *	ConfigPerElemType = {
	 *		SayTRA = true;
	 *		SayTSA = false;
	 *		SayATZ = true;
	 *		SayD = false;
	 *		SayR = false;
	 *
	 *		}
	 */
	config_genericairspace_true = std::make_shared<ConfigurationFile>("./test_input/configuration_airspace_aroundpoint_and_fixed_genericanouncementfromregex_true.conf");
	config_genericairspace_true->parse();

	config_genericairspace_false = std::make_shared<ConfigurationFile>("./test_input/configuration_airspace_aroundpoint_and_fixed_genericanouncementfromregex_false.conf");
	config_genericairspace_false->parse();

	config_genericairspace_futuretime = std::make_shared<ConfigurationFile>("./test_input/configuration_airspace_aroundpoint_and_fixed_genericanouncementfromregex_true_futuretime.conf");
	config_genericairspace_futuretime->parse();

	config_genericairspace_dict = std::make_shared<ConfigurationFile>("./test_input/configuration_airspace_aroundpoint_and_fixed_anouncement_dictionary.conf");
	config_genericairspace_dict->parse();

	playlist_sampler = std::make_shared<PlaylistSamplerPL>(config_genericairspace_true);

	first.designator = "ATZ EPBA";
	first.centroidLatitudeY = 49.766487f;
	first.centroidLongitudeX = 18.944265f;
	first.distanceFromSetpoint = 0.0f;
	first.type = AIRSPACE_ATZ;
	first.reservations.emplace_back(1706076000ULL, 1706119200ULL, 0, 1000);
	// Wednesday, 24 January 2024 06:00:00   -->   Wednesday, 24 January 2024 18:00:00

	second.designator = "EPTR10A";
	second.centroidLatitudeY = 50.31573f;
	second.centroidLongitudeX = 21.30986f;
	second.distanceFromSetpoint = 150439.0f;
	second.reservations.emplace_back(1706079600ULL, 1706130000ULL, 0, 1000);
	//  Wednesday, 24 January 2024 07:00:00    ---> Wednesday, 24 January 2024 21:00:00

	third.designator = "EPTR114";
	third.centroidLatitudeY = 52.77208597936472f;
	third.centroidLongitudeX = 23.815587796821536f;
	third.distanceFromSetpoint = 467344.9f;
	third.reservations.emplace_back(1706140800ULL, 1706162400ULL, 0, 1500);
	third.reservations.emplace_back(1789076000ULL, 1789140740ULL, 123, 1400);
	// Thursday, 25 January 2024 00:00:00  --->   Thursday, 25 January 2024 06:00:00
	// Thursday, 10 September 2026 21:33:20  -->  Friday, 11 September 2026 15:32:20

	airspaceReservationsFirst.insert(std::pair<std::string, PansaAirspace_Zone>("ATZ EPBA", first));

	//airspaceReservationsAll.insert(std::pair<std::string, PansaAirspace_Zone>("ATZ EPBA", first));
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

// PlaylistAssemblerAirspace::convertMetersToHundretsMeters
BOOST_AUTO_TEST_CASE(altitude_recalculation)
{
	BOOST_CHECK_EQUAL(1100, Test_PlaylistAssemblerAirspace::convertMetersToHundretsMeters(1001, true));
	BOOST_CHECK_EQUAL(1000, Test_PlaylistAssemblerAirspace::convertMetersToHundretsMeters(1001, false));
	BOOST_CHECK_EQUAL(1400, Test_PlaylistAssemblerAirspace::convertMetersToHundretsMeters(1398, true));
	BOOST_CHECK_EQUAL(1300, Test_PlaylistAssemblerAirspace::convertMetersToHundretsMeters(1398, false));
	BOOST_CHECK_EQUAL(0, Test_PlaylistAssemblerAirspace::convertMetersToHundretsMeters(99, false));
	BOOST_CHECK_EQUAL(100, Test_PlaylistAssemblerAirspace::convertMetersToHundretsMeters(99, true));
	BOOST_CHECK_EQUAL(100, Test_PlaylistAssemblerAirspace::convertMetersToHundretsMeters(100, true));
	BOOST_CHECK_EQUAL(100, Test_PlaylistAssemblerAirspace::convertMetersToHundretsMeters(100, false));


}

BOOST_AUTO_TEST_CASE(epba_around_point_genericanouncementfromregex_false)
{
	PlaylistAssemblerAirspace airspace(playlist_sampler, config_genericairspace_false);
	PlaylistAssembler assembler(playlist_sampler, config_genericairspace_false);

	assembler.start();

	airspace.setPlaylist(assembler.getPlaylist());

	const ConfigurationFile_Airspace & configAirspace = config_genericairspace_false->getAirspace();
	const ConfigurationFile_Airspace_AroundPoint & point = configAirspace.aroundPoint.at(0);

	airspace.reservationsAroundPoint(point, airspaceReservationsFirst);

	const std::shared_ptr<std::vector<std::string> > & playlistOut = airspace.getPlaylist();

	std::vector<std::string> playlistVct = *playlistOut;
	std::vector<std::string>::const_iterator it = playlistVct.begin();

	BOOST_CHECK_EQUAL("intro.ogg", *(it++));
	BOOST_CHECK_EQUAL(OGRANICZENIA_LOTOW, *(it++));
	BOOST_CHECK_EQUAL(W_PROMIENIU, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_12, *(it++));
	BOOST_CHECK_EQUAL(KILOMETER_FOUR, *(it++));
	BOOST_CHECK_EQUAL(OD_LOKALIZACJI, *(it++));
	BOOST_CHECK_EQUAL("beef.mp3", *(it++));
	BOOST_CHECK_EQUAL(AIRSPACE_ATZ_PL, *(it++));
	BOOST_CHECK_EQUAL(PH_ALPHA_PL, *(it++));
	BOOST_CHECK_EQUAL(PH_TANGO_PL, *(it++));
	BOOST_CHECK_EQUAL(PH_ZULU_PL, *(it++));
	BOOST_CHECK_EQUAL(PH_ECHO_PL, *(it++));
	BOOST_CHECK_EQUAL(PH_PAPA_PL, *(it++));
	BOOST_CHECK_EQUAL(PH_BRAVO_PL, *(it++));
	BOOST_CHECK_EQUAL(PH_ALPHA_PL, *(it++));
	BOOST_CHECK_EQUAL(OD, *(it++));
	BOOST_CHECK_EQUAL(SZOSTA, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0, *(it++));
	BOOST_CHECK_EQUAL(UNIWERSALNEGO, *(it++));
	BOOST_CHECK_EQUAL(DO, *(it++));
	BOOST_CHECK_EQUAL(OSIEMNASTA, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0, *(it++));
	BOOST_CHECK_EQUAL(UNIWERSALNEGO, *(it++));
	BOOST_CHECK_EQUAL(WYSOKOSC, *(it++));
	BOOST_CHECK_EQUAL(OD, *(it++));
	BOOST_CHECK_EQUAL(POZIOMU_GRUNTU, *(it++));
	BOOST_CHECK_EQUAL(DO, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_1k, *(it++));
	BOOST_CHECK_EQUAL(METER_FOUR, *(it++));
}

BOOST_AUTO_TEST_CASE(epba_around_point_genericanouncementfromregex_true)
{
	PlaylistAssemblerAirspace airspace(playlist_sampler, config_genericairspace_true);
	PlaylistAssembler assembler(playlist_sampler, config_genericairspace_true);

	assembler.start();

	airspace.setPlaylist(assembler.getPlaylist());

	const ConfigurationFile_Airspace & configAirspace = config_genericairspace_true->getAirspace();
	const ConfigurationFile_Airspace_AroundPoint & point = configAirspace.aroundPoint.at(0);

	airspace.reservationsAroundPoint(point, airspaceReservationsFirst);

	const std::shared_ptr<std::vector<std::string> > & playlistOut = airspace.getPlaylist();

	std::vector<std::string> playlistVct = *playlistOut;
	std::vector<std::string>::const_iterator it = playlistVct.begin();

	BOOST_CHECK_EQUAL("intro.ogg", *(it++));
	BOOST_CHECK_EQUAL(OGRANICZENIA_LOTOW, *(it++));
	BOOST_CHECK_EQUAL(W_PROMIENIU, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_12, *(it++));
	BOOST_CHECK_EQUAL(KILOMETER_FOUR, *(it++));
	BOOST_CHECK_EQUAL(OD_LOKALIZACJI, *(it++));
	BOOST_CHECK_EQUAL("beef.mp3", *(it++));
	BOOST_CHECK_EQUAL(AIRSPACE_ATZ_PL, *(it++));
	BOOST_CHECK_EQUAL(PH_ECHO_PL, *(it++));
	BOOST_CHECK_EQUAL(PH_PAPA_PL, *(it++));
	BOOST_CHECK_EQUAL(PH_BRAVO_PL, *(it++));
	BOOST_CHECK_EQUAL(PH_ALPHA_PL, *(it++));
	BOOST_CHECK_EQUAL(OD, *(it++));
	BOOST_CHECK_EQUAL(SZOSTA, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0, *(it++));
	BOOST_CHECK_EQUAL(UNIWERSALNEGO, *(it++));
	BOOST_CHECK_EQUAL(DO, *(it++));
	BOOST_CHECK_EQUAL(OSIEMNASTA, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0, *(it++));
	BOOST_CHECK_EQUAL(UNIWERSALNEGO, *(it++));
	BOOST_CHECK_EQUAL(WYSOKOSC, *(it++));
	BOOST_CHECK_EQUAL(OD, *(it++));
	BOOST_CHECK_EQUAL(POZIOMU_GRUNTU, *(it++));
	BOOST_CHECK_EQUAL(DO, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_1k, *(it++));
	BOOST_CHECK_EQUAL(METER_FOUR, *(it++));
	BOOST_CHECK(playlistVct.end() == it); ////

}

BOOST_AUTO_TEST_CASE(tra_around_point_genericanouncementfromregex_true)
{
	PlaylistAssemblerAirspace airspace(playlist_sampler, config_genericairspace_true);
	PlaylistAssembler assembler(playlist_sampler, config_genericairspace_true);

	assembler.start();

	airspace.setPlaylist(assembler.getPlaylist());

	const ConfigurationFile_Airspace & configAirspace = config_genericairspace_true->getAirspace();
	const ConfigurationFile_Airspace_AroundPoint & point = configAirspace.aroundPoint.at(0);

	airspace.reservationsAroundPoint(point, airspaceReservationsAll);

	const std::shared_ptr<std::vector<std::string> > & playlistOut = airspace.getPlaylist();

	std::vector<std::string> playlistVct = *playlistOut;
	std::vector<std::string>::const_iterator it = playlistVct.begin();

	BOOST_CHECK_EQUAL("intro.ogg", *(it++));
	BOOST_CHECK_EQUAL(OGRANICZENIA_LOTOW, *(it++));
	BOOST_CHECK_EQUAL(W_PROMIENIU, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_12, *(it++));
	BOOST_CHECK_EQUAL(KILOMETER_FOUR, *(it++));
	BOOST_CHECK_EQUAL(OD_LOKALIZACJI, *(it++));
	BOOST_CHECK_EQUAL("beef.mp3", *(it++));
	BOOST_CHECK_EQUAL(AIRSPACE_TRA_PL, *(it++));
	BOOST_CHECK_EQUAL(PH_TANGO_PL, *(it++));
	BOOST_CHECK_EQUAL(PH_ROMEO_PL, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_1_EN, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0_EN, *(it++));
	BOOST_CHECK_EQUAL(SEKTOR, *(it++));
	BOOST_CHECK_EQUAL(PH_ALPHA_PL, *(it++));
	BOOST_CHECK_EQUAL(OD, *(it++));
	BOOST_CHECK_EQUAL(SIODMA, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0, *(it++));
	BOOST_CHECK_EQUAL(UNIWERSALNEGO, *(it++));
	BOOST_CHECK_EQUAL(DO, *(it++));
	BOOST_CHECK_EQUAL(DWUDZIESTA, *(it++));
	BOOST_CHECK_EQUAL(PIERWSZA, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0, *(it++));
	BOOST_CHECK_EQUAL(UNIWERSALNEGO, *(it++));
	BOOST_CHECK_EQUAL(WYSOKOSC, *(it++));
	BOOST_CHECK_EQUAL(OD, *(it++));
	BOOST_CHECK_EQUAL(POZIOMU_GRUNTU, *(it++));
	BOOST_CHECK_EQUAL(DO, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_1k, *(it++));
	BOOST_CHECK_EQUAL(METER_FOUR, *(it++)); //
	BOOST_CHECK_EQUAL(AIRSPACE_TRA_PL, *(it++));
	BOOST_CHECK_EQUAL(PH_TANGO_PL, *(it++));
	BOOST_CHECK_EQUAL(PH_ROMEO_PL, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_1_EN, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_1_EN, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_4_EN, *(it++));
	BOOST_CHECK_EQUAL(OD, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0, *(it++));
	BOOST_CHECK_EQUAL(UNIWERSALNEGO, *(it++));
	BOOST_CHECK_EQUAL(DO, *(it++));
	BOOST_CHECK_EQUAL(SZOSTA, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0, *(it++));
	BOOST_CHECK_EQUAL(UNIWERSALNEGO, *(it++));
	BOOST_CHECK_EQUAL(WYSOKOSC, *(it++));
	BOOST_CHECK_EQUAL(OD, *(it++));
	BOOST_CHECK_EQUAL(POZIOMU_GRUNTU, *(it++));
	BOOST_CHECK_EQUAL(DO, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_1k, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_500, *(it++));
	BOOST_CHECK_EQUAL(METER_FOUR, *(it++)); ////
	BOOST_CHECK_EQUAL(OD, *(it++));
	BOOST_CHECK_EQUAL(DWUDZIESTA, *(it++));
	BOOST_CHECK_EQUAL(PIERWSZA, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_30, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_3, *(it++));
	BOOST_CHECK_EQUAL(UNIWERSALNEGO, *(it++));
	BOOST_CHECK_EQUAL(DO, *(it++));
	BOOST_CHECK_EQUAL(PIETNASTA, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_30, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_2, *(it++));
	BOOST_CHECK_EQUAL(UNIWERSALNEGO, *(it++));
	BOOST_CHECK_EQUAL(WYSOKOSC, *(it++));
	BOOST_CHECK_EQUAL(OD, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_100, *(it++));
	BOOST_CHECK_EQUAL(METER_FOUR, *(it++));
	BOOST_CHECK_EQUAL(DO, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_1k, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_400, *(it++));
	BOOST_CHECK_EQUAL(METER_FOUR, *(it++));
	BOOST_CHECK(playlistVct.end() == it); ////


}// GODZINA

BOOST_AUTO_TEST_CASE(tra_around_point_genericanouncementfromregex_true_nonzero_future_time)
{
	PlaylistAssemblerAirspace airspace(playlist_sampler, config_genericairspace_futuretime);
	PlaylistAssembler assembler(playlist_sampler, config_genericairspace_futuretime);

	assembler.start();

	airspace.setPlaylist(assembler.getPlaylist());

	const ConfigurationFile_Airspace & configAirspace = config_genericairspace_true->getAirspace();
	const ConfigurationFile_Airspace_AroundPoint & point = configAirspace.aroundPoint.at(0);

	airspace.reservationsAroundPoint(point, airspaceReservationsAll);

	const std::shared_ptr<std::vector<std::string> > & playlistOut = airspace.getPlaylist();

	std::vector<std::string> playlistVct = *playlistOut;
	std::vector<std::string>::const_iterator it = playlistVct.begin();

	BOOST_CHECK_EQUAL("intro.ogg", *(it++));
	BOOST_CHECK_EQUAL(OGRANICZENIA_LOTOW, *(it++));
	BOOST_CHECK_EQUAL(W_PROMIENIU, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_12, *(it++));
	BOOST_CHECK_EQUAL(KILOMETER_FOUR, *(it++));
	BOOST_CHECK_EQUAL(OD_LOKALIZACJI, *(it++));
	BOOST_CHECK_EQUAL("beef.mp3", *(it++));
	BOOST_CHECK_EQUAL(AIRSPACE_TRA_PL, *(it++));
	BOOST_CHECK_EQUAL(PH_TANGO_PL, *(it++));
	BOOST_CHECK_EQUAL(PH_ROMEO_PL, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_1_EN, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0_EN, *(it++));
	BOOST_CHECK_EQUAL(SEKTOR, *(it++));
	BOOST_CHECK_EQUAL(PH_ALPHA_PL, *(it++));
	BOOST_CHECK_EQUAL(OD, *(it++));
	BOOST_CHECK_EQUAL(SIODMA, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0, *(it++));
	BOOST_CHECK_EQUAL(UNIWERSALNEGO, *(it++));
	BOOST_CHECK_EQUAL(DO, *(it++));
	BOOST_CHECK_EQUAL(DWUDZIESTA, *(it++));
	BOOST_CHECK_EQUAL(PIERWSZA, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0, *(it++));
	BOOST_CHECK_EQUAL(UNIWERSALNEGO, *(it++));
	BOOST_CHECK_EQUAL(WYSOKOSC, *(it++));
	BOOST_CHECK_EQUAL(OD, *(it++));
	BOOST_CHECK_EQUAL(POZIOMU_GRUNTU, *(it++));
	BOOST_CHECK_EQUAL(DO, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_1k, *(it++));
	BOOST_CHECK_EQUAL(METER_FOUR, *(it++)); //
	BOOST_CHECK_EQUAL(AIRSPACE_TRA_PL, *(it++));
	BOOST_CHECK_EQUAL(PH_TANGO_PL, *(it++));
	BOOST_CHECK_EQUAL(PH_ROMEO_PL, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_1_EN, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_1_EN, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_4_EN, *(it++));
	BOOST_CHECK_EQUAL(OD, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0, *(it++));
	BOOST_CHECK_EQUAL(UNIWERSALNEGO, *(it++));
	BOOST_CHECK_EQUAL(DO, *(it++));
	BOOST_CHECK_EQUAL(SZOSTA, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0, *(it++));
	BOOST_CHECK_EQUAL(UNIWERSALNEGO, *(it++));
	BOOST_CHECK_EQUAL(WYSOKOSC, *(it++));
	BOOST_CHECK_EQUAL(OD, *(it++));
	BOOST_CHECK_EQUAL(POZIOMU_GRUNTU, *(it++));
	BOOST_CHECK_EQUAL(DO, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_1k, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_500, *(it++));
	BOOST_CHECK_EQUAL(METER_FOUR, *(it++)); ////
	BOOST_CHECK(playlistVct.end() == it); ////

}

BOOST_AUTO_TEST_CASE(epba_aroundpoint_and_fixed_anouncement_dictionary)
{
	PlaylistAssemblerAirspace airspace(playlist_sampler, config_genericairspace_dict);
	PlaylistAssembler assembler(playlist_sampler, config_genericairspace_dict);

	assembler.start();

	airspace.setPlaylist(assembler.getPlaylist());

	const ConfigurationFile_Airspace & configAirspace = config_genericairspace_true->getAirspace();
	const ConfigurationFile_Airspace_AroundPoint & point = configAirspace.aroundPoint.at(0);

	airspace.reservationsAroundPoint(point, airspaceReservationsAll);

	const std::shared_ptr<std::vector<std::string> > & playlistOut = airspace.getPlaylist();

	std::vector<std::string> playlistVct = *playlistOut;
	std::vector<std::string>::const_iterator it = playlistVct.begin();

	BOOST_CHECK_EQUAL("intro.ogg", *(it++));
	BOOST_CHECK_EQUAL(OGRANICZENIA_LOTOW, *(it++));
	BOOST_CHECK_EQUAL(W_PROMIENIU, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_12, *(it++));
	BOOST_CHECK_EQUAL(KILOMETER_FOUR, *(it++));
	BOOST_CHECK_EQUAL(OD_LOKALIZACJI, *(it++));
	BOOST_CHECK_EQUAL("beef.mp3", *(it++));
	BOOST_CHECK_EQUAL("costam.mp3", *(it++));
	BOOST_CHECK_EQUAL(OD, *(it++));
	BOOST_CHECK_EQUAL(SIODMA, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0, *(it++));
	BOOST_CHECK_EQUAL(UNIWERSALNEGO, *(it++));
	BOOST_CHECK_EQUAL(DO, *(it++));
	BOOST_CHECK_EQUAL(DWUDZIESTA, *(it++));
	BOOST_CHECK_EQUAL(PIERWSZA, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0, *(it++));
	BOOST_CHECK_EQUAL(UNIWERSALNEGO, *(it++));
	BOOST_CHECK_EQUAL(WYSOKOSC, *(it++));
	BOOST_CHECK_EQUAL(OD, *(it++));
	BOOST_CHECK_EQUAL(POZIOMU_GRUNTU, *(it++));
	BOOST_CHECK_EQUAL(DO, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_1k, *(it++));
	BOOST_CHECK_EQUAL(METER_FOUR, *(it++)); //
	BOOST_CHECK_EQUAL(AIRSPACE_TRA_PL, *(it++));
	BOOST_CHECK_EQUAL(PH_TANGO_PL, *(it++));
	BOOST_CHECK_EQUAL(PH_ROMEO_PL, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_1_EN, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_1_EN, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_4_EN, *(it++));
	BOOST_CHECK_EQUAL(OD, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0, *(it++));
	BOOST_CHECK_EQUAL(UNIWERSALNEGO, *(it++));
	BOOST_CHECK_EQUAL(DO, *(it++));
	BOOST_CHECK_EQUAL(SZOSTA, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_0, *(it++));
	BOOST_CHECK_EQUAL(UNIWERSALNEGO, *(it++));
	BOOST_CHECK_EQUAL(WYSOKOSC, *(it++));
	BOOST_CHECK_EQUAL(OD, *(it++));
	BOOST_CHECK_EQUAL(POZIOMU_GRUNTU, *(it++));
	BOOST_CHECK_EQUAL(DO, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_1k, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_500, *(it++));
	BOOST_CHECK_EQUAL(METER_FOUR, *(it++)); ////
	BOOST_CHECK_EQUAL(OD, *(it++));
	BOOST_CHECK_EQUAL(DWUDZIESTA, *(it++));
	BOOST_CHECK_EQUAL(PIERWSZA, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_30, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_3, *(it++));
	BOOST_CHECK_EQUAL(UNIWERSALNEGO, *(it++));
	BOOST_CHECK_EQUAL(DO, *(it++));
	BOOST_CHECK_EQUAL(PIETNASTA, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_30, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_2, *(it++));
	BOOST_CHECK_EQUAL(UNIWERSALNEGO, *(it++));
	BOOST_CHECK_EQUAL(WYSOKOSC, *(it++));
	BOOST_CHECK_EQUAL(OD, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_100, *(it++));
	BOOST_CHECK_EQUAL(METER_FOUR, *(it++));
	BOOST_CHECK_EQUAL(DO, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_1k, *(it++));
	BOOST_CHECK_EQUAL(NUMBER_400, *(it++));
	BOOST_CHECK_EQUAL(METER_FOUR, *(it++));
	BOOST_CHECK(playlistVct.end() == it); ////

}

