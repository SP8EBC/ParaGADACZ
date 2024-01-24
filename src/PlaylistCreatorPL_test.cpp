/*
 * PlaylistCreatorPL_test.cpp
 *
 *  Created on: Dec 31, 2022
 *      Author: mateusz
 */

#include "PlaylistSamplerPL.h"
#include "PlaylistSamplerPL_files.h"


#define BOOST_TEST_MODULE PlaylistCreatorPL_test
#include <boost/test/included/unit_test.hpp>
#include <fstream>

#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/pattern_formatter.h"

#include "ConfigurationFile.h"

std::shared_ptr<ConfigurationFile> config;

struct MyConfig
{
  MyConfig() : test_log( "./test_reports/playlistcreatorpl_test.log" )
  {
    boost::unit_test::unit_test_log.set_stream( test_log );
    boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_level::log_successful_tests);
	//spdlog::set_pattern("[%H:%M:%S.%e %z] [%L] [THR %-5t] [%s:%#] %v" );
	spdlog::set_level(spdlog::level::debug);
	config = std::make_shared<ConfigurationFile>("./test_input/configuration_playlist_creator_pl.conf");
	config->parse();
  }
  ~MyConfig()
  {
    boost::unit_test::unit_test_log.set_stream( std::cout );
  }

  std::ofstream test_log;
};

BOOST_GLOBAL_FIXTURE (MyConfig);

BOOST_AUTO_TEST_CASE(phonetic_1) {

	const std::string _test = "dupa";

	std::unique_ptr<PlaylistSampler> c = std::make_unique<PlaylistSamplerPL>(config);

	BOOST_TEST_MESSAGE(_test);
	std::vector<std::string> out = c->getPhoneticForWord(_test);

	BOOST_CHECK_EQUAL(4, out.size());
	BOOST_CHECK_EQUAL(PH_DELTA_PL, out.at(0));
	BOOST_CHECK_EQUAL(PH_UNIFORM_PL, out.at(1));
	BOOST_CHECK_EQUAL(PH_PAPA_PL, out.at(2));
	BOOST_CHECK_EQUAL(PH_ALPHA_PL, out.at(3));

}

BOOST_AUTO_TEST_CASE(phonetic_two_words_in_sequence) {

	const std::string _test = "dupa sto";

	std::unique_ptr<PlaylistSampler> c = std::make_unique<PlaylistSamplerPL>(config);

	BOOST_TEST_MESSAGE(_test);
	std::vector<std::string> out = c->getPhoneticForWord(_test);

	BOOST_CHECK_EQUAL(4, out.size());
	BOOST_CHECK_EQUAL(PH_DELTA_PL, out.at(0));
	BOOST_CHECK_EQUAL(PH_UNIFORM_PL, out.at(1));
	BOOST_CHECK_EQUAL(PH_PAPA_PL, out.at(2));
	BOOST_CHECK_EQUAL(PH_ALPHA_PL, out.at(3));

}

BOOST_AUTO_TEST_CASE(phonetic_two_words_in_sequence_leading_space) {

	const std::string _test = "dupa sto";

	std::unique_ptr<PlaylistSampler> c = std::make_unique<PlaylistSamplerPL>(config);

	BOOST_TEST_MESSAGE(_test);
	std::vector<std::string> out = c->getPhoneticForWord(_test);

	BOOST_CHECK_EQUAL(4, out.size());
	BOOST_CHECK_EQUAL(PH_DELTA_PL, out.at(0));
	BOOST_CHECK_EQUAL(PH_UNIFORM_PL, out.at(1));
	BOOST_CHECK_EQUAL(PH_PAPA_PL, out.at(2));
	BOOST_CHECK_EQUAL(PH_ALPHA_PL, out.at(3));

}

