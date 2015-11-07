/*
 * ControladorConexion.h
 *
 *  Created on: 7 de nov. de 2015
 *      Author: emanuel
 */

#ifndef SRC_RED_CONTROLADORCONEXION_H_
#define SRC_RED_CONTROLADORCONEXION_H_
#include "../utils/Thread.h"
#include "../red/ControladorServidor.h"

class ControladorConexion: public Thread {
private:
	ControladorServidor* controladorServidor;
	int sock_cliente;
public:
	ControladorConexion(ControladorServidor* controlador, int sock_cliente);
	void* run();
	virtual ~ControladorConexion();
};

#endif /* SRC_RED_CONTROLADORCONEXION_H_ */
