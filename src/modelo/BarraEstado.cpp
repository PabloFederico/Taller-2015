/*
 * BarraInfo.cpp
 *
 *  Created on: 21 de oct. de 2015
 *      Author: emanuel
 */

#include "../modelo/BarraEstado.h"

BarraEstado::BarraEstado(int ancho, int alto, Jugador* jug): jugador(jug) {
	this->height = alto;
	this->width = ancho;
}

std::pair<int, int> BarraEstado::getDimension(){
	return std::make_pair(width,height);
}

std::string BarraEstado::getDescripcion(){
	return infoDescripcion;
}

std::map<TipoEntidad,int> BarraEstado::getMapRecursosEconomicos() {
	return jugador->getMapRecursosEconomicos();
}

void BarraEstado::setInformacion(std::string nuevaDescripcion){
	this->infoDescripcion = nuevaDescripcion;
}

BarraEstado::~BarraEstado() {
}

