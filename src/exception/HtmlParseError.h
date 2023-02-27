/*
 * HtmlParseError.h
 *
 *  Created on: Feb 26, 2023
 *      Author: mateusz
 */

#ifndef EXCEPTION_HTMLPARSEERROR_H_
#define EXCEPTION_HTMLPARSEERROR_H_

#include <exception>


class HtmlParseError : public std::exception {
public:

    virtual const char* what() const throw() {
		return "HtmlParseError";
    }
};

#endif /* EXCEPTION_HTMLPARSEERROR_H_ */
