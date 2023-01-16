/*
 * AudioFileNotFoundEx.h
 *
 *  Created on: Jan 15, 2023
 *      Author: mateusz
 */

#ifndef AUDIOFILENOTFOUNDEX_H_
#define AUDIOFILENOTFOUNDEX_H_

#include <exception>

class AudioFileNotFoundEx {
public:
    virtual const char* what() const throw() {
		return "AudioFileNotFoundEx";
    }
};

#endif /* AUDIOFILENOTFOUNDEX_H_ */
