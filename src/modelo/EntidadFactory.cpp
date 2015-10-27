/*
 * EntidadFactory.cpp
 *
 *  Created on: Oct 16, 2015
 *      Author: martin
 */

#include "EntidadFactory.h"


EntidadFactory::EntidadFactory(int num_jug, vector<InfoEntidad> v): idJug(num_jug), vInfoEntidades(v) {};


Entidad* EntidadFactory::nuevaEntidad(TipoEntidad tipo) {
	Entidad *e;
	if (tipo == SOLDADO || tipo == JUANA_DE_ARCO) //hardcodeado
		e = new Entidad(tipo, idJug);
	else
		e = new Entidad(tipo, 0);

	std::vector<InfoEntidad>::iterator it = std::find(this->vInfoEntidades.begin(), this->vInfoEntidades.end(), tipo);

	if (it != this->vInfoEntidades.end()) {
		e->setTam(it->ancho, it->alto);
	}
	return e;
}


EntidadFactory::~EntidadFactory() {
	// TODO Auto-generated destructor stub
}
