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

SDL_Texture* Loader::cargarImagen(SDL_Renderer* renderer, std::string path){
	SDL_Texture *texture = NULL;
	texture = IMG_LoadTexture(renderer, path.c_str());
	return texture;
}

Loader::~Loader() {
	// TODO Auto-generated destructor stub
}

