/*
 * Sprite.h
 *
 *  Created on: 16 de sept. de 2015
 *      Author: emanuel
 */

#ifndef SPRITE_H_
#define SPRITE_H_
#include <SDL2/SDL.h>
#include <vector>
#include "Imagen.h"

class Sprite {
private:
	int cant_Direcciones;
	int cant_Img_Distintas;
	Imagen* imagen;

	SDL_Rect** frames;
	SDL_Rect frameActual;

	int direccion;
	int indexSpriteActual;

	void cargarFrames();

public:
	Sprite(int cant_Direcciones, int cant_Img_Distintas, Imagen* imagen);

	void setDireccion(int direccion);

	void efectuarMovimiento();

	Imagen* getImagen();

	SDL_Rect getSDLRectActual();

	virtual ~Sprite();
};

#endif /* SPRITE_H_ */
