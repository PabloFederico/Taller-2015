/*
 * ControladorMouse.h
 *
 *  Created on: 8 de oct. de 2015
 *      Author: emanuel
 */

#ifndef CONTROLADOR_CONTROLADORMOUSE_H_
#define CONTROLADOR_CONTROLADORMOUSE_H_
#include "../modelo/Juego.h"
//#include "../red/Proxy.h"
#include <SDL2/SDL.h>

class ControladorMouse {
private:
	Juego* juego;

public:
	ControladorMouse(Juego *juego);

	void procesarEvento(SDL_Event &event, int x, int y);//, Connection *lan);

	virtual ~ControladorMouse();
};

#endif /* CONTROLADOR_CONTROLADORMOUSE_H_ */
