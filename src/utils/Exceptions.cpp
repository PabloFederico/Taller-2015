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

EntidadMurio::EntidadMurio() {}

ConstruccionTermino::ConstruccionTermino(TipoEntidad tipoEdif, int dni, int idJug, int x, int y, int vidaRestante):
		tipoEdif(tipoEdif), dni(dni), idJug(idJug), x(x), y(y), vidaRestante(vidaRestante) {};

Recoleccion::Recoleccion(TipoEntidad tipo, int cant): tipo(tipo), cant(cant) {}

DestinoEncontrado::DestinoEncontrado() {}

NoTieneRecurso::NoTieneRecurso() {}
FueraDeEscenario::FueraDeEscenario() {}

TileEstaOcupado::TileEstaOcupado() {}

CaminoVacio::CaminoVacio() {}
PasoCompletado::PasoCompletado(int ID): id(ID) {}

NoSeRecibio::NoSeRecibio() {}

ConnectionProblem::ConnectionProblem() {}

Disconnected::Disconnected() {}

NoExiste::NoExiste() {}

