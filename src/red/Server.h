/*
 * Server.h
 *
 *  Created on: 15 de oct. de 2015
 *      Author: emanuel
 */

#ifndef RED_SERVER_H_
#define RED_SERVER_H_

#include <iostream>
#include <stdlib.h>

#include "../red/SocketServidor.h"
#include "../red/Red.h"
#include "../utils/Structs.h"
#include "../modelo/Log.h"

class Server {
private:
	SocketServidor* socket;
	int lastDescriptor;
public:
	static void ejecutar();
	void iniciar();

	static void enviar(Camino cam); //prueba con iniciar incluido
	//enviar de cada tipo

	void finalizar();

};

#endif /* RED_SERVER_H_ */
