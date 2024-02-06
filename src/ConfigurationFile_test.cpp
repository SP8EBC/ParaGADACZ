/*
 * ConfigurationFile_test.cpp
 *
 *  Created on: Dec 24, 2022
 *      Author: mateusz
 */

#include "ConfigurationFile.h"

#define BOOST_TEST_MODULE ConfigurationFile_test
#include <boost/test/included/unit_test.hpp>
#include <fstream>

#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/pattern_formatter.h"

struct MyConfig
{
  MyConfig() : test_log( "./test_reports/configuraionfile_test.log" )
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

BOOST_AUTO_TEST_CASE (split_callsign)
{
	std::string test = "SP8EBC-11";

	std::tuple<std::string, unsigned> result = ConfigurationFile::splitCallsign(test);

	BOOST_CHECK_EQUAL(std::get<std::string>(result), "SP8EBC");
	BOOST_CHECK_EQUAL(std::get<unsigned>(result), 11);
}

BOOST_AUTO_TEST_CASE (split_callsign_no_ssid)
{
	std::string test = "SP8EBC";

	std::tuple<std::string, unsigned> result = ConfigurationFile::splitCallsign(test);

	BOOST_CHECK_EQUAL(std::get<std::string>(result), "SP8EBC");
	BOOST_CHECK_EQUAL(std::get<unsigned>(result), 0);
}

BOOST_AUTO_TEST_CASE (inhibitor_parse)
{
	ConfigurationFile configurationFile("./test_input/configuration_inhibitor_parse.conf");

	bool result = configurationFile.parse();

	BOOST_CHECK_EQUAL(result, true);
	/////////////////////////////////////
	const ConfigurationFile_Inhibitor inhibitor = configurationFile.getInhibitor();
	BOOST_CHECK_EQUAL(inhibitor.serial.enable, true);
	BOOST_CHECK_EQUAL(inhibitor.serial.okActiveLevel, false);
	BOOST_CHECK_EQUAL(inhibitor.serial.port, "/dev/ttyS0");

	BOOST_CHECK_EQUAL(inhibitor.http.enable, true);
	BOOST_CHECK_EQUAL(inhibitor.http.ignoreNoAnswer, true);
	BOOST_CHECK_EQUAL(inhibitor.http.url, "http://pogoda.cc");

	BOOST_CHECK_EQUAL(inhibitor.exec.enable, true);
	BOOST_CHECK_EQUAL(inhibitor.exec.okRetval, 0);
	BOOST_CHECK_EQUAL(inhibitor.exec.path, "/bin/more");
	////////////////////////////////////////
//	const ConfigurationFile_Avalanche avalanceWarning = configurationFile.getAvalancheWarning();
//	BOOST_CHECK_EQUAL(avalanceWarning.goprBabiaGora, true);
}

BOOST_AUTO_TEST_CASE (simple_parse)
{
	ConfigurationFile configurationFile("./test_input/configuration_simple_parse.conf");

	//spdlog::set_pattern("[%H:%M:%S.%e %z] [%L] [THR %-5t] [%s:%#] %v" );

	bool result = configurationFile.parse();

	BOOST_CHECK_EQUAL(result, true);
	/////////////////
	BOOST_CHECK_EQUAL(configurationFile.getIntro().ident.at(0), "intro.ogg");
	BOOST_CHECK_EQUAL(configurationFile.getIntro().sayCurrentTime, false);
	BOOST_CHECK_EQUAL(configurationFile.getIntro().sayRegionalPressure, true);
	/////////////////
	BOOST_CHECK_EQUAL(configurationFile.isDebug(), false);
	BOOST_CHECK_EQUAL(configurationFile.getLogOutput(), "gadacz.log");
	BOOST_CHECK_EQUAL(configurationFile.getMaximumDataAgeInMinutes(), 60);
	BOOST_CHECK_EQUAL(configurationFile.getAprxConfig().aprxRfLogPath, "/home/mateusz/Documents/test/aprx/aprx-rf.log");
	/////////////////
	BOOST_CHECK_EQUAL(configurationFile.getCurrent().size(), 2);
	BOOST_CHECK_EQUAL(configurationFile.getCurrent().at(0).name, "SR9WXM");
	BOOST_CHECK_EQUAL(configurationFile.getCurrent().at(0).type, APRX);
	BOOST_CHECK_EQUAL(configurationFile.getCurrent().at(0).fnIdent, "magurka.mp3");
	BOOST_CHECK_EQUAL(configurationFile.getCurrent().at(0).sayHumidy, true);
	BOOST_CHECK_EQUAL(configurationFile.getCurrent().at(0).sayTemperature, true);
	BOOST_CHECK_EQUAL(configurationFile.getCurrent().at(0).sayWind, true);
	BOOST_CHECK_EQUAL(configurationFile.getCurrent().at(0).sayPressure, false);
	BOOST_CHECK_EQUAL(configurationFile.getCurrent().at(0).regionalPressure, true);
	/////////////////
	BOOST_CHECK_EQUAL(configurationFile.getForecast().futureTime , 180);
	BOOST_CHECK_EQUAL(configurationFile.getForecast().locations.size(), 2);
	BOOST_CHECK_EQUAL(configurationFile.getForecast().locations.at(0).name, "Skrzyczne");
	BOOST_CHECK_EQUAL(configurationFile.getForecast().locations.at(0).nameIdent, "costam.mp3");
	BOOST_CHECK_EQUAL(configurationFile.getForecast().locations.at(0).sayPrecipation, true);
	BOOST_CHECK_EQUAL(configurationFile.getForecast().locations.at(0).sayTemperature, true);
	BOOST_CHECK_EQUAL(configurationFile.getForecast().locations.at(0).sayWind, true);


}

BOOST_AUTO_TEST_CASE (parsing_pogodacc_config)
{
	ConfigurationFile configurationFile("./test_input/configuration_simple_parse.conf");

	bool result = configurationFile.parse();

	BOOST_CHECK_EQUAL(result, true);
	///////////////////////////////////////////
	BOOST_CHECK_EQUAL(configurationFile.getPogodaCc().baseUrl, "http://pogoda.cc:8080/meteo_backend");
	BOOST_CHECK_EQUAL(configurationFile.getPogodaCc().ignoreTemperatureQf, true);
	BOOST_CHECK_EQUAL(configurationFile.getPogodaCc().ignoreHumidityQf, true);
	BOOST_CHECK_EQUAL(configurationFile.getPogodaCc().ignorePressureQf, false);
	BOOST_CHECK_EQUAL(configurationFile.getPogodaCc().ignoreWindQf, false);

}

BOOST_AUTO_TEST_CASE (email_announcements)
{
	ConfigurationFile configurationFile("./test_input/configuration_forecast_email_speech_synthesis.conf");

	bool result = configurationFile.parse();

	BOOST_CHECK_EQUAL(result, true);

	const ConfigurationFile_Email& em = configurationFile.getEmailAnnonuncements();

	BOOST_CHECK_EQUAL(0x100, em.maximumLenghtInWords);
	BOOST_CHECK_EQUAL(true, em.enabled);

	BOOST_CHECK_EQUAL("poczta.interia.pl", em.serverConfig.imapAddress);
	BOOST_CHECK_EQUAL(995, em.serverConfig.imapPort);
	BOOST_CHECK_EQUAL("sp8ebc@interia.pl", em.serverConfig.username);
	BOOST_CHECK_EQUAL(false, em.serverConfig.startTls);

	BOOST_CHECK_EQUAL(1, em.allowedSendersList.size());
	ConfigurationFile_Email_AllowedSender sender = em.allowedSendersList.at(0);

	BOOST_CHECK_EQUAL("sklep@8a.pl", sender.emailAddress);
	BOOST_CHECK_EQUAL(true, sender.singleAnnouncement);
	BOOST_CHECK_EQUAL(true, sender.eodAnnouncement);
	BOOST_CHECK_EQUAL(false, sender.timedAnnouncement);
	BOOST_CHECK_EQUAL(true, sender.defaultAnnouncement);
	BOOST_CHECK_EQUAL(88, sender.defaultAnnouncementLn);

	BOOST_CHECK_EQUAL(1, sender.preprocessing.startFromParagraph);
	BOOST_CHECK_EQUAL(3, sender.preprocessing.endOnParagraph);
	BOOST_CHECK_EQUAL(true, sender.preprocessing.inhibitSizeLimit);

}

BOOST_AUTO_TEST_CASE(tts)
{
	ConfigurationFile configurationFile("./test_input/configuration_forecast_email_speech_synthesis.conf");

	bool result = configurationFile.parse();

	BOOST_CHECK_EQUAL(result, true);
	BOOST_CHECK_EQUAL("./test_input/ttsIndex2.json", configurationFile.getSpeechSynthesis().indexFilePath);
	BOOST_CHECK_EQUAL(1234, configurationFile.getSpeechSynthesis().ignoreOlderThan);
	BOOST_CHECK_EQUAL(SPEECH_POLISH, configurationFile.getSpeechSynthesis().language);

}

BOOST_AUTO_TEST_CASE(airspace_aroundpoint_and_fixed)
{
	ConfigurationFile configurationFile("./test_input/configuration_airspace_aroundpoint_and_fixed.conf");

	bool result = configurationFile.parse();
	const ConfigurationFile_Airspace& airspace = configurationFile.getAirspace();

	BOOST_CHECK_EQUAL(result, true);
	BOOST_CHECK_EQUAL(2, airspace.aroundPoint.size());
	BOOST_CHECK_EQUAL(2, airspace.fixed.size());
	BOOST_CHECK_EQUAL(true, airspace.bailoutIfNothingToSay);

	const ConfigurationFile_Airspace_AroundPoint & around1st = airspace.aroundPoint.at(0);
	const ConfigurationFile_Airspace_AroundPoint & around2nd = airspace.aroundPoint.at(1);

	const ConfigurationFile_Airspace_Fixed & fixed1st = airspace.fixed.at(0);
	const ConfigurationFile_Airspace_Fixed & fixed2nd = airspace.fixed.at(1);

	BOOST_CHECK_EQUAL("deadbeef.mp3", around1st.audioFilename);
	BOOST_CHECK_EQUAL(19.1234f, around1st.latitude);
	BOOST_CHECK_EQUAL(49.876f, around1st.longitude);
	BOOST_CHECK_EQUAL(12345, around1st.radius);

	BOOST_CHECK_EQUAL("nocoffe.mp3", around2nd.audioFilename);
	BOOST_CHECK_EQUAL(19.4321f, around2nd.latitude);
	BOOST_CHECK_EQUAL(49.8888f, around2nd.longitude);
	BOOST_CHECK_EQUAL(54321, around2nd.radius);

	BOOST_CHECK_EQUAL("EPTS6A", fixed1st.designator);
	BOOST_CHECK_EQUAL(false, fixed1st.sayAltitudes);

	BOOST_CHECK_EQUAL("ATZ EPBA", fixed2nd.designator);
	BOOST_CHECK_EQUAL(true, fixed2nd.sayAltitudes);

	// defaults not defined in test configuration file
	BOOST_CHECK_EQUAL(0, airspace.reservationFutureTimeMargin);
	BOOST_CHECK_EQUAL(false, airspace.sayPast);
	BOOST_CHECK_EQUAL(false, airspace.sayAltitudes);
	BOOST_CHECK_EQUAL(true, airspace.genericAnouncementFromRegex);

	BOOST_CHECK_EQUAL(true, airspace.confPerElemType.sayTRA);
	BOOST_CHECK_EQUAL(true, airspace.confPerElemType.sayTSA);
	BOOST_CHECK_EQUAL(true, airspace.confPerElemType.sayATZ);
	BOOST_CHECK_EQUAL(true, airspace.confPerElemType.sayD);
	BOOST_CHECK_EQUAL(true, airspace.confPerElemType.sayR);

	BOOST_CHECK_EQUAL("\\s[A-Z]{4}", airspace.confPerElemType.atzDesignatorRegexp);
	BOOST_CHECK_EQUAL("[A-Z]{1,2}[0-9]{1,3}", airspace.confPerElemType.traDesignatorRegexp);
	BOOST_CHECK_EQUAL("[A-Z]{1}$", airspace.confPerElemType.traSectorRegexp);
	BOOST_CHECK_EQUAL("[A-Z]{1,2}[0-9]{1,3}", airspace.confPerElemType.tsaDesignatorRegexp);
	BOOST_CHECK_EQUAL("[A-Z]{1}$", airspace.confPerElemType.tsaSectorRegexp);
	BOOST_CHECK_EQUAL("[A-Z][0-9]{1,3}", airspace.confPerElemType.dDesignatorRegexp);
	BOOST_CHECK_EQUAL("[A-Z]$", airspace.confPerElemType.dSectorRegexp);
	BOOST_CHECK_EQUAL("[A-Z][0-9]{1,3}$", airspace.confPerElemType.rDesignatorRegexp);

}

BOOST_AUTO_TEST_CASE(configuration_airspace_aroundpoint_and_fixed_genericairspace_false)
{
	ConfigurationFile configurationFile("./test_input/configuration_airspace_aroundpoint_and_fixed_genericanouncementfromregex_false.conf");

	bool result = configurationFile.parse();
	const ConfigurationFile_Airspace& airspace = configurationFile.getAirspace();

	BOOST_CHECK_EQUAL(result, true);
	BOOST_CHECK_EQUAL(2, airspace.aroundPoint.size());
	BOOST_CHECK_EQUAL(2, airspace.fixed.size());

	const ConfigurationFile_Airspace_AroundPoint & around1st = airspace.aroundPoint.at(0);
	const ConfigurationFile_Airspace_AroundPoint & around2nd = airspace.aroundPoint.at(1);

	const ConfigurationFile_Airspace_Fixed & fixed1st = airspace.fixed.at(0);
	const ConfigurationFile_Airspace_Fixed & fixed2nd = airspace.fixed.at(1);

	BOOST_CHECK_EQUAL("beef.mp3", around1st.audioFilename);
	BOOST_CHECK_EQUAL(19.1234f, around1st.latitude);
	BOOST_CHECK_EQUAL(49.876f, around1st.longitude);
	BOOST_CHECK_EQUAL(12345, around1st.radius);

	BOOST_CHECK_EQUAL("coffe.mp3", around2nd.audioFilename);
	BOOST_CHECK_EQUAL(19.4321f, around2nd.latitude);
	BOOST_CHECK_EQUAL(49.8888f, around2nd.longitude);
	BOOST_CHECK_EQUAL(54321, around2nd.radius);

	BOOST_CHECK_EQUAL("EPTS6A", fixed1st.designator);
	BOOST_CHECK_EQUAL(false, fixed1st.sayAltitudes);

	BOOST_CHECK_EQUAL("ATZ EPBA", fixed2nd.designator);
	BOOST_CHECK_EQUAL(true, fixed2nd.sayAltitudes);

	// defaults not defined in test configuration file
	BOOST_CHECK_EQUAL(0, airspace.reservationFutureTimeMargin);
	BOOST_CHECK_EQUAL(true, airspace.sayPast);
	BOOST_CHECK_EQUAL(true, airspace.sayAltitudes);
	BOOST_CHECK_EQUAL(false, airspace.genericAnouncementFromRegex);

	BOOST_CHECK_EQUAL(true, airspace.confPerElemType.sayTRA);
	BOOST_CHECK_EQUAL(false, airspace.confPerElemType.sayTSA);
	BOOST_CHECK_EQUAL(true, airspace.confPerElemType.sayATZ);
	BOOST_CHECK_EQUAL(false, airspace.confPerElemType.sayD);
	BOOST_CHECK_EQUAL(false, airspace.confPerElemType.sayR);

	BOOST_CHECK_EQUAL("\\s[A-Z]{4}", airspace.confPerElemType.atzDesignatorRegexp);
	BOOST_CHECK_EQUAL("[A-Z]{1,2}[0-9]{1,3}", airspace.confPerElemType.traDesignatorRegexp);
	BOOST_CHECK_EQUAL("[A-Z]{1}$", airspace.confPerElemType.traSectorRegexp);
	BOOST_CHECK_EQUAL("[A-Z]{1,2}[0-9]{1,3}", airspace.confPerElemType.tsaDesignatorRegexp);
	BOOST_CHECK_EQUAL("[A-Z]{1}$", airspace.confPerElemType.tsaSectorRegexp);
	BOOST_CHECK_EQUAL("[A-Z][0-9]{1,3}", airspace.confPerElemType.dDesignatorRegexp);
	BOOST_CHECK_EQUAL("[A-Z]$", airspace.confPerElemType.dSectorRegexp);
	BOOST_CHECK_EQUAL("[A-Z][0-9]{1,3}$", airspace.confPerElemType.rDesignatorRegexp);

}

// configuration_airspace_custom_regexp
BOOST_AUTO_TEST_CASE(configuration_airspace_custom_regexp)
{
	ConfigurationFile configurationFile("./test_input/configuration_airspace_custom_regexp.conf");

	bool result = configurationFile.parse();
	const ConfigurationFile_Airspace& airspace = configurationFile.getAirspace();

	// defaults not defined in test configuration file
	BOOST_CHECK_EQUAL(600, airspace.reservationFutureTimeMargin);
	BOOST_CHECK_EQUAL(true, airspace.sayPast);
	BOOST_CHECK_EQUAL(true, airspace.sayAltitudes);
	BOOST_CHECK_EQUAL(true, airspace.genericAnouncementFromRegex);

	BOOST_CHECK_EQUAL(false, airspace.confPerElemType.sayTRA);
	BOOST_CHECK_EQUAL(false, airspace.confPerElemType.sayTSA);
	BOOST_CHECK_EQUAL(false, airspace.confPerElemType.sayATZ);
	BOOST_CHECK_EQUAL(false, airspace.confPerElemType.sayD);
	BOOST_CHECK_EQUAL(false, airspace.confPerElemType.sayR);

	BOOST_CHECK_EQUAL(".{1..3}", airspace.confPerElemType.atzDesignatorRegexp);
	BOOST_CHECK_EQUAL(".{2..4}", airspace.confPerElemType.traDesignatorRegexp);
	BOOST_CHECK_EQUAL(".{3..5}", airspace.confPerElemType.traSectorRegexp);
	BOOST_CHECK_EQUAL(".{4..6}", airspace.confPerElemType.tsaDesignatorRegexp);
	BOOST_CHECK_EQUAL(".{5..7}", airspace.confPerElemType.tsaSectorRegexp);
	BOOST_CHECK_EQUAL(".{6..8}", airspace.confPerElemType.dDesignatorRegexp);
	BOOST_CHECK_EQUAL(".{7..9}", airspace.confPerElemType.dSectorRegexp);
	BOOST_CHECK_EQUAL(".{8..9}", airspace.confPerElemType.rDesignatorRegexp);

}

BOOST_AUTO_TEST_CASE(configuration_airspace_dictionary)
{
	ConfigurationFile configurationFile("./test_input/configuration_airspace_dictionary.conf");

	bool result = configurationFile.parse();
	const ConfigurationFile_Airspace& airspace = configurationFile.getAirspace();

	BOOST_CHECK_EQUAL(result, true);
	BOOST_CHECK_EQUAL(2, airspace.aroundPoint.size());
	BOOST_CHECK_EQUAL(2, airspace.fixed.size());

	BOOST_CHECK_EQUAL(3, airspace.airspaceDesignatorsAnouncement.size());

	auto first = airspace.airspaceDesignatorsAnouncement.find("ATZ EPBA");
	auto second = airspace.airspaceDesignatorsAnouncement.find("EPTR3");

	BOOST_CHECK(first != airspace.airspaceDesignatorsAnouncement.end());
	BOOST_CHECK(second != airspace.airspaceDesignatorsAnouncement.end());

	BOOST_CHECK_EQUAL("ATZ EPBA", first->first);
	BOOST_CHECK_EQUAL("bielsko_biala.mp3", first->second);

	BOOST_CHECK_EQUAL("EPTR3", second->first);
	BOOST_CHECK_EQUAL("costam.mp3", second->second);

}
// configuration_airspace_dictionary
