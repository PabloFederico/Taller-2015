/*
 * Loader.h
 *
 *  Created on: 16 de sept. de 2015
 *      Author: emanuel
 */

#ifndef LOADER_H_
#define LOADER_H_
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class Loader {
public:
	Loader();

	SDL_Texture* cargarImagen(SDL_Renderer* renderer, std::string path);

	virtual ~Loader();
};

#endif /* LOADER_H_ */
