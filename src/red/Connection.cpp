/*
 * Connection.cpp
 *
 *  Created on: Oct 19, 2015
 *      Author: martin
 */

#include "Connection.h"


void Connection::enviar(Camino cam) {
	string msg = cam.enc();//.c_str();// + '.';
	std::cout << "Enviando:" << std::endl << msg << std::endl << "-----";//
	if (Red::enviarInformacion(this->lastDescriptor, msg) < 0) {
		//Log::imprimirALog(ERR, "ERROR: send failed.");
		std::cout << "ERROR: send failed." << std::endl;
	} else
		std::cout << "Message sent" << std::endl;//
}


Camino Connection::recibirCamino() {
	char info[MAX_BYTES_LECTURA];
	if (Red::recibirInformacion(this->lastDescriptor, info) < 0)
		throw NoSeRecibio();
	std::cout << "Information received."<<std::endl;//
	std::cout << "Recibido:" << std::endl << info << std::endl << "-----";//
	return Camino::dec(info);
}


void Connection::finalizar() {
	this->socket->cerrarSocket(lastDescriptor);
	delete socket;
}

Connection::~Connection() {
	finalizar();
}

