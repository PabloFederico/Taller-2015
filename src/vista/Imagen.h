/*
 * Imagen.h
 *
 *  Created on: 16 de sept. de 2015
 *      Author: emanuel
 */

#ifndef VISTA_IMAGEN_H_
#define VISTA_IMAGEN_H_
#include <SDL2/SDL.h>

class Imagen {
private:
	int width;
	int height;
	SDL_Texture *imagen;

public:
	Imagen(int width, int height, SDL_Texture *texture);

	SDL_Texture* getTexture();

	int getPixelsX();

	int getPixelsY();

	virtual ~Imagen();
};

#endif /* VISTA_IMAGEN_H_ */
