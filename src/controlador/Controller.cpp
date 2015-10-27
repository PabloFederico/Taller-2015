/*
 * Controller.cpp
 *
 *  Created on: 6 de sept. de 2015
 *      Author: emanuel
 */

#include "../controlador/Controller.h"


Controller::Controller(Connection* lan = NULL) {
	if (lan != NULL)
		Proxy::esperarComienzo(lan);

	this->juego = new Juego(lan, NULL);
	if (lan != NULL)
		Proxy::enviar(lan, juego->getPosEntDeProtagonista());

	this->controladorMouse = new ControladorMouse(juego);
	this->controladorCamara = new ControladorCamara(juego);
}

Juego* Controller::getJuego(){
	return this->juego;
}

void Controller::procesarEvento(SDL_Event &event){
	int x,y;
	SDL_GetMouseState(&x,&y);
	controladorMouse->procesarEvento(event,x,y);
	controladorCamara->procesarPosicionMouse(x,y);

	if (this->juego->esCliente()) {
		try {
			TipoMensajeRed tipo = Proxy::actualizarMultiplayer(this->juego);
			std::cout << "Mensaje recibido y procesado de tipo "<<tipo<<std::endl;//
		} catch ( NoSeRecibio &e ) {}
	}
}

void Controller::agregarCamara(Camara *cam){
	this->controladorCamara->setCamara(cam);
}

void Controller::reiniciarJuego(){
	this->juego->reiniciar();
	delete this->controladorCamara;
	delete this->controladorMouse;
	this->controladorMouse = new ControladorMouse(juego);
	this->controladorCamara = new ControladorCamara(juego);
}

int Controller::verificarConexion(std::string string_IP){

	return -1;
}


Controller::~Controller() {
	delete this->juego;
	delete this->controladorCamara;
	delete this->controladorMouse;
}

