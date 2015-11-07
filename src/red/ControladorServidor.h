/*
 * ControladorServidor.h
 *
 *  Created on: 7 de nov. de 2015
 *      Author: emanuel
 */

#ifndef SRC_RED_CONTROLADORSERVIDOR_H_
#define SRC_RED_CONTROLADORSERVIDOR_H_
#include "../red/StructsParaRed.h"
#include <pthread.h>
#include "../red/Server.h"

class ControladorServidor {
private:
	Clientes clientes;
	Server* server;
	pthread_mutex_t mutex;
	clock_t t;

	void esperarConexiones();
	void generacionDeRecursosRandom();

public:
	ControladorServidor(Server* server);

	bool validarLogIn(int sock);

	bool procesarComoServidor(int sock, std::string buffer);

	void enviarATodosMenos(int sock, std::string buffer);

	void clienteSeDesconecto(int sock_cliente);

	virtual ~ControladorServidor();
};

#endif /* SRC_RED_CONTROLADORSERVIDOR_H_ */
