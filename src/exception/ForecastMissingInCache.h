/*
 * ForecastMissingInCache.h
 *
 *  Created on: Apr 16, 2023
 *      Author: mateusz
 */

#ifndef EXCEPTION_FORECASTMISSINGINCACHE_H_
#define EXCEPTION_FORECASTMISSINGINCACHE_H_

#include <exception>

class ForecastMissingInCache  : public std::exception {
public:

    virtual const char* what() const throw() {
		return "ForecastMissingInCache";
    }
};

#endif /* EXCEPTION_FORECASTMISSINGINCACHE_H_ */
