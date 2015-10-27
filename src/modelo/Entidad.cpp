/*
 * Entidad.cpp
 *
 *  Created on: 2 de sept. de 2015
 *      Author: pgfederi
 */

#include "../modelo/Entidad.h"

Entidad::Entidad(TipoEntidad tipo, int num_jug): idJug(num_jug)  {
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

TipoEntidad Entidad::getTipo(){
	return this->tipo;
}

int Entidad::getIDJug() {
	return this->idJug;
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

std::string Entidad::enc() {
	ostringstream enc;
	enc << idJug<<","<<tipo<<","<<ancho<<","<<alto;
	return enc.str();
}

Entidad Entidad::dec(std::string s) {
	int id,ti,an,al;
	stringstream ss(s);
	ss >> id; ss.ignore();
	ss >> ti; ss.ignore();
	ss >> an; ss.ignore();
	ss >> al;
	Entidad e = Entidad(TipoEntidad(ti), id);
	e.setTam(an, al);
	return e;
}

Entidad::~Entidad() {
	// TODO Auto-generated destructor stub
}

