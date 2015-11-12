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
		case ALDEANO:
			movible = true;
			ocupador = true;
			info = "Aldeano";
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
		case ORO:
				movible = false;
				ocupador = false;
				info = "Oro";
				break;
		case COMIDA:
				movible = false;
				ocupador = false;
				info = "Comida";
				break;
		case MADERA:
				movible = false;
				ocupador = false;
				info = "Madera";
				break;
		default:
				movible = false;
				ocupador = true;
				info = "";
				break;
	}
	x = 0;
	y = 0;
}

int Entidad::getIDJug(){
	return idJug;
}

TipoEntidad Entidad::getTipo(){
	return this->tipo;
}

bool Entidad::esRecurso(){
	return (tipo == ORO || tipo == MADERA || tipo == COMIDA || tipo == PIEDRA);
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

Coordenada Entidad::getPosicion(){
	return Coordenada(x,y);
}

void Entidad::setPosicion(Coordenada nuevaCoord){
	x = nuevaCoord.x;
	y = nuevaCoord.y;
}


std::string Entidad::getInfo(){
	return info;
}

std::string Entidad::enc() {
	ostringstream enc;
	enc << idJug<<","<<tipo<<","<<ancho<<","<<alto;
	return enc.str();
}

Entidad* Entidad::dec(std::string s) {
	int id,ti,an,al;
	stringstream ss(s);
	ss >> id; ss.ignore();
	ss >> ti; ss.ignore();
	ss >> an; ss.ignore();
	ss >> al;
	Entidad *e = new Entidad(TipoEntidad(ti), id);
	e->setTam(an, al);
	return e;
}

Entidad::~Entidad() {
	// TODO Auto-generated destructor stub
}

