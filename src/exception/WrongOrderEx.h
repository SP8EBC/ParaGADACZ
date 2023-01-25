/*
 * WrongOrderEx.h
 *
 *  Created on: Jan 13, 2023
 *      Author: mateusz
 */

#ifndef WRONGORDEREX_H_
#define WRONGORDEREX_H_

#include <exception>

class WrongOrderEx: public std::exception {

    virtual const char* what() const throw() {
		return "WrongOrderEx";
    }
};

#endif /* WRONGORDEREX_H_ */
