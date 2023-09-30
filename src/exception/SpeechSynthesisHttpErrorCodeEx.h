/*
 * SpeechSynthesisHttpErrorCode.h
 *
 *  Created on: Sep 30, 2023
 *      Author: mateusz
 */

#ifndef EXCEPTION_SPEECHSYNTHESISHTTPERRORCODEEX_H_
#define EXCEPTION_SPEECHSYNTHESISHTTPERRORCODEEX_H_


#include <exception>

class SpeechSynthesisHttpErrorCodeEx : public std::exception{

    virtual const char* what() const throw() {
		return "SpeechSynthesisHttpErrorCodeEx";
    }
};



#endif /* EXCEPTION_SPEECHSYNTHESISHTTPERRORCODEEX_H_ */
