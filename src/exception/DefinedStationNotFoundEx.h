/*
 * DefinedStationNotFoundEx.h
 *
 *  Created on: Jan 13, 2023
 *      Author: mateusz
 */

#ifndef DEFINEDSTATIONNOTFOUNDEX_H_
#define DEFINEDSTATIONNOTFOUNDEX_H_

#include <exception>

class DefinedStationNotFoundEx : public std::exception{

    virtual const char* what() const throw() {
		return "WrongOrderEx";
    }
};

#endif /* DEFINEDSTATIONNOTFOUNDEX_H_ */
