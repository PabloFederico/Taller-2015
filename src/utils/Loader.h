/*
 * Loader.h
 *
 *  Created on: 16 de sept. de 2015
 *      Author: emanuel
 */

#ifndef UTILS_LOADER_H_
#define UTILS_LOADER_H_
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

#include "../vista/Imagen.h"

class Loader {
public:
	Loader(){};

	Imagen* cargarImagen(SDL_Renderer* renderer, std::string path){
		SDL_Texture *texture = NULL;
		SDL_Surface* surface = IMG_Load(path.c_str());


		texture = SDL_CreateTextureFromSurface(renderer,surface);
		Imagen* imagen = new Imagen(surface->w, surface->h, texture);
		SDL_FreeSurface(surface);
		surface = NULL;

		return imagen;
	};

	virtual ~Loader(){};
};

#endif /* UTILS_LOADER_H_ */
