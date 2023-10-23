/*
 * NoCommWithEmailServer.h
 *
 *  Created on: Oct 23, 2023
 *      Author: mateusz
 */

#ifndef EXCEPTION_NOCOMMWITHEMAILSERVEREX_H_
#define EXCEPTION_NOCOMMWITHEMAILSERVEREX_H_


#include <exception>

class NoCommWithEmailServerEx {
public:
    virtual const char* what() const throw() {
		return "NoCommWithEmailServerEx";
    }
};


#endif /* EXCEPTION_NOCOMMWITHEMAILSERVEREX_H_ */
