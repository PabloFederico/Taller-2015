/*
 * Entidad.cpp
 *
 *  Created on: 2 de sept. de 2015
 *      Author: pgfederi
 */

#include "../modelo/Entidad.h"

//Entidad::Entidad(std::string tipo) {
Entidad::Entidad(TipoEntidad tipo) {
	this->tipo = tipo;
	switch (tipo) {
		case PASTO:
		case TIERRA:
		case AGUA:
				movible = false;
				ocupador = false;
				break;
		case SOLDADO:
		case JUANA_DE_ARCO:
		case ANIMAL:
				movible = true;
				ocupador = true;
				break;
		default:
				movible = false;
				ocupador = true;
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

bool Entidad::ocupaSuTile() {
	return this->ocupador;
}

Entidad::~Entidad() {
	// TODO Auto-generated destructor stub
}

