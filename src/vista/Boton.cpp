/*
 * Boton.cpp
 *
 *  Created on: 25 de oct. de 2015
 *      Author: emanuel
 */

#include "Boton.h"

Boton::Boton(TipoBoton tipo, SDL_Rect pos) {
	this->tipo = tipo;
	this->rect = pos;
	switch (tipo){
		case BOTON_SELECTOR_ESCENARIO: info = "Escenario";
						 break;
		case BOTON_EXIT: info = "Exit";
						 break;
		case BOTON_NUEVA_PARTIDA: info = "Nueva\n Partida";
						 break;
		case BOTON_CARGAR_PARTIDA: info = "Cargar\n Partida";
						 break;
		default : 		 info = " ";
						 break;
	}
	imagenTexto = NULL;
}

TipoBoton Boton::getTipo(){
	return tipo;
}

std::string Boton::getInfoBoton(){
	return this->info;
}

Imagen* Boton::getImagen(){
	return this->imagenTexto;
}

SDL_Rect Boton::getPosicion(){
	return rect;
}

void Boton::setImagen(Imagen* imagen){
	this->imagenTexto = imagen;
}

Boton::~Boton() {
	delete this->imagenTexto;
}

