/*
 * Edificio.cpp
 *
 *  Created on: 9 de nov. de 2015
 *      Author: emanuel
 */

#include "Edificio.h"

Edificio::Edificio(TipoEntidad tipo, int id_jug, int dni): Entidad(tipo,id_jug) {
	this->dni = dni;
}

void Edificio::set_id_jugador(int nuevoDuenio){
	idJug = nuevoDuenio;
}

void Edificio::set_identificador(int nuevoDNI){
	dni = nuevoDNI;
}

int Edificio::get_identificador(){
	return dni;
}

bool Edificio::esEdificio(){
	return true;
}

int Edificio::getCostoPorUnidad(){
	switch (this->tipo){
	case CUARTEL: return COSTO_SOLDADO;
	case BARRACK : return COSTO_ARQUERO;
	case CENTRO_CIVICO : return COSTO_ALDEANO;
	default : return COSTO_ALDEANO;
	}
}

void Edificio::morir() {
	Entidad::morir();
}

Edificio::~Edificio() {
	// TODO Auto-generated destructor stub
}

