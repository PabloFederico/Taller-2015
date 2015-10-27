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
			movible = false;
			ocupador = false;
			info = "";
			break;
		case TIERRA:
				movible = false;
				ocupador = false;
				info = "";
				break;
		case AGUA:
				movible = false;
				ocupador = true;
				info = "Agua";
				break;
		case SOLDADO:
			movible = true;
			ocupador = true;
			info = "Soldado";
			break;
		case JUANA_DE_ARCO:
			movible = true;
			ocupador = true;
			info = "Juana De Arco";
			break;
		case ANIMAL:
				movible = true;
				ocupador = true;
				info = "Animal";
				break;
		case CASTILLO:
				movible = false;
				ocupador = true;
				info = "Castillo";
				break;
		case ARBOL:
				movible = false;
				ocupador = true;
				info = "Arbol";
				break;
		default:
				movible = false;
				ocupador = true;
				info = "";
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

std::string Entidad::getInfo(){
	return info;
}

Entidad::~Entidad() {
	// TODO Auto-generated destructor stub
}

