/*
 * Entidad.cpp
 *
 *  Created on: 2 de sept. de 2015
 *      Author: pgfederi
 */

#include "../modelo/Entidad.h"

//Entidad::Entidad(std::string tipo) {
Entidad::Entidad(TipoEntidad tipo) {
	this->ancho = 1;
	this->alto = 1;
	this->tipo = tipo;
	switch (tipo) {
		case PASTO:
		case TIERRA:
				movible = false;
				ocupador = false;
				break;
		case AGUA:
				movible = false;
				ocupador = true;
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

void Entidad::setTam(int ancho, int alto) {
	this->ancho = ancho;
	this->alto  = alto;
}

pair<int,int> Entidad::getTam() {
	return std::pair<int,int>(this->ancho, this->alto);
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

