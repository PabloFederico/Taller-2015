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
	int fps;
	int delay;
	/*
	Uint32 num_sprite;
	Uint32 segundos;
	Uint32 ticks;
	*/
	int milisegundos;

	SDL_Rect** frames;
	SDL_Rect frameActual;

	int direccion;
	int indexSpriteActual;

	SDL_Rect posicion;

	void cargarFrames();

public:
	Sprite(int cant_Direcciones, Uint32 cant_Img_Distintas, Imagen* imagen, SDL_Rect posicion);

	void setDireccion(int direccion);

	void efectuarMovimiento();

	Imagen* getImagen();

	SDL_Rect getPosicion();

	void setPosX(int x);
	void setPosY(int y);

	void mover(int cant_x, int cant_y);

	SDL_Rect getFrameActual();

	int getFps();

	void setFps(int fps);

	void setDelay(int delay);

	SDL_Rect getSDLRect(int i, int j);

	void resetTime();

	int currentTime();

	void acomodar();

	virtual ~Sprite();
};

#endif /* SPRITE_H_ */
