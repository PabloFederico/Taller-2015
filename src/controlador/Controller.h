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
#include "Mouse.h"
//#include "../red/Proxy.h"
#include "ControladorMouse.h"
#include "ControladorCamara.h"
#include "ControladorJuego.h"


class Controller {
private:
	Juego *juego;
	Mouse* mouse;
	//Connection *lan;
	ControladorMouse *controladorMouse;
	ControladorCamara *controladorCamara;
	ControladorJuego *controladorJuego;

public:
	Controller();//Connection* lan);

	Juego* getJuego();

	Mouse* getMouse();

	void iniciarJuego();

	void actualizarEstadoDelJuego();

	void capturarEvento(SDL_Event &event);

	void procesarEvento();

	void agregarCamara(Camara *cam);

	void reiniciarJuego();

	int verificarConexion(std::string string_IP);

	void posicionarCamaraEnProtagonista();

	virtual ~Controller();
};

#endif /* CONTROLADOR_CONTROLLER_H_ */
