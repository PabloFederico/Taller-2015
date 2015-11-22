/*
 * Construccion.cpp
 *
 *  Created on: Nov 21, 2015
 *      Author: martin
 */

#include "Construccion.h"

Construccion::Construccion(TipoEntidad tipo, int id_jug):Edificio(CONSTRUCCION,id_jug) {
	tipoEdif = tipo;
	this->progresoPorc = 0;
}

bool Construccion::esConstruccion() {
	return true;
}

void Construccion::continuarConstruyendo() {
	this->progresoPorc += 25; //hardcodeo
	if (this->progresoPorc == 100) {
		throw ConstruccionTermino(this->tipoEdif,this->dni,this->idJug,this->getPosicion().x,this->getPosicion().y,this->vidaRestante);
	}
}

Construccion::~Construccion() {}

