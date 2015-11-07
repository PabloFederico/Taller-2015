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

class ControladorServidor;

class Server: public Connection {
private:
	SocketServidor* socket;
	fd_set readset;
	int maxfd;
	Clientes clientes;

	ControladorServidor* controladorServer;

	//map<int,Thread*> hilos;

public:
	Server();
	bool iniciar();

	void inicializarCliente(int peersock, int segundosDeEspera);

	int intentarNuevaConexion(fd_set* tempset, int segundosDeEspera);
	void chequearPorNuevosClientes();
	void conexionPerdida(int j);
	bool procesarComoServidor(int idJug, string s);

	void enviarATodos(string mensaje);
	void enviarATodosMenos(int socketNoRecibe, string mensaje);

	void correr();

	void finalizar();
	~Server();
};

#endif /* RED_SERVER_H_ */
