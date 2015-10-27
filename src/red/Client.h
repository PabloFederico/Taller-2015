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

public:
	Client();
	bool iniciar();
	void finalizar();
	~Client();
};

#endif /* RED_CLIENT_H_ */
