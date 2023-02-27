/*
 * HtmlNoDataEx.h
 *
 *  Created on: Feb 26, 2023
 *      Author: mateusz
 */

#ifndef EXCEPTION_HTMLNODATAEX_H_
#define EXCEPTION_HTMLNODATAEX_H_

#include <exception>

class HtmlNoDataEx : public std::exception {
public:

    virtual const char* what() const throw() {
		return "HtmlNoDataEx";
    }
};

#endif /* EXCEPTION_HTMLNODATAEX_H_ */
