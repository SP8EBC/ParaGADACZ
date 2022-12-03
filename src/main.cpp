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

std::condition_variable conditionVariable;

std::mutex mutex;

void callback (const struct libvlc_event_t *p_event, void *p_data) {
	std::cout << "dupa" << std::endl;
	conditionVariable.notify_all();
	return;
}

int main() {

	std::unique_lock<std::mutex> lock(mutex);

	int vlc_result = 0;

    libvlc_instance_t * inst;
    libvlc_media_player_t *mp;
    libvlc_media_t *m;
    libvlc_event_manager_t *evm;

    /* Load the VLC engine */
    inst = libvlc_new (0, NULL);

    // create a new item
    m = libvlc_media_new_path(inst, "chirp.mp3");

    // create a media play playing environment
    mp = libvlc_media_player_new_from_media(m);

    evm = libvlc_media_player_event_manager(mp);

    vlc_result = libvlc_event_attach(evm, libvlc_MediaPlayerStopped, callback, 0);

    // play the media_player
    libvlc_media_player_play(mp);

    conditionVariable.wait_for(lock, std::chrono::seconds(33));

    //sleep(11);

    // stop playing
//    libvlc_media_player_stop(mp);

    /* Free the media_player */
    libvlc_media_player_release (mp);

    libvlc_release (inst);

	return 0;
}



