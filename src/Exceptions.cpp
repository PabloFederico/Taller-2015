/*
 * Exceptions.cpp
 *
 *  Created on: Sep 19, 2015
 *      Author: martin
 */

#include "Exceptions.h"


bad_nombreTipoEntidad::bad_nombreTipoEntidad(const std::string& message): std::invalid_argument(message) {}

const char* bad_nombreTipoEntidad::what() const throw()
{
	return "Tipo de entidad mal especificado.";
}
