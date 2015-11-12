/*
 * Unidad.cpp
 *
 *  Created on: 9 de nov. de 2015
 *      Author: emanuel
 */

#include "Unidad.h"
#include <string>
#include <sstream>
using namespace std;

Unidad::Unidad(TipoEntidad tipo, int id_jug): Entidad(tipo,id_jug) {
	this->dni = 0;
	petrificado = false;
	ostringstream ssInfo;
	ssInfo << info<<" (Jugador "<<id_jug<<")";
	info = ssInfo.str();
}

void Unidad::set_id_jugador(int nuevoDuenio){
	idJug = nuevoDuenio;
}

int Unidad::get_identificador(){
	return dni;
}

void Unidad::set_identificador(int nuevoDNI){
	dni = nuevoDNI;
}

bool Unidad::estaPetrificado(){
	return petrificado;
}

void Unidad::petrificar(){
	petrificado = true;
}

void Unidad::despetrificar(){
	petrificado = false;
}

std::string Unidad::enc(){
	ostringstream enc;
	enc << idJug<<","<<tipo<<","<<ancho<<","<<alto;
	return enc.str();
}

Unidad* Unidad::dec(std::string s){
	int id,ti,an,al;
	stringstream ss(s);
	ss >> id; ss.ignore();
	ss >> ti; ss.ignore();
	ss >> an; ss.ignore();
	ss >> al;
	Unidad *u = new Unidad(TipoEntidad(ti),id);
	u->setTam(an, al);
	return u;
}

Unidad::~Unidad() {
	// TODO Auto-generated destructor stub
}

