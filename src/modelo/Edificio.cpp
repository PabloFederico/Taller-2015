/*
 * Edificio.cpp
 *
 *  Created on: 9 de nov. de 2015
 *      Author: emanuel
 */

#include "Edificio.h"

Edificio::Edificio(TipoEntidad tipo, int id_jug):Entidad(tipo,id_jug) {
	dni = 0;
}

void Edificio::set_id_jugador(int nuevoDuenio){
	idJug = nuevoDuenio;
}

void Edificio::set_identificador(int nuevoDNI){
	dni = nuevoDNI;
}

Edificio::~Edificio() {
	// TODO Auto-generated destructor stub
}