BOOST_AUTO_TEST_CASE(phonetic_wrong_str) {

	const std::string _test = "dupa;;123";

	std::unique_ptr<PlaylistSampler> c = std::make_unique<PlaylistSamplerPL>(config);

	BOOST_TEST_MESSAGE(_test);
	BOOST_CHECK_THROW(std::vector<std::string> out = c->getPhoneticForWord(_test), std::runtime_error);
//
//	BOOST_CHECK_EQUAL(4, out.size());
//	BOOST_CHECK_EQUAL(PH_DELTA_PL, out.at(0));
//	BOOST_CHECK_EQUAL(PH_UNIFORM_PL, out.at(1));
//	BOOST_CHECK_EQUAL(PH_PAPA_PL, out.at(2));
//	BOOST_CHECK_EQUAL(PH_ALPHA_PL, out.at(3));

}

BOOST_AUTO_TEST_CASE(phonetic_with_number) {
	const std::string _test = "dupa123";

	std::unique_ptr<PlaylistSampler> c = std::make_unique<PlaylistSamplerPL>(config);

	BOOST_TEST_MESSAGE(_test);
	std::vector<std::string> out = c->getPhoneticForWord(_test);

	BOOST_CHECK_EQUAL(7, out.size());
	BOOST_CHECK_EQUAL(PH_DELTA_PL, out.at(0));
	BOOST_CHECK_EQUAL(PH_UNIFORM_PL, out.at(1));
	BOOST_CHECK_EQUAL(PH_PAPA_PL, out.at(2));
	BOOST_CHECK_EQUAL(PH_ALPHA_PL, out.at(3));
	BOOST_CHECK_EQUAL(NUMBER_1_EN, out.at(4));
	BOOST_CHECK_EQUAL(NUMBER_2_EN, out.at(5));
	BOOST_CHECK_EQUAL(NUMBER_3_EN, out.at(6));
}

BOOST_AUTO_TEST_CASE(decimal_123_4) {

	std::unique_ptr<PlaylistSampler> c = std::make_unique<PlaylistSamplerPL>(config);

	std::vector<std::string> out = c->getAudioListFromNumber(123.4f);

	BOOST_CHECK_EQUAL(5, out.size());
	BOOST_CHECK_EQUAL(NUMBER_100, out.at(0));
	BOOST_CHECK_EQUAL(NUMBER_20, out.at(1));
	BOOST_CHECK_EQUAL(NUMBER_3, out.at(2));
	BOOST_CHECK_EQUAL(KROPKA, out.at(3));
	BOOST_CHECK_EQUAL(NUMBER_4, out.at(4));

}

// 20.7
BOOST_AUTO_TEST_CASE(decimal_20_7) {

	std::unique_ptr<PlaylistSampler> c = std::make_unique<PlaylistSamplerPL>(config);

	std::vector<std::string> out = c->getAudioListFromNumber(20.7f);

//	BOOST_CHECK_EQUAL(5, out.size());
	BOOST_CHECK_EQUAL(NUMBER_20, out.at(0));
	BOOST_CHECK_EQUAL(KROPKA, out.at(1));
	BOOST_CHECK_EQUAL(NUMBER_7, out.at(2));
}

BOOST_AUTO_TEST_CASE(decimal_1123_4) {

	//spdlog::set_pattern("[%H:%M:%S.%e %z] [%L] [THR %-5t] [%s:%#] %v" );
	spdlog::set_level(spdlog::level::debug);

	std::unique_ptr<PlaylistSampler> c = std::make_unique<PlaylistSamplerPL>(config);

	std::vector<std::string> out = c->getAudioListFromNumber(1123.4f);

	BOOST_CHECK_EQUAL(6, out.size());
	BOOST_CHECK_EQUAL(NUMBER_1k, out.at(0));
	BOOST_CHECK_EQUAL(NUMBER_100, out.at(1));
	BOOST_CHECK_EQUAL(NUMBER_20, out.at(2));
	BOOST_CHECK_EQUAL(NUMBER_3, out.at(3));
	BOOST_CHECK_EQUAL(KROPKA, out.at(4));
	BOOST_CHECK_EQUAL(NUMBER_4, out.at(5));

}

