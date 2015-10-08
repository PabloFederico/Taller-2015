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

class ControladorCamara {
private:
	Juego *juego;
	Camara *camara;
public:
	ControladorCamara(Juego *juego);
	void setCamara(Camara *cam);
	void procesarPosicionMouse(int x, int y);
	virtual ~ControladorCamara();
};

#endif /* CONTROLADOR_CONTROLADORCAMARA_H_ */
