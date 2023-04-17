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
public:

    virtual const char* what() const throw() {
		return "ForecastInCacheTooOldEx";
    }
};

#endif /* EXCEPTION_FORECASTINCACHETOOOLDEX_H_ */