BOOST_AUTO_TEST_CASE(decimal_1103_4) {

	std::unique_ptr<PlaylistSampler> c = std::make_unique<PlaylistSamplerPL>(config);

	std::vector<std::string> out = c->getAudioListFromNumber(1103.4f);

	BOOST_CHECK_EQUAL(5, out.size());
	BOOST_CHECK_EQUAL(NUMBER_1k, out.at(0));
	BOOST_CHECK_EQUAL(NUMBER_100, out.at(1));
	BOOST_CHECK_EQUAL(NUMBER_3, out.at(2));
	BOOST_CHECK_EQUAL(KROPKA, out.at(3));
	BOOST_CHECK_EQUAL(NUMBER_4, out.at(4));

}

BOOST_AUTO_TEST_CASE(decimal_1013_4) {

	std::unique_ptr<PlaylistSampler> c = std::make_unique<PlaylistSamplerPL>(config);

	std::vector<std::string> out = c->getAudioListFromNumber(1013.4f);

	BOOST_CHECK_EQUAL(4, out.size());
	BOOST_CHECK_EQUAL(NUMBER_1k, out.at(0));
	BOOST_CHECK_EQUAL(NUMBER_13, out.at(1));
	BOOST_CHECK_EQUAL(KROPKA, out.at(2));
	BOOST_CHECK_EQUAL(NUMBER_4, out.at(3));

}

BOOST_AUTO_TEST_CASE(decimal_1013) {



	std::unique_ptr<PlaylistSampler> c = std::make_unique<PlaylistSamplerPL>(config);

	std::vector<std::string> out = c->getAudioListFromNumber(1013.0f);

	BOOST_CHECK_EQUAL(2, out.size());
	BOOST_CHECK_EQUAL(NUMBER_1k, out.at(0));
	BOOST_CHECK_EQUAL(NUMBER_13, out.at(1));

}

BOOST_AUTO_TEST_CASE(decimal_999) {

	std::unique_ptr<PlaylistSampler> c = std::make_unique<PlaylistSamplerPL>(config);

	std::vector<std::string> out = c->getAudioListFromNumber(999.0f);

	BOOST_CHECK_EQUAL(3, out.size());
	BOOST_CHECK_EQUAL(NUMBER_900, out.at(0));
	BOOST_CHECK_EQUAL(NUMBER_90, out.at(1));
	BOOST_CHECK_EQUAL(NUMBER_9, out.at(2));

}

BOOST_AUTO_TEST_CASE(decimal_11) {

	std::unique_ptr<PlaylistSampler> c = std::make_unique<PlaylistSamplerPL>(config);

	std::vector<std::string> out = c->getAudioListFromNumber(11.0f);

	BOOST_CHECK_EQUAL(1, out.size());
	BOOST_CHECK_EQUAL(NUMBER_11, out.at(0));


}

BOOST_AUTO_TEST_CASE(integer_11) {
	std::unique_ptr<PlaylistSampler> c = std::make_unique<PlaylistSamplerPL>(config);

	std::vector<std::string> out = c->getAudioListFromNumber((int)11);

	BOOST_CHECK_EQUAL(1, out.size());
	BOOST_CHECK_EQUAL(NUMBER_11, out.at(0));
}

BOOST_AUTO_TEST_CASE(integer_999) {

	std::unique_ptr<PlaylistSampler> c = std::make_unique<PlaylistSamplerPL>(config);

	std::vector<std::string> out = c->getAudioListFromNumber((int)999);

	BOOST_CHECK_EQUAL(3, out.size());
	BOOST_CHECK_EQUAL(NUMBER_900, out.at(0));
	BOOST_CHECK_EQUAL(NUMBER_90, out.at(1));
	BOOST_CHECK_EQUAL(NUMBER_9, out.at(2));

}

