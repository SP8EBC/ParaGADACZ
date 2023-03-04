/*
 * AvalancheWarnings.cpp
 *
 *  Created on: Feb 26, 2023
 *      Author: mateusz
 */

#include "AvalancheWarnings.h"

#include "curl_code_to_str.h"

#include "exception/HtmlParseError.h"
#include "exception/HtmlNoDataEx.h"
#include "exception/UnknownLocationEx.h"

#include <string>

#include <myhtml/token.h>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/bin_to_hex.h"

#include "curl/curl.h"

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#define MINIMUM_VALID_HTML_LENGHT	100


AvalancheWarnings::AvalancheWarnings() {
	currentLevel = -1;
}

AvalancheWarnings::~AvalancheWarnings() {
}

void AvalancheWarnings::parseLevel() {

    const char* attr_key = "class";
    const char* attr_value = "rate-icon";

    const char* attr_key_alt = "alt";

	// check if a response is senseful
	if (httpResponse.size() < MINIMUM_VALID_HTML_LENGHT) {

		SPDLOG_ERROR("HTTP response looks invalid, lenght: {}", httpResponse.size());

		throw HtmlNoDataEx();
	}

	SPDLOG_INFO("Scraping severity level from downloaded response");

    // basic init
    myhtml_t* myhtml = myhtml_create();
    myhtml_init(myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0);

    // init tree
    myhtml_tree_t* tree = myhtml_tree_create();
    myhtml_tree_init(tree, myhtml);

    // parse html
    mystatus_t result = myhtml_parse(tree, MyENCODING_UTF_8, this->httpResponse.c_str(), this->httpResponse.length());

	this->currentLevel = -1;

    myhtml_collection_t* collection = NULL;

    /**
     *
     * <div class="rate-icon">
     *	<a href="https://www.gopr.pl/lawiny/baza-wiedzy/skala-zagrozenia#level-2" title="Zagrożenie lawinowe umiarkowane (drugi stopień)" style="display: block;">
	 *	  <img src="gopr_zarzad_babia_gora_files/2_icon_num.png" alt="2. Umiarkowany">
	 *	</a>
	 * </div>
     *
     */

    // look for certain point of HTML file downloaded from GOPR website
    collection = myhtml_get_nodes_by_attribute_value_contain(	tree, NULL, NULL, true,
    															attr_key, strlen(attr_key),
																attr_value, strlen(attr_value), NULL);

    if(collection) {
    	SPDLOG_DEBUG("{} elements found", collection->length);

    	// this is a pointer to '<div class="rate-icon">'
        myhtml_tree_node_t * div_node = collection->list[0];

        // this is a pointer to '<a href=  (...)'. for some reason MyHtml splits tabulation before a tag
        // and insert it as separate node in a document tree. '<a href=  (...)' tag is next node, next
        // child under div
        myhtml_tree_node_t * a_href_node =  myhtml_node_next(myhtml_node_child(div_node));

        // check if tag has been found
        if (a_href_node) {

        	// get a pointer to '<img src= (...)' tag which consist an url and alternative text to severity level itself
        	myhtml_tree_node_t * img_src_node = myhtml_node_next(myhtml_node_child(a_href_node));

        	// check if that has been found
        	if (img_src_node) {
        		// get token from this node - this consist all key-value pairs
        		myhtml_token_node_t * token = myhtml_node_token(img_src_node);

        		// get 'alt' attribute from this token
        		myhtml_token_attr_t * attribute = myhtml_token_attr_by_name(token, attr_key_alt, strlen(attr_key_alt));

        		if (attribute) {
        			mycore_string_t * value = &attribute->value;

        			std::string value_as_string = std::string(mycore_string_data(value), mycore_string_length(value));

        			// get current severity level for that location
        			this->currentLevel = AvalancheWarnings::parseSeverityLevelFromString(value_as_string);

        			SPDLOG_INFO("severity value scraped from HTTP response: {}", value_as_string);
        		}
        		else {
                	SPDLOG_ERROR("non existing attribute");
        		}

        	}
        	else {
            	SPDLOG_ERROR("a tag '<img src= (...)' with a severity level itself is missing");
        	}
        }
        else {
        	SPDLOG_ERROR("a tag '<a href=  (...)' with a link to severity level explanation is missing");
        }

    }
    else {
    	SPDLOG_ERROR("Cannot find avalanche severity level within HTTP data");
    }

    // release resources
    myhtml_tree_destroy(tree);
    myhtml_destroy(myhtml);
}

