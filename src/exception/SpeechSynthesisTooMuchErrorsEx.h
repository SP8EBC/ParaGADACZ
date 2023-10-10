/*
 * SpeechSynthesisTooMuchErrorsEx.h
 *
 *  Created on: Sep 30, 2023
 *      Author: mateusz
 */

#ifndef EXCEPTION_SPEECHSYNTHESISTOOMUCHERRORSEX_H_
#define EXCEPTION_SPEECHSYNTHESISTOOMUCHERRORSEX_H_


#include <exception>

class SpeechSynthesisTooMuchErrorsEx : public std::exception{

    virtual const char* what() const throw() {
		return "SpeechSynthesisTooMuchErrorsEx";
    }
};



#endif /* EXCEPTION_SPEECHSYNTHESISTOOMUCHERRORSEX_H_ */
