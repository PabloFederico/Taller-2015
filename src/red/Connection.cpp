/*
 * Connection.cpp
 *
 *  Created on: Oct 19, 2015
 *      Author: martin
 */

#include "Connection.h"


//Connection::Connection() {
//	idJug = 0;
//	lastDescriptor = 0;
//	lastPing = clock();
//}

//void Connection::chequearPing() {
//	if ((lastPing + 3.0*CLOCKS_PER_SEC) > clock())	// Si pasaron más de 3,0 segundos desde el último ping... salta.
//		throw Disconnected();
//}

//void Connection::reestablecerPing() {
//	lastPing = clock();
//}

//void Connection::revisarPing() {
//	chequearPing();
//	reestablecerPing();
//}


int Connection::getIDJugador() {
	return this->idJug;
}


void Connection::enviar(std::string s) {
	if (Red::enviarInformacion(this->lastDescriptor, s) < 0) {
		//Log::imprimirALog(ERR, "ERROR: send failed.");
		std::cout << "ERROR: send failed." << std::endl;
	} else
		std::cout << "Enviando: "<< s <<std::endl;//
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
