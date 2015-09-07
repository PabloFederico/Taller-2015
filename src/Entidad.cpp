/*
 * Entidad.cpp
 *
 *  Created on: 2 de sept. de 2015
 *      Author: pgfederi
 */

#include "Entidad.h"

Entidad::Entidad(TipoEntidad tipo) {
	this->tipo = tipo;
	this->path = "";
}

void Entidad::setPath(std::string path){
	this->path = path;
}

TipoEntidad Entidad::getTipo(){
	return this->tipo;
}

std::string Entidad::getPath(){
	return this->path;
}

Entidad::~Entidad() {
	// TODO Auto-generated destructor stub
}

