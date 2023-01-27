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

    m = NULL;

#ifdef NDEBUG

#else

#endif

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

	// rewind iterator to get next file
	currentFile++;

	// check if this was the last one
	if (currentFile == currentPlaylist->end()) {
		SPDLOG_INFO("Playlist done.");

		// notify everybody that playback is done
		conditionVariable.notify_all();
	}
	else {
		// release current media
        libvlc_media_release(m);

	    // create an item to next file
	    m = libvlc_media_new_path(inst, currentFile->c_str());

	    // set this media for playback
	    libvlc_media_player_set_media(mp, m);

	    // play it
	    const int playback_result = libvlc_media_player_play(mp);

	    // check result
	    if (playback_result != 0) {
	    	throw PlaybackStartFailedEx();
		}

	}

	SPDLOG_DEBUG("dupa dupa");

}

void Player::plainStaticCallback(const struct libvlc_event_t *p_event,
		void *p_data) {

	Player * ptr = static_cast<Player *> (p_data);

	ptr->playbackStoppedCallback();
}

void Player::play(std::shared_ptr<std::vector<std::string> > playlist) {

    evm = libvlc_media_player_event_manager(mp);

    // set a callback called after each audio file ends (something like Auto-Cue)
    const int result = libvlc_event_attach(evm, libvlc_MediaPlayerStopped, &Player::plainStaticCallback, static_cast<void*>(this));

    SPDLOG_DEBUG("Result of callback configuration {}", result);

    if (playlist) {
    	if (playlist->size() > 0) {
    		SPDLOG_INFO("Starting playback from file {}", playlist->at(0));

    		// get an interator to first element of playlist
    		currentFile = playlist->begin();

    		// store the playlist we are working with
    		currentPlaylist = playlist;

    	    // create a new item
    	    m = libvlc_media_new_path(inst, currentFile->c_str());

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

void Player::waitForPlaybackToFinish() {

	std::unique_lock<std::mutex> lock (mutex);

	std::cv_status result = conditionVariable.wait_for(lock, std::chrono::seconds(PLAYER_PLAYBACK_TIMEOUT_SECONDS));

	if (result == std::cv_status::timeout) {
		SPDLOG_ERROR("Timeout during playlist playback. {} seconds expired", PLAYER_PLAYBACK_TIMEOUT_SECONDS);
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