BOOST_AUTO_TEST_CASE(current_time) {
	std::unique_ptr<PlaylistSampler> c = std::make_unique<PlaylistSamplerPL>(config);

	auto out = c->getAudioForCurrentTime();

	// results of this test may very depends on current time of day
	std::vector<std::string> playlist = std::get<0>(out);
	boost::posix_time::ptime time = std::get<1>(out);

	// hours and minutes
	const int hours = time.time_of_day().hours();
	const int minutes = time.time_of_day().minutes();

	BOOST_TEST_MESSAGE("playlist content");

	for (std::string elem : playlist) {
		BOOST_TEST_MESSAGE(elem);
	}

	// const
	BOOST_CHECK_EQUAL(*(playlist.begin()), GODZINA);
	BOOST_CHECK_EQUAL(*(playlist.end() - 1), UNIWERSALNEGO);

	int i = 1;

	// then check parts which depends on current time
	switch (hours) {
		case 0: BOOST_CHECK_EQUAL(NUMBER_0, playlist.at(i++)); break;
		case 1: BOOST_CHECK_EQUAL(PIERWSZA, playlist.at(i++)); break;
		case 2: BOOST_CHECK_EQUAL(DRUGA, playlist.at(i++)); break;
		case 3: BOOST_CHECK_EQUAL(TRZECIA, playlist.at(i++)); break;
		case 4: BOOST_CHECK_EQUAL(CZWARTA, playlist.at(i++)); break;
		case 5: BOOST_CHECK_EQUAL(PIATA, playlist.at(i++)); break;
		case 6: BOOST_CHECK_EQUAL(SZOSTA, playlist.at(i++)); break;
		case 7: BOOST_CHECK_EQUAL(SIODMA, playlist.at(i++)); break;
		case 8: BOOST_CHECK_EQUAL(OSMA, playlist.at(i++)); break;
		case 9: BOOST_CHECK_EQUAL(DZIEWIATA, playlist.at(i++)); break;
		case 10: BOOST_CHECK_EQUAL(DZIESIATA, playlist.at(i++)); break;
		case 11: BOOST_CHECK_EQUAL(JEDENASTA, playlist.at(i++)); break;
		case 12: BOOST_CHECK_EQUAL(DWUNASTA, playlist.at(i++)); break;
		case 13: BOOST_CHECK_EQUAL(TRZYNASTA, playlist.at(i++)); break;
		case 14: BOOST_CHECK_EQUAL(CZTERNASTA, playlist.at(i++)); break;
		case 15: BOOST_CHECK_EQUAL(PIETNASTA, playlist.at(i++)); break;
		case 16: BOOST_CHECK_EQUAL(SZESNASTA, playlist.at(i++)); break;
		case 17: BOOST_CHECK_EQUAL(SIEDEMNASTA, playlist.at(i++)); break;
		case 18: BOOST_CHECK_EQUAL(OSIEMNASTA, playlist.at(i++)); break;
		case 19: BOOST_CHECK_EQUAL(DZIEWIETNASTA, playlist.at(i++)); break;
		case 20: BOOST_CHECK_EQUAL(DWUDZIESTA, playlist.at(i++)); break;
		case 21: BOOST_CHECK_EQUAL(DWUDZIESTA, playlist.at(i++)); BOOST_CHECK_EQUAL(PIERWSZA, playlist.at(i++)); break;
		case 22: BOOST_CHECK_EQUAL(DWUDZIESTA, playlist.at(i++)); BOOST_CHECK_EQUAL(DRUGA, playlist.at(i++)); break;
		case 23: BOOST_CHECK_EQUAL(DWUDZIESTA, playlist.at(i++)); BOOST_CHECK_EQUAL(TRZECIA, playlist.at(i++)); break;
	}

	if (minutes > 19 || minutes < 10) {

		// minutes - first digit
		switch (minutes - (minutes % 10)) {
			//case 0: BOOST_CHECK_EQUAL(NUMBER_0, *(playlist.end() - 2)); break;
	//		case 10: BOOST_CHECK_EQUAL(NUMBER_1, *(playlist.end() - 2)); break;
			case 20: BOOST_CHECK_EQUAL(NUMBER_20, playlist.at(i++)); break;
			case 30: BOOST_CHECK_EQUAL(NUMBER_30, playlist.at(i++)); break;
			case 40: BOOST_CHECK_EQUAL(NUMBER_40, playlist.at(i++)); break;
			case 50: BOOST_CHECK_EQUAL(NUMBER_50, playlist.at(i++)); break;
		}

		// minutes - second digit
		switch (minutes % 10) {
			//case 0: BOOST_CHECK_EQUAL(NUMBER_0, *(playlist.end() - 2)); break;
			case 1: BOOST_CHECK_EQUAL(NUMBER_1, playlist.at(i++)); break;
			case 2: BOOST_CHECK_EQUAL(NUMBER_2, playlist.at(i++)); break;
			case 3: BOOST_CHECK_EQUAL(NUMBER_3, playlist.at(i++)); break;
			case 4: BOOST_CHECK_EQUAL(NUMBER_4, playlist.at(i++)); break;
			case 5: BOOST_CHECK_EQUAL(NUMBER_5, playlist.at(i++)); break;
			case 6: BOOST_CHECK_EQUAL(NUMBER_6, playlist.at(i++)); break;
			case 7: BOOST_CHECK_EQUAL(NUMBER_7, playlist.at(i++)); break;
			case 8: BOOST_CHECK_EQUAL(NUMBER_8, playlist.at(i++)); break;
			case 9: BOOST_CHECK_EQUAL(NUMBER_9, playlist.at(i++)); break;
		}
	}
	else {
		switch (minutes ) {
			case 10: BOOST_CHECK_EQUAL(NUMBER_10, playlist.at(i++)); break;
			case 11: BOOST_CHECK_EQUAL(NUMBER_11, playlist.at(i++)); break;
			case 12: BOOST_CHECK_EQUAL(NUMBER_12, playlist.at(i++)); break;
			case 13: BOOST_CHECK_EQUAL(NUMBER_13, playlist.at(i++)); break;
			case 14: BOOST_CHECK_EQUAL(NUMBER_14, playlist.at(i++)); break;
			case 15: BOOST_CHECK_EQUAL(NUMBER_15, playlist.at(i++)); break;
			case 16: BOOST_CHECK_EQUAL(NUMBER_16, playlist.at(i++)); break;
			case 17: BOOST_CHECK_EQUAL(NUMBER_17, playlist.at(i++)); break;
			case 18: BOOST_CHECK_EQUAL(NUMBER_18, playlist.at(i++)); break;
			case 19: BOOST_CHECK_EQUAL(NUMBER_19, playlist.at(i++)); break;
		}
	}
}

