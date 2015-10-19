/*
 * Client.h
 *
 *  Created on: 15 de oct. de 2015
 *      Author: emanuel
 */

#ifndef RED_CLIENT_H_
#define RED_CLIENT_H_

#include <iostream>
#include <stdlib.h>

#include "../red/SocketCliente.h"
#include "../red/Red.h"
#include "../utils/Structs.h"

class Client {
private:
	SocketCliente* socket;

public:
	static void ejecutar();

	void iniciar();

	static Camino recibir(); //prueba con iniciar incluido
	//enviar de cada tipo

	void finalizar();
};

#endif /* RED_CLIENT_H_ */
