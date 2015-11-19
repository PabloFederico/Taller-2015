/*
 * ControladorMouse.h
 *
 *  Created on: 8 de oct. de 2015
 *      Author: emanuel
 */

#ifndef CONTROLADOR_CONTROLADORMOUSE_H_
#define CONTROLADOR_CONTROLADORMOUSE_H_
#include "../modelo/Juego.h"
#include "Mouse.h"
//#include "../red/Proxy.h"
#include <SDL2/SDL.h>

class ControladorMouse {
private:
	Juego* juego;

	void procesarClickIzquierdo(Mouse* mouse);
	void procesarClickDerecho(Mouse* mouse);
	void procesarArrastreClickDerecho(Mouse* mouse);

public:
	ControladorMouse(Juego *juego);

	void procesarMouse(Mouse* mouse);
	//void procesarEvento(SDL_Event &event, int x, int y, Connection *lan);

	virtual ~ControladorMouse();
};

#endif /* CONTROLADOR_CONTROLADORMOUSE_H_ */
