/*
 * Boton.cpp
 *
 *  Created on: 25 de oct. de 2015
 *      Author: emanuel
 */

#include "Boton.h"

Boton::Boton(std::string data, SDL_Rect pos) {
	this->rect = pos;
	presionado = false;
	info = data;
	imagenTexto = NULL;
}

std::string Boton::getInfoBoton(){
	return this->info;
}

void Boton::resetBoton(){
	presionado = false;
}

Imagen* Boton::getImagen(){
	return this->imagenTexto;
}

bool Boton::fue_presionado(){
	return presionado;
}

void Boton::presionarBoton(){
	frameActual = frames[1];
	presionado = true;
}

SDL_Rect Boton::getPosicion(){
	return rect;
}

SDL_Rect Boton::getFrameActual(){
	return frameActual;
}

void Boton::setImagen(Imagen* imagen){
	this->imagenTexto = imagen;
	rect.w = imagen->getPixelsX() / 2;
	rect.h = imagen->getPixelsY();
	SDL_Rect rect0;
	rect0.x = 0;
	rect0.y = 0;
	rect0.w = rect.w;
	rect0.h = rect.h;
	frames[0] = rect0;
	SDL_Rect rect1;
	rect1.x = rect.w;
	rect1.y = 0;
	rect1.w = rect.w;
	rect1.h = rect.h;
	frames[1] = rect1;
	frameActual = frames[0];
}

Boton::~Boton() {
	delete this->imagenTexto;
}

