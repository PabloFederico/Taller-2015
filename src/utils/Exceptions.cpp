/*
 * Exceptions.cpp
 *
 *  Created on: Sep 19, 2015
 *      Author: martin
 */

#include "../utils/Exceptions.h"


bad_nombreTipoEntidad::bad_nombreTipoEntidad(const std::string& message): std::invalid_argument(message) {}

const char* bad_nombreTipoEntidad::what() const throw()
{
	return "Tipo de entidad mal especificado.";
}

DestinoEncontrado::DestinoEncontrado() {}

NoTieneRecurso::NoTieneRecurso() {}
FueraDeEscenario::FueraDeEscenario() {}

TileEstaOcupado::TileEstaOcupado() {}

CaminoVacio::CaminoVacio() {}

NoSeRecibio::NoSeRecibio() {}

ConnectionProblem::ConnectionProblem() {}

Disconnected::Disconnected() {}

