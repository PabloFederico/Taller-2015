/*
 * SocketCliente.cpp
 *
 *  Created on: 15 de oct. de 2015
 *      Author: emanuel
 */

#include "../red/SocketCliente.h"

SocketCliente::SocketCliente():Socket() {
	if (descriptor > -1){
		infoDir.sin_family = AF_INET;
		infoDir.sin_addr.s_addr = inet_addr(IP_SERVIDOR);
		infoDir.sin_port = htons(PUERTO_SERVIDOR);
	}
}

SocketCliente::~SocketCliente() {
	// TODO Auto-generated destructor stub
}

