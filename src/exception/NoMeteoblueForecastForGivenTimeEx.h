/*
 * NoMeteoblueForecastForGivenTimeEx.h
 *
 *  Created on: Apr 30, 2023
 *      Author: mateusz
 */

#ifndef EXCEPTION_NOMETEOBLUEFORECASTFORGIVENTIMEEX_H_
#define EXCEPTION_NOMETEOBLUEFORECASTFORGIVENTIMEEX_H_

#include <exception>
#include <string>

class NoMeteoblueForecastForGivenTimeEx : public std::exception {

	std::string type;
	int futureTime;

	std::string whatMessage;

public:

	NoMeteoblueForecastForGivenTimeEx(std::string _type, int _futureTime) {
		type = _type;
		futureTime = _futureTime;
		whatMessage = std::string("NoMeteoblueForecastForGivenTimeEx, ") +
				std::string("futureTime: ") + std::to_string(futureTime) +
				std::string("type: ") + type;
	}

    virtual const char* what() const throw() {
		return whatMessage.c_str();
    }
};

#endif /* EXCEPTION_NOMETEOBLUEFORECASTFORGIVENTIMEEX_H_ */
