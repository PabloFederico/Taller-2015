/*
 * CentroCivico.cpp
 *
 *  Created on: 9 de nov. de 2015
 *      Author: emanuel
 */

#include "CentroCivico.h"

CentroCivico::CentroCivico(int id_jug):Edificio(EDIFICIO,id_jug) {
	this->id_jug = id_jug;
}

Unidad* CentroCivico::crearNuevaUnidad(TipoEntidad tipo){
	return new Unidad(tipo,id_jug);
}

CentroCivico::~CentroCivico() {
	// TODO Auto-generated destructor stub
}

