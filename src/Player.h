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


class Player {

	ConfigurationFile & config;

    libvlc_instance_t * inst;

    libvlc_media_player_t *mp;

    libvlc_event_manager_t *evm;

    std::condition_variable conditionVariable;

    std::mutex mutex;

    void playbackStoppedCallback();

    static void plainStaticCallback(const struct libvlc_event_t *p_event, void *p_data);


public:
	Player(ConfigurationFile & configurationFile);
	virtual ~Player();
	Player(const Player &other);
	Player(Player &&other);
	Player& operator=(const Player &other);
	Player& operator=(Player &&other);

	/**
	 * starts playing given playlist
	 */
	void play(std::shared_ptr<std::vector<std::string>> playlist);

	/**
	 * Synchronously wait for playback to finish
	 */
	void waitForPlaybackToFinish();
};

#endif /* PLAYER_H_ */
