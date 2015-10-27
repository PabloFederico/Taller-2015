/*
 * Client.cpp
 *
 *  Created on: 15 de oct. de 2015
 *      Author: emanuel
 */

#include "../red/Client.h"


Client::Client() {
	if (!iniciar()) {
		throw ConnectionProblem();
	}
}

bool Client::iniciar() {
	//this->Connection::finalizar();
	/* código que debería ejecutar el cliente */
	std::cout << "======= CLIENTE =======" << std::endl;

	this->socket = new SocketCliente();

	if (this->socket->creadoCorrectamente() < 0) {
		std::cout << "ERROR: No se puede crear socket."<<std::endl;
		return false;
	}

	/* connect() */
	if (Red::crearConexion(this->socket) < 0) {
		std::cout << "ERROR: connect failed."<<std::endl;
		return false;
	}

	// Recibir # de jugador.
	std::stringstream ss(recibir());
	ss >> this->idJug;

	fcntl(this->socket->getDescriptor(), F_SETFL, O_NONBLOCK); // non-blocking mode

	this->lastDescriptor = this->socket->getDescriptor();
	std::cout << "Conectado como jugador #"<<idJug<<"."<<std::endl;
	return true;
}

Client::~Client() {
	std::cout << "====== /CLIENTE/ ======" << std::endl;
}

