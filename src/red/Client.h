/*
 * Client.h
 *
 *  Created on: 15 de oct. de 2015
 *      Author: emanuel
 */

#ifndef RED_CLIENT_H_
#define RED_CLIENT_H_

#include "Connection.h"
#include <sstream>


class Client: public Connection {
private:
	SocketCliente* socket;
	string direccion_ip;
	int PUERTO;

public:
	Client(string dir_ip, int puerto);
	bool iniciar();
//	bool reintentarConexion(string nombreActual);
	void finalizar();
	~Client();
};

#endif /* RED_CLIENT_H_ */