BOOST_AUTO_TEST_CASE(get_audio_for_skrzyczne) {
	std::unique_ptr<PlaylistSampler> c = std::make_unique<PlaylistSamplerPL>(config);

//	bool parsing_result = config->parse();
//
//	BOOST_CHECK(parsing_result);

	// intentionally check with lowerase, while config file
	std::optional<std::string> result = c->getAudioForStationName("skrzyczne");

	BOOST_CHECK(result.has_value());
	BOOST_CHECK_EQUAL(result.value(), "skrzyczne.mp3");
}

BOOST_AUTO_TEST_CASE(get_audio_for_nonexistent_station) {

	std::unique_ptr<PlaylistSampler> c = std::make_unique<PlaylistSamplerPL>(config);

//	bool parsing_result = config->parse();
//
//	BOOST_CHECK(parsing_result);

	std::optional<std::string> result = c->getAudioForStationName("dupaduapaduadojiojorihg[iorhr[h");

	BOOST_CHECK(!result.has_value());
}

BOOST_AUTO_TEST_CASE(get_audio_for_meteoblue_jezioro) {

	std::unique_ptr<PlaylistSampler> c = std::make_unique<PlaylistSamplerPL>(config);

//	bool parsing_result = config->parse();
//
//	BOOST_CHECK(parsing_result);

	// intentionally check with lowerase, while config file
	std::optional<std::string> result = c->getAudioForForecastName("JEZIORO");

	BOOST_CHECK(result.has_value());
	BOOST_CHECK_EQUAL(result.value(), "costam2.mp3");
}

