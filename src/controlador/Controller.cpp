/*
 * Controller.cpp
 *
 *  Created on: 6 de sept. de 2015
 *      Author: emanuel
 */

#include "../controlador/Controller.h"


Controller::Controller(Connection* lan = NULL) {
	try {
		if (lan != NULL)
			Proxy::esperarComienzo(lan);
	} catch ( Disconnected &e ) { lan = NULL; }

	this->juego = new Juego(lan, NULL);
	if (lan != NULL)
		Proxy::enviar(lan, juego->getPosEntDeProtagonista());

	this->controladorMouse = new ControladorMouse(juego);
	this->controladorCamara = new ControladorCamara(juego);
}

Juego* Controller::getJuego(){
	return this->juego;
}

void Controller::posicionarCamaraEnProtagonista(){
	bool playerEstaEnElCentro = false;
	int cant_mover = 10;
	int width_camera = juego->getDimensionVentana().first;
	int height_camera = juego->getDimensionVentana().second - juego->getBarraEstado()->getDimension().second;
	while (!playerEstaEnElCentro){
		SDL_Rect posicion = juego->getSpritePlayer()->getPosicion();

		if (posicion.x < (width_camera/2 - 30)){
			controladorCamara->moverCamara(cant_mover,0);
		}
		posicion = juego->getSpritePlayer()->getPosicion();
		if (posicion.x > (width_camera/2 + 30)){
			controladorCamara->moverCamara(-cant_mover,0);
		}
		posicion = juego->getSpritePlayer()->getPosicion();
		if (posicion.y < (height_camera/2 - 30)){
			controladorCamara->moverCamara(0,cant_mover);
		}
		posicion = juego->getSpritePlayer()->getPosicion();
		if (posicion.y > (height_camera/2 + 30)){
			controladorCamara->moverCamara(0,-cant_mover);
		}
		posicion = juego->getSpritePlayer()->getPosicion();

		if (posicion.x > 0 &&
			posicion.x < width_camera &&
			posicion.y > 0 &&
			posicion.y < height_camera){
			playerEstaEnElCentro = true;
		}
	}
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
		} catch ( NoSeRecibio &e ) {
		} catch ( Disconnected &e ) {
			juego->olvidarConnection();
		}
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

