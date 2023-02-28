/*
 * UnknownLocationEx.h
 *
 *  Created on: Feb 27, 2023
 *      Author: mateusz
 */

#ifndef EXCEPTION_UNKNOWNLOCATIONEX_H_
#define EXCEPTION_UNKNOWNLOCATIONEX_H_


#include <exception>

class UnknownLocationEx : public std::exception {

    virtual const char* what() const throw() {
		return "UnknownLocationEx";
    }
};


#endif /* EXCEPTION_UNKNOWNLOCATIONEX_H_ */
