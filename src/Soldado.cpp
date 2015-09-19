/*
 * Soldado.cpp
 *
 *  Created on: 6 de sept. de 2015
 *      Author: emanuel
 */

#include "Soldado.h"

Soldado::Soldado(std::string tipo): Entidad(tipo) {
	this->fps = 1;
	this->delay = 1;
}

void Soldado::setFps(int fps){
	this->fps = fps;
}

void Soldado::setDelay(int delay){
	this->delay = delay;
}

Soldado::~Soldado() {
	// TODO Auto-generated destructor stub
}

