/*
 * Controller.h
 *
 *  Created on: 6 de sept. de 2015
 *      Author: emanuel
 */

#ifndef CONTROLADOR_CONTROLLER_H_
#define CONTROLADOR_CONTROLLER_H_
#include <SDL2/SDL.h>
#include "../modelo/Juego.h"
#include "../red/Connection.h"
#include "ControladorMouse.h"
#include "ControladorCamara.h"

class Controller {
private:
	Juego *juego;
	ControladorMouse *controladorMouse;
	ControladorCamara *controladorCamara;

public:
	Controller(Connection* lan);

	Juego* getJuego();

	void iniciarJuego();

	void procesarEvento(SDL_Event &event);

	void agregarCamara(Camara *cam);

	void reiniciarJuego();

	int verificarConexion(std::string string_IP);

	virtual ~Controller();
};

#endif /* CONTROLADOR_CONTROLLER_H_ */
