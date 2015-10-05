/*
 * Imagen.cpp
 *
 *  Created on: 16 de sept. de 2015
 *      Author: emanuel
 */

#include "../vista/Imagen.h"

Imagen::Imagen(int width, int height, SDL_Texture *texture) {
	this->width = width;
	this->height = height;
	this->imagen = texture;
}

SDL_Texture* Imagen::getTexture(){
	return this->imagen;
}

int Imagen::getPixelsX(){
	return this->width;
}

int Imagen::getPixelsY(){
	return this->height;
}

Imagen::~Imagen() {
	SDL_DestroyTexture(this->imagen);
	this->imagen = NULL;
}

