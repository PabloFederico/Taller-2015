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
	switch (tipo) {
		case SOLDADO:
		case JUANA_DE_ARCO:
		case ANIMAL:
					movible = true;
					break;
		default :	movible = false;
				  	break;
	}

}

//std::string Entidad::getTipo(){
TipoEntidad Entidad::getTipo(){
	return this->tipo;
}

bool Entidad::esMovible(){
	return this->movible;
}

Entidad::~Entidad() {
	// TODO Auto-generated destructor stub
}

