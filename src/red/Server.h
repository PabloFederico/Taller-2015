/*
 * Server.h
 *
 *  Created on: 15 de oct. de 2015
 *      Author: emanuel
 */

#ifndef RED_SERVER_H_
#define RED_SERVER_H_

#include "Connection.h"
#include "../utils/Calculador.h"
#include "../utils/Constantes.h"
#include "../red/StructsParaRed.h"
#include "Proxy.h"
#include <cerrno>
#include <time.h>
#include <SDL2/SDL.h>
#include "../utils/Thread.h"

class ControladorServidor;

class Server: public Connection {
private:
	SocketServidor* socket;
	fd_set readset;
	int maxfd;
	Clientes clientes;

	ControladorServidor* controladorServer;

	vector<Thread*> hilos;

public:
	Server();
	bool iniciar();

	void fd_clr();

	void inicializarCliente(int peersock, int segundosDeEspera);

	int intentarNuevaConexion(fd_set* tempset, int segundosDeEspera);
	void chequearPorNuevosClientes();
	void conexionPerdida(int j);
	bool procesarComoServidor(int idJug, string s);

	void enviarATodos(string mensaje);
	void enviarATodosMenos(int socketNoRecibe, string mensaje);

	void correr();

	int fd_ISSET(int sock);

	void finalizar();
	~Server();
};

#endif /* RED_SERVER_H_ */
