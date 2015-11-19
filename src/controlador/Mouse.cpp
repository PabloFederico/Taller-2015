/*
 * Mouse.cpp
 *
 *  Created on: 14 de nov. de 2015
 *      Author: emanuel
 */

#include "Mouse.h"

Mouse::Mouse() {
	estado = NO_CLICK;
	click_izq_presionado = false;
	click_der_presionado = false;
	coord_ant_inicializada = false;
}

EstadoMouse Mouse::getEstado(){
	return estado;
}

void Mouse::setEstado(EstadoMouse nuevoEstado){
	estado = nuevoEstado;
}

bool Mouse::estaInicializadaCoordAnterior(){
	return coord_ant_inicializada;
}

void Mouse::inicializarCoordenadaAnterior(Coordenada c){
	c_anterior = c;
	coord_ant_inicializada = true;
}

void Mouse::setXY(Coordenada c){
	this->c = c;
}

bool Mouse::click_der_y_arrastrado(){
	return (click_der_presionado && c != c_anterior);
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

void Mouse::pulsar_click_izq(){
	click_izq_presionado = true;
	click_der_presionado = false;
}

void Mouse::pulsar_click_der(){
	click_der_presionado = true;
	click_izq_presionado = false;
}

void Mouse::soltar_click_izq(){
	click_izq_presionado = false;
}

void Mouse::soltar_click_der(){
	click_der_presionado = false;
}

bool Mouse::click(){
	return (estado != NO_CLICK);
}

bool Mouse::clickIzqPresionado(){
	return click_izq_presionado;
}

bool Mouse::clickDerPresionado(){
	return click_der_presionado;
}

bool Mouse::botonFuePresionado(Boton* boton){
	if (click_izq_presionado){
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

