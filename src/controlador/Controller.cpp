/*
 * Controller.cpp
 *
 *  Created on: 6 de sept. de 2015
 *      Author: emanuel
 */

#include "../controlador/Controller.h"

Controller::Controller() {
	this->juego = new Juego();
	this->ventana = new VentanaJuego(this->juego);
}

void Controller::iniciarJuego(){
	this->ventana->mostrar();
}

Controller::~Controller() {
	//delete this->juego;
	delete this->ventana;
}

