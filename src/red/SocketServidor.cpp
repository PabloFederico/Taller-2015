/*
 * SocketServidor.cpp
 *
 *  Created on: 15 de oct. de 2015
 *      Author: emanuel
 */

#include "../red/SocketServidor.h"

SocketServidor::SocketServidor(int puerto):Socket() {
	if (descriptor > -1){
		infoDir.sin_family = AF_INET;
		infoDir.sin_addr.s_addr = INADDR_ANY;
		infoDir.sin_port = htons(puerto);
	}
}

SocketServidor::~SocketServidor() {
	// TODO Auto-generated destructor stub
}

