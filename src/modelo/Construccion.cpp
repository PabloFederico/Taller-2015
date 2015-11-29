/*
 * Construccion.cpp
 *
 *  Created on: Nov 21, 2015
 *      Author: martin
 */

#include "Construccion.h"

Construccion::Construccion(TipoEntidad tipo, int id_jug, int identificador): Edificio(CONSTRUCCION,id_jug,identificador) {
	tipoEdif = tipo;
	this->progresoPorc = 0;
}

bool Construccion::esConstruccion() {
	return true;
}

TipoEntidad Construccion::getTipoEdificio(){
	return tipoEdif;
}

void Construccion::continuarConstruyendo() {
	this->progresoPorc += 20; //hardcodeo
	if (this->progresoPorc == 100) {
		throw ConstruccionTermino(this->tipoEdif,this->dni,this->getIDJug(),this->getPosicion().x,this->getPosicion().y,this->vidaRestante);
	}
}

std::string Construccion::getInfo() {
	ostringstream o;
	o << Edificio::getInfo()<<" en construcción";
	return o.str();
}

std::string Construccion::getVidaString() {
	ostringstream o;
	o << Entidad::getVidaString()<<" construido: "<<this->progresoPorc;
	return o.str();
}

Construccion::~Construccion() {}

