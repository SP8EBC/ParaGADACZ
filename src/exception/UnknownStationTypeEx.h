/*
 * UnknownStationTypeEx.h
 *
 *  Created on: Jan 15, 2023
 *      Author: mateusz
 */

#ifndef UNKNOWNSTATIONTYPEEX_H_
#define UNKNOWNSTATIONTYPEEX_H_

#include <exception>

class UnknownStationTypeEx : public std::exception {

    virtual const char* what() const throw() {
		return "UnknownStationTypeEx";
    }
};

#endif /* UNKNOWNSTATIONTYPEEX_H_ */
