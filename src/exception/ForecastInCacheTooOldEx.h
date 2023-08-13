/*
 * ForecastInCacheTooOldEx.h
 *
 *  Created on: Apr 16, 2023
 *      Author: mateusz
 */

#ifndef EXCEPTION_FORECASTINCACHETOOOLDEX_H_
#define EXCEPTION_FORECASTINCACHETOOOLDEX_H_

#include <exception>

class ForecastInCacheTooOldEx   : public std::exception  {

	const std::string name;

	const int secondsOld;

public:

	ForecastInCacheTooOldEx(std::string _name, int _secondsOld) : secondsOld(_secondsOld), name(_name) {

	}

	const std::string message() {

		std::string out = "ForecastInCacheTooOldEx, name: " + name + ", " + std::to_string(secondsOld) + " seconds old";

		return out;
	}

    virtual const char* what() const throw() {

		return "ForecastInCacheTooOldEx";
    }
};

#endif /* EXCEPTION_FORECASTINCACHETOOOLDEX_H_ */
