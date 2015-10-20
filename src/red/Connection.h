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

#include "SocketCliente.h"
#include "SocketServidor.h"
#include "Red.h"
#include "../utils/Structs.h"
#include "../modelo/Log.h"


class Connection {
protected:
	Socket* socket = NULL;
	int lastDescriptor;

public:
	virtual bool iniciar() = 0;

	void enviar(Camino cam);
	//enviar de cada tipo

	Camino recibirCamino();
	//recibir de cada tipo

	void finalizar();
	virtual ~Connection();
};

#endif /* RED_CONNECTION_H_ */
