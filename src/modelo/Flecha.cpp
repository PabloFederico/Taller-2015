/*
 * Flecha.cpp
 *
 *  Created on: 28 de nov. de 2015
 *      Author: emanuel
 */

#include "Flecha.h"
#include <math.h>

Flecha::Flecha(Unidad* emisor, Direccion direccion):Entidad(FLECHA,emisor->getIDJug()) {
	this->emisor = emisor;
	this->direccion = direccion;
	c = emisor->getPosicion();
	origen = c;
}

int Flecha::getIdEmisor(){
	return getIDJug();
}

Direccion Flecha::getDireccion(){
	return direccion;
}

float distanciaEuclidiana2(Coordenada a, Coordenada z) {
	return sqrt( pow(z.x-a.x,2) + pow(z.y-a.y,2) );
}

bool Flecha::llegoASuLimite(){
	if (distanciaEuclidiana2(c,origen) > 3.0)
		return true;
	return false;
}

void Flecha::lastimar(Entidad* receptor){
	try {
		receptor->sufrirGolpe(emisor->generarGolpe());
	} catch (EntidadMurio &e) {
		emisor->olvidarInteraccion();
	}
}

Flecha::~Flecha() {
	// TODO Auto-generated destructor stub
}

