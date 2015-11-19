/*
 * ControladorJuego.h
 *
 *  Created on: 14 de nov. de 2015
 *      Author: emanuel
 */

#ifndef SRC_CONTROLADOR_CONTROLADORJUEGO_H_
#define SRC_CONTROLADOR_CONTROLADORJUEGO_H_
#include "../modelo/Juego.h"
#include "Mouse.h"

class ControladorJuego {
private:
	Juego* juego;

public:
	ControladorJuego(Juego* juego);

	void actualizarJuego(Mouse* mouse);

	virtual ~ControladorJuego();
};

#endif /* SRC_CONTROLADOR_CONTROLADORJUEGO_H_ */
