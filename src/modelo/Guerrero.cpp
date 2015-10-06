/*
 * Soldado.cpp
 *
 *  Created on: 6 de sept. de 2015
 *      Author: emanuel
 */

#include "../modelo/Guerrero.h"

Guerrero::Guerrero(TipoEntidad tipo):Entidad(tipo) {
	this->fps = 1;
	this->delay = 1;
}

int Guerrero::getFps(){
	return this->fps;
}

void Guerrero::setFps(int fps){
	this->fps = fps;
}

int Guerrero::getDelay(){
	return this->delay;
}

void Guerrero::setDelay(int delay){
	this->delay = delay;
}

Guerrero::~Guerrero() {
	// TODO Auto-generated destructor stub
}

