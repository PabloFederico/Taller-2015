/*
 * Server.cpp
 *
 *  Created on: 15 de oct. de 2015
 *      Author: emanuel
 */

#include "../red/Server.h"


Server::Server() {
	if (!iniciar()) {
		throw ConnectionProblem();
	}
}


///código original
void Server::ejecutar(){
	/* código que debería ejecutar el servidor */
	std::cout << "======= SERVIDOR ======="<<std::endl;

	SocketServidor* socket = new SocketServidor();

	if (socket->creadoCorrectamente() < 0){
		std::cout << "ERROR: No se pudo crear socket."<<std::endl;
		exit(EXIT_FAILURE);
	}

	/* bind() */
	if (Red::enlazarSocket(socket) < 0){
		std::cout << "ERROR: bind failed."<<std::endl;
		exit(EXIT_FAILURE);
	}

	/* listen() */
	Red::escucharConexiones(socket->getDescriptor(),MAX_CONEXIONES);
	std::cout << "waiting for incoming connections..."<<std::endl;

	/* accept() */
	int new_descriptor = Red::aceptarClientes(socket);

	if (new_descriptor < 0){
		std::cout << "ERROR: accept failed."<<std::endl;
	}
	std::cout << "Connection accepted."<<std::endl;

	char info[MAX_BYTES_LECTURA];
	/* recv() */
	if (Red::recibirInformacion(new_descriptor, info) < 0){
		std::cout << "ERROR: recv failed."<<std::endl;
	}else{
		std::cout << "Information received."<<std::endl;
		std::cout << "Cliente dice: "<< info << std::endl;
	}

	string msg = "Hola, todavía no puedes jugar.";
	/* Enviando datos al cliente */
	if (Red::enviarInformacion(new_descriptor, msg) < 0){
		std::cout << "ERROR: send failed."<<std::endl;
		exit(EXIT_FAILURE);
	}else{
		std::cout << "Message send."<<std::endl;
	}

	socket->cerrarSocket(new_descriptor);
	delete socket;
}



bool Server::iniciar() {
	//this->Connection::finalizar();
	/* código que debería ejecutar el servidor */
	std::cout << "======= SERVIDOR =======" << std::endl;

	this->socket = new SocketServidor();

	if (this->socket->creadoCorrectamente() < 0) {
		std::cout << "ERROR: No se pudo crear socket."<<std::endl;
		return false;
	}

	/* bind() */
	if (Red::enlazarSocket(this->socket) < 0) {
		std::cout << "ERROR: bind failed."<<std::endl;
		return false;
	}

	/* listen() */
	Red::escucharConexiones(this->socket->getDescriptor(),MAX_CONEXIONES);
	std::cout << "waiting for incoming connections..."<<std::endl;

	/* accept() */
	int new_descriptor = Red::aceptarClientes(this->socket);

	if (new_descriptor < 0){
		std::cout << "ERROR: accept failed."<<std::endl;
		return false;
	}
	fcntl(new_descriptor, F_SETFL, O_NONBLOCK); // non-blocking mode

	this->lastDescriptor = new_descriptor;
	std::cout << "Connected."<<std::endl;
	return true;
}


void Server::finalizar() {
	int fd = this->socket->getDescriptor();
	Connection::finalizar();
	this->socket->cerrarSocket(fd);
}

Server::~Server() {
	std::cout << "====== /SERVIDOR/ ======" << std::endl;
}
