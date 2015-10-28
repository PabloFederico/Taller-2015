/*
 * Connection.h
 *
 *  Created on: Oct 19, 2015
 *      Author: martin
 */

#ifndef RED_CONNECTION_H_
#define RED_CONNECTION_H_

#include <iostream>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <string>

#include "SocketCliente.h"
#include "SocketServidor.h"
#include "Red.h"
//#include "../utils/Structs.h"
#include "../utils/Exceptions.h"
#include "../utils/Log.h"


class Connection {
protected:
	Socket* socket = NULL;
	int lastDescriptor;
	int idJug;

public:
	virtual bool iniciar() = 0;

	int getIDJugador();

	void enviar(std::string);
	std::string recibir();

	void finalizar();
	virtual ~Connection();
};

#endif /* RED_CONNECTION_H_ */
