/*
 * SpeechSynthesisTimeout.h
 *
 *  Created on: Sep 30, 2023
 *      Author: mateusz
 */

#ifndef EXCEPTION_SPEECHSYNTHESISTIMEOUTEX_H_
#define EXCEPTION_SPEECHSYNTHESISTIMEOUTEX_H_


#include <exception>

class SpeechSynthesisTimeoutEx : public std::exception{

    virtual const char* what() const throw() {
		return "SpeechSynthesisTimeoutEx";
    }
};



#endif /* EXCEPTION_SPEECHSYNTHESISTIMEOUTEX_H_ */
