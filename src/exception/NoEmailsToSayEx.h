/*
 * NoEmailsToSayEx.h
 *
 *  Created on: Oct 23, 2023
 *      Author: mateusz
 */

#ifndef EXCEPTION_NOEMAILSTOSAYEX_H_
#define EXCEPTION_NOEMAILSTOSAYEX_H_


#include <exception>

class NoEmailsToSayEx {
public:
    virtual const char* what() const throw() {
		return "NoEmailsToSayEx";
    }
};


#endif /* EXCEPTION_NOEMAILSTOSAYEX_H_ */
