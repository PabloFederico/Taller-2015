/*
 * Camara.cpp
 *
 *  Created on: 7 de oct. de 2015
 *      Author: emanuel
 */

#include "Camara.h"

Camara::Camara(int *cero_x, int *cero_y) {
	this->cero_x = cero_x;
	this->cero_y = cero_y;
	this->margen_scroll = 10;
	this->width = 10;
	this->height = 10;
	this->limite_X = 10;
	this->limite_Y = 10;
}

void Camara::mover(int cant_x, int cant_y){
	*this->cero_x += cant_x;
	*this->cero_y += cant_y;
}

void Camara::setLimites(int x, int y){
	this->limite_X = x;
	this->limite_Y = y;
}

void Camara::setMargenScrolling(int margen){
	this->margen_scroll = margen;
}

void Camara::setDimension(int width, int height){
	this->width = width;
	this->height = height;
}

std::pair<int,int> Camara::getDimension(){
	return std::make_pair(width,height);
}

std::pair<int,int> Camara::getLimites(){
	return std::make_pair(limite_X,limite_Y);
}

int Camara::getMargenScrolling(){
	return this->margen_scroll;
}

Camara::~Camara() {
	delete this->cero_x;
	delete this->cero_y;
}

