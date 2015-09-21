/*
 * Entidad.cpp
 *
 *  Created on: 2 de sept. de 2015
 *      Author: pgfederi
 */

#include "Entidad.h"

//Entidad::Entidad(std::string tipo) {
Entidad::Entidad(TipoEntidad tipo) {
	this->tipo = tipo;
}

//std::string Entidad::getTipo(){
TipoEntidad Entidad::getTipo(){
	return this->tipo;
}

Entidad::~Entidad() {
	// TODO Auto-generated destructor stub
}

