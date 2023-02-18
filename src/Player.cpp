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

Player::Player() {

    /* Load the VLC engine */
    inst = libvlc_new (0, NULL);

    mp = libvlc_media_player_new(inst);

    evm = libvlc_media_player_event_manager(mp);

    m = NULL;

    SPDLOG_INFO("Initializing player");

    // set a callback called after each audio file ends (something like Auto-Cue)
    libvlc_event_attach(evm, libvlc_MediaPlayerStopped, &Player::plainStaticCallback, static_cast<void*>(this));

#ifdef NDEBUG

#else

#endif

}

Player::~Player() {
    /* Free the media_player */
    libvlc_media_player_release (mp);

    libvlc_release (inst);

    SPDLOG_INFO("Deinitializing player");
}

void Player::playbackStoppedCallback() {

	// release current media
	libvlc_media_release(m);

	// notify everybody that playback is done
	conditionVariable.notify_all();

    if (currentFile == currentPlaylist->end()) {
    	SPDLOG_INFO("Last file has been played");
    }

}

void Player::plainStaticCallback(const struct libvlc_event_t *p_event,
		void *p_data) {

	Player * ptr = static_cast<Player *> (p_data);

	ptr->playbackStoppedCallback();
}

void Player::setPlaylist(std::shared_ptr<std::vector<std::string> > playlist, const std::string & prefix) {

    if (playlist) {
    	filePrefix = prefix;

    	if (playlist->size() > 0) {
    		currentPlaylist = playlist;

    		// get an interator to first element of playlist
    		currentFile = playlist->begin();
    	}
    }
}

bool Player::playNext() {

    if (currentFile == currentPlaylist->end()) {
    	return false;
    }

    std::string fn;

    if (filePrefix.length() > 1) {
    	fn = filePrefix + *currentFile;
    }
    else {
    	fn = *currentFile;
    }

	SPDLOG_INFO("Playling file file {}", fn.c_str());

    // create a new item
    m = libvlc_media_new_path(inst, fn.c_str());

    libvlc_media_player_set_media(mp, m);

    // play the media_player
    const int playback_result = libvlc_media_player_play(mp);

    // check result
    if (playback_result != 0) {
    	throw PlaybackStartFailedEx();
	}

    currentFile++;

    return true;

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
