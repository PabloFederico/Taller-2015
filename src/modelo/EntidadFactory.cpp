/*
 * EntidadFactory.cpp
 *
 *  Created on: Oct 16, 2015
 *      Author: martin
 */

#include "EntidadFactory.h"
#include "../modelo/Unidad.h"
#include "../modelo/Edificio.h"


EntidadFactory::EntidadFactory(int num_jug, vector<InfoEntidad> v): idJug(num_jug), vInfoEntidades(v) {};


Entidad* EntidadFactory::nuevaEntidad(TipoEntidad tipo) {
	Entidad *e;
	switch (tipo){
	case SOLDADO:
	case ALDEANO:
				e = new Unidad(tipo,idJug);
				break;
	case EDIFICIO:
				e = new Edificio(tipo,idJug);
				break;
	default :
				e = new Entidad(tipo,0);
				break;
	}

	std::vector<InfoEntidad>::iterator it = std::find(this->vInfoEntidades.begin(), this->vInfoEntidades.end(), tipo);
	if (it != this->vInfoEntidades.end()) {
		e->setTam(it->ancho, it->alto);
	}
	return e;
}

int EntidadFactory::getId_Jug(){
	return idJug;
}


EntidadFactory::~EntidadFactory() {
	// TODO Auto-generated destructor stub
}
