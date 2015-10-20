/*
 * Client.cpp
 *
 *  Created on: 15 de oct. de 2015
 *      Author: emanuel
 */

#include "../red/Client.h"


Client::Client() {
	iniciar();//recibir res, tirar error
}


//código original
void Client::ejecutar(){
	/* código que debería ejecutar el cliente */
	std::cout << "======= CLIENTE ======="<<std::endl;

	SocketCliente* socket = new SocketCliente();

	if (socket->creadoCorrectamente() < 0){
		std::cout << "ERROR: No se puede crear socket."<<std::endl;
		exit(EXIT_FAILURE);
	}

	/* connect() */
	if (Red::crearConexion(socket) < 0){
		std::cout << "ERROR: connect failed."<<std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "Connected."<<std::endl;

	string msg = "Hola, quiero jugar al Age of Empires.";

	/* send() */
	if (Red::enviarInformacion(socket->getDescriptor(), msg) < 0){
		std::cout << "ERROR: send failed."<<std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "Message sent."<<std::endl;

	char info[MAX_BYTES_LECTURA];
	/* Recibiendo datos del servidor */
	if (Red::recibirInformacion(socket->getDescriptor(), info) < 0){
		std::cout << "ERROR: recv failed."<<std::endl;
	}else{
		std::cout << "Information received."<<std::endl;
		std::cout <<"Servidor dice: " << info << std::endl;
	}

	socket->cerrarSocket(socket->getDescriptor());
	delete socket;
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
	fcntl(this->socket->getDescriptor(), F_SETFL, O_NONBLOCK); // non-blocking mode

	this->lastDescriptor = this->socket->getDescriptor();
	std::cout << "Connected."<<std::endl;
	return true;
}

Client::~Client() {
	std::cout << "====== /CLIENTE/ ======" << std::endl;
}

