/*
 * Ventana.h
 *
 *  Created on: 29 de sept. de 2015
 *      Author: emanuel
 */

#ifndef VISTA_VENTANA_H_
#define VISTA_VENTANA_H_
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "../controlador/Controller.h"


class Ventana {
protected:
	Controller *controlador;
	SDL_Window *window;
	SDL_Renderer *renderer;
	TTF_Font *fuenteTexto;
	TTF_Font *fuenteTextoRecursos;
	Mix_Music *musica_fondo;
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;

	bool init();
	void close();

public:
	Ventana(Controller *controlador);
	virtual ~Ventana();
};

#endif /* VISTA_VENTANA_H_ */
