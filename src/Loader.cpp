/*
 * Loader.cpp
 *
 *  Created on: 16 de sept. de 2015
 *      Author: emanuel
 */

#include "Loader.h"

Loader::Loader() {
	// TODO Auto-generated constructor stub

}

Imagen* Loader::cargarImagen(SDL_Renderer* renderer, std::string path){
	SDL_Texture *texture = NULL;
	SDL_Surface* surface = IMG_Load(path.c_str());

	texture = SDL_CreateTextureFromSurface(renderer,surface);
	Imagen* imagen = new Imagen(surface->w, surface->h, texture);

	return imagen;
}

Loader::~Loader() {
	// TODO Auto-generated destructor stub
}