void AvalancheWarnings::parseDangerousExposition() {

	/**
	 * 	<div class="aspect">
	 *	<strong>Szczególnie niebezpieczna wystawa</strong><br>
	 *	<img src="gopr_zarzad_babia_gora_files/n-ne-e.png" alt="Szczególnie niebezpieczna wystawa: Pn.-Pn.Wsch.-Wsch."><br>
	 *	Pn.-Pn.Wsch.-Wsch.
	 *	</div>
	 *
	 *
	 */

    const char* attr_key = "alt";
    const char* attr_value = "niebezpieczna";

	// check if a response is senseful
	if (httpResponse.size() < MINIMUM_VALID_HTML_LENGHT) {
		throw HtmlNoDataEx();
	}

	SPDLOG_INFO("Scraping dangerous exposition from downloaded response");

    // basic init
    myhtml_t* myhtml = myhtml_create();
    myhtml_init(myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0);

    // init tree
    myhtml_tree_t* tree = myhtml_tree_create();
    myhtml_tree_init(tree, myhtml);

    // parse html
    mystatus_t result = myhtml_parse(tree, MyENCODING_UTF_8, this->httpResponse.c_str(), this->httpResponse.length());

	SPDLOG_INFO("HTML parsing result: 0x{0:x}", result);

    // look for certain point of HTML file downloaded from GOPR website
    myhtml_collection_t* collection = myhtml_get_nodes_by_attribute_value_contain(	tree, NULL, NULL, true,
    															attr_key, strlen(attr_key),
																attr_value, strlen(attr_value), NULL);

    // check if parsing was successful
    if (collection) {
		myhtml_token_node_t * token = myhtml_node_token(collection->list[0]);

		// get 'alt' attribute from this token
		myhtml_token_attr_t * attribute = myhtml_token_attr_by_name(token, attr_key, strlen(attr_key));

		if (attribute) {
			mycore_string_t * value = &attribute->value;

			std::string value_as_string = std::string(mycore_string_data(value), mycore_string_length(value));

			SPDLOG_INFO("exposition value scraped from HTTP response: {}", value_as_string);

			auto exposition = AvalancheWarnings::parseExpositionsFromString(value_as_string);

			currentExpositions = exposition;
		}
		else {
        	SPDLOG_ERROR("cannot find dangerous exposition in HTML data!!");
		}
    }
}

int AvalancheWarnings::download(AvalancheWarnings_Location location) {

	std::string url;

    std::array<char, 128> header_string;

	CURLcode result = CURLcode::CURLE_OBSOLETE50;

    char* effectiveUrl;
    long response_code;

    // check if valid location is provided
	switch (location) {
		case AvalancheWarnings_Location::BABIA_GORA:
			url = goprBabiaGora;
			break;
		default:
			SPDLOG_ERROR("unknown location selected");
			throw UnknownLocationEx();
	}

	SPDLOG_INFO("Downloading avalanche warning data from GOPR webpage, URL: {}", url);

	// initialize cURL
    auto curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());		// set URL to query
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);			// shutdown progress meter and do not call progress function cbk
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.42.0");	// UserAgent identification
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 5L);			// how many HTTP redirects are allowed before cURL will gave up
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);			// maximum HTTP transaction timeout in seconds
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 5000L);		// the same but in miliseconds ???? why doubled ??

    header_string.fill(0);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &AvalancheWarnings::staticWriteCallback);	// needs to be bind
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, header_string.data());

    // send HTTP request and wait for response
    result = curl_easy_perform(curl);

    // get all results
    double elapsed;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &elapsed);
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effectiveUrl);

    SPDLOG_INFO("CURL response_code: {}", boost::lexical_cast<std::string>(response_code));
    SPDLOG_INFO("CURL query elapsed time: {}", boost::lexical_cast<std::string>(elapsed));
    SPDLOG_INFO("CURL result: {}", curlCodeToStr(result));

    // deinitialize cURL
    curl_easy_cleanup(curl);
    curl = NULL;

    if (response_code == 200) {
    	httpResponse = std::move(responseBuffer);
    }

    return response_code;

}

