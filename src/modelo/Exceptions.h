/*
 * Exceptions.h
 *
 *  Created on: Sep 19, 2015
 *      Author: martin
 */

#ifndef MODELO_EXCEPTIONS_H_
#define MODELO_EXCEPTIONS_H_

#include <exception>
#include <stdexcept>

class bad_nombreTipoEntidad: public std::invalid_argument
{
public:
	bad_nombreTipoEntidad(const std::string& message);
	const char* what() const throw();
};

class DestinoEncontrado: public std::exception
{
public:
	DestinoEncontrado();
};

class FueraDeEscenario: public std::exception
{
public:
	FueraDeEscenario();
};

#endif /* MODELO_EXCEPTIONS_H_ */
