/*
 * AvalancheWarnings.cpp
 *
 *  Created on: Feb 26, 2023
 *      Author: mateusz
 */

#include "AvalancheWarnings.h"

#include "exception/HtmlParseError.h"
#include "exception/HtmlNoDataEx.h"

#include <myhtml/api.h>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/bin_to_hex.h"

#define MINIMUM_VALID_HTML_LENGHT	100

mystatus_t serialization_callback(const char* data, size_t len, void* ctx)
{
	std::string _data(data, len);
	SPDLOG_DEBUG("data: {}", _data);
    //printf("%.*s", (int)len, data);
    return MyCORE_STATUS_OK;
}

AvalancheWarnings::AvalancheWarnings() {

}

void AvalancheWarnings::parse() {

    const char* attr_key = "class";
    const char* attr_value = "rate-icon";

	// check if a response is senseful
	if (httpResponse.size() < MINIMUM_VALID_HTML_LENGHT) {
		throw HtmlNoDataEx();
	}

    // basic init
    myhtml_t* myhtml = myhtml_create();
    myhtml_init(myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0);

    // init tree
    myhtml_tree_t* tree = myhtml_tree_create();
    myhtml_tree_init(tree, myhtml);

    // parse html
    mystatus_t result = myhtml_parse(tree, MyENCODING_UTF_8, this->httpResponse.c_str(), this->httpResponse.length());

	SPDLOG_INFO("html parsing result: 0x{0:x}", result);

    myhtml_collection_t* collection = NULL;
    myhtml_collection_t* collection2 = NULL;
    collection = myhtml_get_nodes_by_attribute_value_contain(	tree, NULL, NULL, true,
    															attr_key, strlen(attr_key),
																attr_value, strlen(attr_value), NULL);

    if(collection) {
    	SPDLOG_DEBUG("{} elements found", collection->length);

        for(size_t i = 0; i < collection->length; i++) {
        	myhtml_serialization_tree_callback(collection->list[i], serialization_callback, NULL);
        }

        myhtml_tree_node_t * child;
        myhtml_tree_node_t *node = collection->list[0];
        child = myhtml_node_child(node);
        myhtml_tree_t * _tree_of_child = myhtml_node_tree(child);

        if (_tree_of_child) {
            collection = myhtml_get_nodes_by_name(_tree_of_child, NULL, "img", 3, NULL);
        }

//        do {
//        	//child = myhtml_node_child(node);
//
////            if(tag_id == MyHTML_TAG__TEXT || tag_id == MyHTML_TAG__COMMENT) {
////                const char* node_text = myhtml_node_text(node, NULL);
////                SPDLOG_DEBUG(">: {}", node_text);
////            }
////            else {
////            	SPDLOG_DEBUG(">\n");
////            }
//
//            myhtml_serialization_node_callback(node, serialization_callback, NULL);
//
//            node = myhtml_node_next(node);
//        } while(node);
        //= myhtml_node_child(collection->list[0]);

        //std::string _content = std::string(static_cast<const char *>(child->data), child->);

    }
    else {
    	SPDLOG_ERROR("nothing found");
    }


    SPDLOG_INFO("done");

}

AvalancheWarnings::~AvalancheWarnings() {
}

void AvalancheWarnings::download(AvalancheWarnings_Location location) {
}

void AvalancheWarnings::parse(std::string &response) {
	httpResponse = std::string(std::move(response));
	parse();
}

int8_t AvalancheWarnings::getSeverityLevel() {

	int8_t output = -1;

	return output;

}