int8_t AvalancheWarnings::parseSeverityLevelFromString(std::string &str) {

	int8_t out = -1;

	// trim input string from unwanted whitespaces (in place)
	boost::algorithm::trim(str);

	// extract first character
	char first = str.at(0);

	switch(first) {
	case '1' : out = 1; break;
	case '2' : out = 2; break;
	case '3' : out = 3; break;
	case '4' : out = 4; break;
	}

	SPDLOG_INFO("severity level found: {}", out);

	return out;
}

void AvalancheWarnings::writeCallback(char *data, size_t data_size) {
	this->responseBuffer.append(data, data_size);
}

int8_t AvalancheWarnings::getSeverityLevel() {

	int8_t output = currentLevel;

	return output;

}

AvalancheWarnings_Expositions AvalancheWarnings::parseExpositionsFromString(
		std::string &str) {

	/**
	 * Szczególnie niebezpieczna wystawa: Pn.-Pn.Wsch.-Wsch.
	 *
	 *  Pn.-Wsch.-Pd.
	 *
	 *   Zagrożenie jednolite
	 */

	AvalancheWarnings_Expositions out;

	// result of splitting a prefix (Szczegolnie niebezpieczna (...)) from the exposition information
	std::vector<std::string> first_split_result;

	// result of second splitting which separates expositions itself
	std::vector<std::string> expositions;

	// split the exposition information from the rest
	boost::algorithm::split(first_split_result, str, boost::is_any_of(":"));

	// output vector should conssit exactly two elements
	if (first_split_result.size() == 2) {
		boost::algorithm::split(expositions, first_split_result.at(1), boost::is_any_of("-"));

		// assumption is that there is at least one dangerous exposition provided in the announcement
		if (expositions.size() > 1) {
			SPDLOG_INFO("{} dangerous expositions found", expositions.size());

			for (std::string exposition : expositions) {

				// north-east
				if (exposition.find("Pn.Wsch.") != std::string::npos) {
					out.northEast = true;
				}
				// south-east
				else if (exposition.find("Pd.Wsch.") != std::string::npos) {
					out.southEast = true;
				}
				// north
				else if (exposition.find("Pn.") != std::string::npos) {
					out.north = true;
				}
				// east
				else if (exposition.find("Wsch.") != std::string::npos) {
					out.east = true;
				}
				// south
				else if (exposition.find("Pd.") != std::string::npos) {
					out.south = true;
				}
				// west
				else if (exposition.find("Zach.") != std::string::npos) {
					out.west = true;
				}

			}
		}
		else {
			; // there is no dangerous exposition at all or there is overall danger on all expositions
		}


	}

	return out;
}


void AvalancheWarnings::parseDangerousExposition(std::string &str) {
	httpResponse = std::string(std::move(str));
	parseDangerousExposition();

}

void AvalancheWarnings::parseLevel(std::string &response) {
	httpResponse = std::string(std::move(response));
	parseLevel();
}

size_t AvalancheWarnings::staticWriteCallback(char* get_data, size_t always_one,
		size_t get_data_size, void* userdata) {

	AvalancheWarnings* class_ptr = static_cast<AvalancheWarnings*>(userdata);

	class_ptr->writeCallback(get_data, get_data_size * always_one);

	return get_data_size * always_one;
}
