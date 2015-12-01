/*
 * EntidadFactory.cpp
 *
 *  Created on: Oct 16, 2015
 *      Author: martin
 */

#include "EntidadFactory.h"
#include "../modelo/Unidad.h"
#include "../modelo/Edificio.h"


EntidadFactory::EntidadFactory(vector<InfoEntidad> v): vInfoEntidades(v) {};

Entidad* EntidadFactory::nuevaEntidad(TipoEntidad tipo, int id_jug, int dni) {
	Entidad *e;
	switch (tipo){

	case ALDEANO:
	case ARQUERO:
	case SOLDADO:
			e = new Unidad(tipo,id_jug,dni);
			break;
	case CONSTRUCCION_BARRACK:
		return nuevaConstruccionDe(BARRACK,id_jug,dni);
	case CONSTRUCCION_CUARTEL:
		return nuevaConstruccionDe(CUARTEL,id_jug,dni);
	case CONSTRUCCION:
		return nuevaConstruccionDe(CONSTRUCCION,id_jug,dni);
	case BARRACK:
	case CASTILLO:
	case CENTRO_CIVICO:
	case CUARTEL:
	case EDIFICIO:
			e = new Edificio(tipo,id_jug,dni);
			break;
	default :
			e = new Entidad(tipo,id_jug,dni);
			break;

	}

	std::vector<InfoEntidad>::iterator it = std::find(this->vInfoEntidades.begin(), this->vInfoEntidades.end(), tipo);
	if (it != this->vInfoEntidades.end()) {
		e->setTam(it->ancho, it->alto);
	}

	return e;
}

// Solo mandar los tipos construibles, BARRACA y CUARTEL.
Construccion* EntidadFactory::nuevaConstruccionDe(TipoEntidad tipoAConstruir, int id_jug, int dni) {
	Construccion *c = new Construccion(tipoAConstruir,id_jug,dni);

	std::vector<InfoEntidad>::iterator it = std::find(this->vInfoEntidades.begin(), this->vInfoEntidades.end(), TipoConstruccion(tipoAConstruir));
	if (it != this->vInfoEntidades.end()) {
		c->setTam(it->ancho, it->alto);
	}
	return c;
}

EntidadFactory::~EntidadFactory() {
	this->vInfoEntidades.clear();
}
