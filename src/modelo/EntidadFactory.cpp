/*
 * EntidadFactory.cpp
 *
 *  Created on: Oct 16, 2015
 *      Author: martin
 */

#include "EntidadFactory.h"


EntidadFactory::EntidadFactory(vector<InfoEntidad> v): vInfoEntidades(v) {};


Entidad* EntidadFactory::nuevaEntidad(TipoEntidad tipo) {
	Entidad *e = new Entidad(tipo);

	std::vector<InfoEntidad>::iterator it = std::find(this->vInfoEntidades.begin(), this->vInfoEntidades.end(), tipo);

	if (it != this->vInfoEntidades.end()) {
		e->setTam(it->ancho, it->alto);
	}
	return e;
}


EntidadFactory::~EntidadFactory() {
	// TODO Auto-generated destructor stub
}
