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
	string nombre = "Octai";
	YAML::Node config;
	try {
		config = YAML::LoadFile("config.yaml");
		if (config["nombre_jugador"])
			nombre = config["nombre_jugador"].as<string>();
		else nombre = "Ema";
	} catch (YAML::BadFile &e) {
		nombre = "Pablito";
	} catch (YAML::ParserException &e) {
		nombre = "Guido!";
	}
	return nombre;
}


Client::Client() {
	std::cout << "======= CLIENTE =======" << std::endl;
	try {
		if (!iniciar())
			throw ConnectionProblem();
	} catch ( Disconnected &e ) {
		throw ConnectionProblem();
	}
}

bool Client::iniciar() {
	string ip = parsearIP();
	this->socket = new SocketCliente(ip);
	if (this->socket->creadoCorrectamente() < 0) {
		std::cout << "ERROR: No se puede crear socket."<<std::endl;
		return false;
	}
	std::cout << "Intentado conectarse ("<<MAX_CONEXIONES<<" intentos)";
	bool res = -1;
	for (int i = 0; i < MAX_CONEXIONES; i++) {
		res = Red::crearConexion(this->socket);
		std::cout << ".";
		if (res >= 0)
			break;
		sleep(5);
	}
	if (res < 0) {
		std::cout << std::endl << "ERROR: connect failed."<<std::endl;
		return false;
	} else {
		this->lastDescriptor = this->socket->getDescriptor();
		std::cout << std::endl << "Success."<<std::endl;
	}

	// Envía el nombre de jugador.
	string mensaje = Red::agregarPrefijoYFinal("COM", parsearNombreJugador());
	send(this->lastDescriptor, mensaje.c_str(), MAX_BYTES_LECTURA, 0);

	// Recibe su número de jugador.
	this->idJug = 0;
	do {
		try {
			std::stringstream ss(recibir());
			ss >> this->idJug;
		} catch ( NoSeRecibio &e ) {}
	} while (this->idJug == 0);

	fcntl(this->socket->getDescriptor(), F_SETFL, O_NONBLOCK); // non-blocking mode
	std::cout << "Conectado como jugador #"<<this->idJug<<"."<<std::endl;
	return true;
}

Client::~Client() {
	// El destructor de Connection se ocupa de llamar a finalizar
	std::cout << "====== /CLIENTE/ ======" << std::endl;
}

