/*
 * Client.cpp
 *
 *  Created on: 15 de oct. de 2015
 *      Author: emanuel
 */

#include "../red/Client.h"
#include <yaml-cpp/yaml.h>


string parsearIP() {
	string ip = IP_SERVIDOR;
	YAML::Node config;
	try {
		config = YAML::LoadFile("config.yaml");
		if (config["direccion_ip"])
			ip = config["direccion_ip"].as<string>();
	} catch (YAML::BadFile &e) {}
	return ip;
}

string parsearNombreJugador() {
	string nombre;
	YAML::Node config;
	try {
		config = YAML::LoadFile("config.yaml");
		if (config["nombre_jugador"])
			nombre = config["nombre_jugador"].as<string>();
		else nombre = "Octai";
	} catch (YAML::BadFile &e) {
		nombre = "Guido";
	}
	return nombre;
}


Client::Client() {
	try {
		if (!iniciar())
			throw ConnectionProblem();
	} catch ( Disconnected &e ) {
		throw ConnectionProblem();
	}
}

bool Client::iniciar() {
	string ip = parsearIP();

	/* código que debería ejecutar el cliente */
	std::cout << "======= CLIENTE =======" << std::endl;

	this->socket = new SocketCliente(ip);

	if (this->socket->creadoCorrectamente() < 0) {
		std::cout << "ERROR: No se puede crear socket."<<std::endl;
		return false;
	}

	/* connect() */
	bool res = -1;
	std::cout << "Intentado conectarse..."<<std::endl;
	for (int i = 0; i < MAX_CONEXIONES; i++) {
		res = Red::crearConexion(this->socket);
		if (res >= 0)
			break;
		sleep(5);
	}
	if (res < 0) {
		std::cout << "ERROR: Tras "<<MAX_CONEXIONES<<" intentos, connect failed."<<std::endl;
		return false;
	}
	this->lastDescriptor = this->socket->getDescriptor();

	std::cout << "Success."<<std::endl;

	// Envía el nombre de jugador.
	ostringstream encode;
	encode << "<COM>"<<parsearNombreJugador()<<"~";
	send(this->lastDescriptor, encode.str().c_str(), MAX_BYTES_LECTURA, 0);

	// Recibir # de jugador.
	this->idJug = 0;
	do {
		try {
			std::stringstream ss(recibir());
			ss >> this->idJug;
		} catch ( NoSeRecibio &e ) {}
	} while (this->idJug == 0);

	fcntl(this->socket->getDescriptor(), F_SETFL, O_NONBLOCK); // non-blocking mode
	std::cout << "Conectado como jugador #"<<idJug<<"."<<std::endl;
	return true;
}

Client::~Client() {
	std::cout << "====== /CLIENTE/ ======" << std::endl;
}

