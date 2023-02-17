/*
 * Player.h
 *
 * Class which wraps libVLC interface and plays set of audio files created
 *
 *  Created on: Jan 24, 2023
 *      Author: mateusz
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include <vector>
#include <memory>
#include <string>
#include <functional>
#include <condition_variable>

#include "ConfigurationFile.h"

#include <vlc/vlc.h>

#define PLAYER_PLAYBACK_TIMEOUT_SECONDS		180

class Player {

    libvlc_instance_t * inst;

    libvlc_media_player_t *mp;

    libvlc_event_manager_t *evm;

    libvlc_media_t *m;

    std::condition_variable conditionVariable;

    std::mutex mutex;

    std::vector<std::string>::iterator currentFile;

    std::shared_ptr<std::vector<std::string> > currentPlaylist;

    void playbackStoppedCallback();

    static void plainStaticCallback(const struct libvlc_event_t *p_event, void *p_data);


public:
	Player();
	virtual ~Player();

	/**
	 * Set a playlist and reset an internal state of the player
	 */
	void setPlaylist(std::shared_ptr<std::vector<std::string>> playlist);

	/**
	 * starts playing given playlist or switch to next file
	 */
	bool playNext();

	/**
	 * Synchronously wait for playback to finish
	 */
	void waitForPlaybackToFinish();
};

#endif /* PLAYER_H_ */
