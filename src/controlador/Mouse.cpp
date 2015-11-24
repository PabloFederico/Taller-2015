/*
 * Mouse.cpp
 *
 *  Created on: 14 de nov. de 2015
 *      Author: emanuel
 */

#include "Mouse.h"

Mouse::Mouse(){
	estado = NO_CLICK;
	moviendoImagen = false;
}

EstadoMouse Mouse::getEstado(){
	return estado;
}

void Mouse::setEstado(EstadoMouse nuevoEstado){
	estado = nuevoEstado;
}

void Mouse::setXY(Coordenada c){
	this->c = c;
}

void Mouse::setXY_anterior(Coordenada c){
	this->c_anterior = c;
}

Coordenada Mouse::getXY(){
	return c;
}

Coordenada Mouse::getXY_anterior(){
	return c_anterior;
}

void Mouse::setearMoviendoImagen(bool valor){
	moviendoImagen = valor;
}

bool Mouse::estaMoviendoImagen(){
	return moviendoImagen;
}

bool Mouse::click(){
	return (estado != NO_CLICK);
}

bool Mouse::botonFuePresionado(Boton* boton){
	if (estado == CLICK_IZQUIERDO){
		int x = boton->getPosicion().x;
		int y = boton->getPosicion().y;
		int w = boton->getPosicion().w;
		int h = boton->getPosicion().h;
		if ( c.x > x && c.x < x+w && c.y > y && c.y < y+h ){
			boton->presionarBoton();
			return true;
		}
		else return false;
	}
	else return false;
}

Mouse::~Mouse() {
	// TODO Auto-generated destructor stub
}

