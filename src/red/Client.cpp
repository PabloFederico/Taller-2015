/*
 * Client.cpp
 *
 *  Created on: 15 de oct. de 2015
 *      Author: emanuel
 */

#include "../red/Client.h"
#include "../red/SocketCliente.h"
#include "../red/Red.h"
#include <iostream>
#include <stdlib.h>

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
	std::cout << "Message send."<<std::endl;

	char info[MAX_BYTES_LECTURA];
	/* Recibiendo datos del servidor */
	if (Red::recibirInformacion(socket->getDescriptor(), info) < 0){
		std::cout << "ERROR: recv failed."<<std::endl;
	}else{
		std::cout << "Information received."<<std::endl;
		std::cout <<"Servidor dice: "<< info <<std::endl;
	}

	socket->cerrarSocket();
	delete socket;
}

