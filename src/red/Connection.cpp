/*
 * Connection.cpp
 *
 *  Created on: Oct 19, 2015
 *      Author: martin
 */

#include "Connection.h"


int Connection::getIDJugador() {
	return this->idJug;
}


void Connection::enviar(std::string s) {
	if (Red::enviarInformacion(this->lastDescriptor, s) < 0) {
		//Log::imprimirALog(ERR, "ERROR: send failed.");
		std::cout << "ERROR: send failed." << std::endl;
	} else
		std::cout << "Enviando: "<<s<<std::endl;//
}

string Connection::recibir() {
	char buffer[MAX_BYTES_LECTURA];
	if (Red::recibirInformacion(this->lastDescriptor, buffer) < 0)
		throw NoSeRecibio();
	std::cout << "Recibido: "<<buffer<<std::endl;//
	return buffer;
}


void Connection::finalizar() {
	this->socket->cerrarSocket(lastDescriptor);
	delete socket;
}

Connection::~Connection() {
	finalizar();
}
