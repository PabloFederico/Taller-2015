/*
 * SocketServidor.h
 *
 *  Created on: 15 de oct. de 2015
 *      Author: emanuel
 */

#ifndef RED_SOCKETSERVIDOR_H_
#define RED_SOCKETSERVIDOR_H_
#include "../red/Socket.h"

class SocketServidor: public Socket{
public:
	SocketServidor(int puerto);
	virtual ~SocketServidor();
};

#endif /* RED_SOCKETSERVIDOR_H_ */
