/*
 * Ventana.h
 *
 *  Created on: 29 de sept. de 2015
 *      Author: emanuel
 */

#ifndef VISTA_VENTANA_H_
#define VISTA_VENTANA_H_
#include <SDL2/SDL.h>

#include "../modelo/Juego.h"

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

#endif /* VISTA_VENTANA_H_ */
