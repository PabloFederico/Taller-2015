/*
 * Castillo.cpp
 *
 *  Created on: 6 de sept. de 2015
 *      Author: emanuel
 */

#include "Castillo.h"

Castillo::Castillo(TipoEntidad tipo): Entidad(tipo){
	this->alto_base = 1;
	this->ancho_base = 1;
	this->pixel_ref_x = 0;
	this->pixel_ref_y = 0;
}

void Castillo::setDimension(int ancho, int alto){
	this->ancho_base = ancho;
	this->alto_base = alto;
}

std::pair<int,int> Castillo::getDimension(){
	return std::make_pair(this->ancho_base,this->alto_base);
}

std::pair<int,int> Castillo::getPixelRef(){
	return std::make_pair(this->pixel_ref_x,this->pixel_ref_y);
}

void Castillo::setPixelRef(int ref_x, int ref_y){
	this->pixel_ref_x = ref_x;
	this->pixel_ref_y = ref_y;
}

Castillo::~Castillo() {
	// TODO Auto-generated destructor stub
}

