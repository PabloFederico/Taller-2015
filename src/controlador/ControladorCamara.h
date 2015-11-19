/*
 * ControladorCamara.h
 *
 *  Created on: 7 de oct. de 2015
 *      Author: emanuel
 */

#ifndef CONTROLADOR_CONTROLADORCAMARA_H_
#define CONTROLADOR_CONTROLADORCAMARA_H_

#include "../vista/Camara.h"
#include "../modelo/Juego.h"
#include "Mouse.h"

class ControladorCamara {
private:
	Juego *juego;
	Camara *camara;
public:
	ControladorCamara(Juego *juego);
	void setCamara(Camara *cam);
	void actualizarScroll(Mouse* mouse);
	void procesarPosicionMouse(int x, int y);
	void moverCamara(int cant_x, int cant_y);
	virtual ~ControladorCamara();
};

#endif /* CONTROLADOR_CONTROLADORCAMARA_H_ */
