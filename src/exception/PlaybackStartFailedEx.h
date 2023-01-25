/*
 * PlaybackStartFailedEx.h
 *
 *  Created on: Jan 25, 2023
 *      Author: mateusz
 */

#ifndef EXCEPTION_PLAYBACKSTARTFAILEDEX_H_
#define EXCEPTION_PLAYBACKSTARTFAILEDEX_H_

#include <exception>

class PlaybackStartFailedEx : public std::exception {
public:
	const virtual char* what() const noexcept;
};

inline const char* PlaybackStartFailedEx::what() const noexcept {

	return "PlaybackStartFailedEx";
}

#endif /* EXCEPTION_PLAYBACKSTARTFAILEDEX_H_ */
