/*
 * SocketCliente.h
 *
 *  Created on: 15 de oct. de 2015
 *      Author: emanuel
 */

#ifndef RED_SOCKETCLIENTE_H_
#define RED_SOCKETCLIENTE_H_
#include "../red/Socket.h"

class SocketCliente: public Socket {
public:
	SocketCliente();
	SocketCliente(std::string string_IP, int puerto);
	virtual ~SocketCliente();
};

#endif /* RED_SOCKETCLIENTE_H_ */
