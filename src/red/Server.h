/*
 * Server.h
 *
 *  Created on: 15 de oct. de 2015
 *      Author: emanuel
 */

#ifndef RED_SERVER_H_
#define RED_SERVER_H_

#include "Connection.h"
#include "Proxy.h"
#include <cerrno>


class Server: public Connection {
private:
	SocketServidor* socket;
	fd_set descriptors;

public:
	Server();
	bool iniciar();

	void correr();

	void finalizar();
	~Server();
};

#endif /* RED_SERVER_H_ */
