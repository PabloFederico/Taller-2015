/*
 * Controller.cpp
 *
 *  Created on: 6 de sept. de 2015
 *      Author: emanuel
 */

#include "../controlador/Controller.h"


Controller::Controller(Connection* lan = NULL) {
	this->juego = new Juego(lan);
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

Controller::~Controller() {
	delete this->juego;
}

