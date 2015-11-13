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

	void esperarConexiones(int sock);


public:
	ControladorServidor(Server* server);

	bool validarLogIn(int sock);

	void generacionDeRecursosRandom();

	void mutexLock();

	void mutexUnlock();

	int fd_ISSET(int fd);

	std::string mensajeParaAvanzarJug(int fd);

	bool procesarComoServidor(int sock, std::string buffer);

	void enviarATodosMenos(int sock, std::string buffer);

	void enviarATodos(std::string buffer);

	void clienteSeDesconecto(int sock_cliente);

	void fd_clr();

	virtual ~ControladorServidor();
};

#endif /* SRC_RED_CONTROLADORSERVIDOR_H_ */
