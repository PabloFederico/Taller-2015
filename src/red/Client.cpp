/*
 * Client.cpp
 *
 *  Created on: 15 de oct. de 2015
 *      Author: emanuel
 */

#include "../red/Client.h"
#include <yaml-cpp/yaml.h>


void parsearIPyNombreJugador(string* ip, string *nombre) {
	*ip = IP_SERVIDOR;
	*nombre = NOMBRE_DEFAULT;
	try {
		YAML::Node config;
		config = YAML::LoadFile("config.yaml");
		if (config["nombre_jugador"])
			*nombre = config["nombre_jugador"].as<string>();
		if (config["direccion_ip"])
			*ip = config["direccion_ip"].as<string>();
		return;
	} catch (YAML::BadFile &e) {
	} catch (YAML::ParserException &e) {
	}
	*ip = "127.0.0.1"; // default
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
	string ip, nombreJug;
	parsearIPyNombreJugador(&ip, &nombreJug);
	std::cout << ip<<std::endl;//

	std::cout << "======= CLIENTE =======" << std::endl;
	this->socket = new SocketCliente(ip);
	if (this->socket->creadoCorrectamente() < 0) {
		std::cout << "ERROR: No se puede crear socket."<<std::endl;
		return false;
	}
	std::cout << "Intentado conectarse ("<<MAX_CONEXIONES<<" intentos)"<<std::endl;
	bool res = -1;

	// MAX_CONEXIONES intentos de conectarse, separados por 5 segundos.
	for (int i = 0; i < MAX_CONEXIONES; i++) {
		res = Red::crearConexion(this->socket);
		std::cout << ".";
		if (res > 0)
			break;
		sleep(5);
	}
	if (res < 0) {
		std::cout << std::endl << "ERROR: connect failed."<<std::endl;
		return false;
	}

	this->lastDescriptor = this->socket->getDescriptor();

	// Recibe su número de jugador.
	this->idJug = 0;
	do {
		try {
			std::stringstream ss(recibir());
			ss >> this->idJug;
		} catch ( NoSeRecibio &e ) {}
	} while (this->idJug == 0);

	fcntl(this->socket->getDescriptor(), F_SETFL, O_NONBLOCK); // non-blocking mode

	std::cout << std::endl << "Success! Conectado como jugador #"<<this->idJug<<"."<<std::endl;
	return true;
}


//bool Client::reintentarConexion(string nombreActual) {
//	string ip, nombreJug;
//	parsearIPyNombreJugador(&ip, &nombreJug);
//
//	this->socket = new SocketCliente(ip);
//	if (this->socket->creadoCorrectamente() < 0)
//		return false;
//	if (Red::crearConexion(this->socket) < 0)
//		return false;
//
//	this->lastDescriptor = this->socket->getDescriptor();
//	string mensaje = Red::agregarPrefijoYFinal("COM", nombreActual);
//	send(this->lastDescriptor, mensaje.c_str(), MAX_BYTES_LECTURA, 0);
//	// Recibe su número de jugador.
//	this->idJug = 0;
//	do {
//		try {
//			std::stringstream ss(recibir());
//			ss >> this->idJug;
//		} catch ( NoSeRecibio &e ) {}
//	} while (this->idJug == 0);
//
//	fcntl(this->socket->getDescriptor(), F_SETFL, O_NONBLOCK); // non-blocking mode
//	std::cout << std::endl << "Success! Reconectado como jugador #"<<this->idJug<<"."<<std::endl;
//	return true;
//}


Client::~Client() {
	// El destructor de Connection se ocupa de llamar a finalizar
	std::cout << "====== /CLIENTE/ ======" << std::endl;
}

