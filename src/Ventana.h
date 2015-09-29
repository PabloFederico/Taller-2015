/*
 * Ventana.h
 *
 *  Created on: 29 de sept. de 2015
 *      Author: emanuel
 */

#ifndef VENTANA_H_
#define VENTANA_H_
#include <SDL2/SDL.h>
#include "Juego.h"

class Ventana {
protected:
	SDL_Window *window;
	SDL_Renderer *renderer;
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;

	bool init();
	void close();

public:
	Ventana(Juego *juego);
	virtual ~Ventana();
};

#endif /* VENTANA_H_ */
