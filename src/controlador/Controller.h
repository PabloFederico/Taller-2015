/*
 * Controller.h
 *
 *  Created on: 6 de sept. de 2015
 *      Author: emanuel
 */

#ifndef CONTROLADOR_CONTROLLER_H_
#define CONTROLADOR_CONTROLLER_H_
#include "../modelo/Juego.h"
#include "../vista/VentanaJuego.h"

class Controller {
private:
	Juego *juego;
	VentanaJuego *ventana;

public:
	Controller();
	void iniciarJuego();
	virtual ~Controller();
};

#endif /* CONTROLADOR_CONTROLLER_H_ */
