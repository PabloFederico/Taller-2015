/*
 * Server.h
 *
 *  Created on: 15 de oct. de 2015
 *      Author: emanuel
 */

#ifndef RED_SERVER_H_
#define RED_SERVER_H_

#include "Connection.h"


class Server: public Connection {
private:
	SocketServidor* socket;

public:
	static void ejecutar();

	Server();
	bool iniciar();
	void finalizar();
	~Server();
};

#endif /* RED_SERVER_H_ */
