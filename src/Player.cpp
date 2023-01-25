/*
 * Player.cpp
 *
 * Class which wraps libVLC interface and plays set of audio files created
 *
 *  Created on: Jan 24, 2023
 *      Author: mateusz
 */

#include "Player.h"

#include "exception/PlaybackStartFailedEx.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

Player::Player(ConfigurationFile & configurationFile) : config(configurationFile) {

    /* Load the VLC engine */
    inst = libvlc_new (0, NULL);

    mp = libvlc_media_player_new(inst);

    evm = NULL;

}

Player::~Player() {
    /* Free the media_player */
    libvlc_media_player_release (mp);

    libvlc_release (inst);
}

Player::Player(const Player &other)  : config(other.config) {
    evm = NULL;

}

Player::Player(Player &&other)  : config(other.config) {
    evm = NULL;

}

Player& Player::operator=(const Player &other) {

}

Player& Player::operator=(Player &&other) {

}

void Player::playbackStoppedCallback() {

	SPDLOG_DEBUG("dupa dupa");

}

void Player::plainStaticCallback(const struct libvlc_event_t *p_event,
		void *p_data) {

	Player * ptr = static_cast<Player *> (p_data);

	ptr->playbackStoppedCallback();
}

void Player::play(std::shared_ptr<std::vector<std::string> > playlist) {

    libvlc_media_t *m;

    evm = libvlc_media_player_event_manager(mp);

    const int result = libvlc_event_attach(evm, libvlc_MediaPlayerStopped, &Player::plainStaticCallback, static_cast<void*>(this));

    SPDLOG_DEBUG("Result of callback configuration {}", result);

    if (playlist) {
    	if (playlist->size() > 0) {
    		SPDLOG_INFO("Starting playback from file {}", playlist->at(0));

    	    // create a new item
    	    m = libvlc_media_new_path(inst, playlist->at(0).c_str());

    	    libvlc_media_player_set_media(mp, m);

    	    // play the media_player
    	    const int playback_result = libvlc_media_player_play(mp);

    	    // check result
    	    if (playback_result != 0) {
    	    	throw PlaybackStartFailedEx();
			}
		}
    }

}


/**
 * https://stackoverflow.com/questions/19808054/convert-c-function-pointer-to-c-function-pointer/19809787
 *
 *
 *template <typename T>
struct Callback;

template <typename Ret, typename... Params>
struct Callback<Ret(Params...)> {
    template <typename... Args>
    static Ret callback(Args... args) { return func(args...); }
    static std::function<Ret(Params...)> func;
};

// Initialize the static member.
template <typename Ret, typename... Params>
std::function<Ret(Params...)> Callback<Ret(Params...)>::func;
 */
