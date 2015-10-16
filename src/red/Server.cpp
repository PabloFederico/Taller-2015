/*
 * Server.cpp
 *
 *  Created on: 15 de oct. de 2015
 *      Author: emanuel
 */

#include "../red/Server.h"
#include "../red/SocketServidor.h"
#include "../red/Red.h"
#include <iostream>
#include <stdlib.h>

void Server::ejecutar(){
	/* código que debería ejecutar el servidor */
	std::cout << "soy el servidor"<<std::endl;

	SocketServidor* socket = new SocketServidor();

	if (socket->creadoCorrectamente() < 0){
		std::cout << "ERROR: No se puedo crear socket."<<std::endl;
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
		std::cout << "Cliente dice: "<< info <<std::endl;
	}

	string msg = "Hola, todavía no puedes jugar.";
	/* Enviando datos al cliente */
	if (Red::enviarInformacion(new_descriptor, msg) < 0){
		std::cout << "ERROR: send failed."<<std::endl;
		exit(EXIT_FAILURE);
	}else{
		std::cout << "Message send."<<std::endl;
	}

	socket->cerrarSocket();
	delete socket;
}

