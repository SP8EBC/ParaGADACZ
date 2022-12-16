/*
 * main.cpp
 *
 *  Created on: Nov 28, 2022
 *      Author: mateusz
 */

#include "main.h"
#include <unistd.h>
#include <cstdint>
#include <iostream>
#include <vlc/vlc.h>
#include <condition_variable>
#include <list>
#include <memory>

// api
#include "ApiConfiguration.h"
#include "ApiClient.h"
#include "ListOfAllStationsApi.h"

std::condition_variable conditionVariable;

std::mutex mutex;

void callback (const struct libvlc_event_t *p_event, void *p_data) {
	std::cout << "dupa" << std::endl;
	conditionVariable.notify_all();
	return;
}

int main() {

	std::shared_ptr<org::openapitools::client::api::ApiClient> apiClient = std::make_shared<org::openapitools::client::api::ApiClient>();
	org::openapitools::client::api::ApiConfiguration apiConfiguration;
	apiConfiguration.setBaseUrl("http://pogoda.cc:8080/meteo_backend_web/");

	apiClient->setConfiguration(std::shared_ptr<org::openapitools::client::api::ApiConfiguration>(&apiConfiguration));

	org::openapitools::client::api::ListOfAllStationsApi listofAllStation(apiClient);

	std::list<std::string> playlist;

	std::unique_lock<std::mutex> lock(mutex);

	int vlc_result = 0;

    libvlc_instance_t * inst;
    libvlc_media_player_t *mp;
    libvlc_media_t *m;
    libvlc_event_manager_t *evm;

    auto type = listofAllStation.listOfAllStationsGet().get();

    playlist.emplace_back("chirp.mp3");
    playlist.emplace_back("11-taniec-krotki-silent.mp3");

    /* Load the VLC engine */
    inst = libvlc_new (0, NULL);

    // create a media play playing environment
    mp = libvlc_media_player_new(inst); //libvlc_media_player_new_from_media(m);

    evm = libvlc_media_player_event_manager(mp);

    vlc_result = libvlc_event_attach(evm, libvlc_MediaPlayerStopped, callback, 0);

    for (std::string file : playlist) {
        // create a new item
        m = libvlc_media_new_path(inst, file.c_str());

        libvlc_media_player_set_media(mp, m);

        // play the media_player
        libvlc_media_player_play(mp);

        conditionVariable.wait_for(lock, std::chrono::seconds(33));

        libvlc_media_release(m);
    }


    //sleep(11);

    // stop playing
//    libvlc_media_player_stop(mp);

    /* Free the media_player */
    libvlc_media_player_release (mp);

    libvlc_release (inst);

	return 0;
}



