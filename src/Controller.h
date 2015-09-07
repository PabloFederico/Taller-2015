/*
 * Controller.h
 *
 *  Created on: 6 de sept. de 2015
 *      Author: emanuel
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_
#include "Juego.h"
#include "VentanaJuego.h"

class Controller {
private:
	Juego *juego;
	VentanaJuego *ventana;

public:
	Controller();
	void iniciarJuego();
	virtual ~Controller();
};

#endif /* CONTROLLER_H_ */
