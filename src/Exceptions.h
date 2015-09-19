/*
 * Exceptions.h
 *
 *  Created on: Sep 19, 2015
 *      Author: martin
 */

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <exception>
#include <stdexcept>

class bad_nombreTipoEntidad: public std::invalid_argument
{
public:
	bad_nombreTipoEntidad(const std::string& message);
	const char* what() const throw();
};

#endif /* EXCEPTIONS_H_ */
