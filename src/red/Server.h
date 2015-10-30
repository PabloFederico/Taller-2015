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
#include "Proxy.h"
#include <cerrno>
#include <time.h>


struct DataCliente {
	int id;
	bool conectado;
	string nombre;
	Coordenada posProtag;

	DataCliente(): id(0), conectado(true), nombre("default"), posProtag(Coordenada(0,0)) {};
	DataCliente(int idJug, string nom, Coordenada posicionInicial):
		id(idJug), nombre(nom), posProtag(posicionInicial) {
		conectado = true;
	}
};



class Server: public Connection {
private:
	SocketServidor* socket;
	fd_set readset;
	int maxfd, cantConectados;
	map<int,DataCliente> clientes;

public:
	Server();
	bool iniciar();

	int intentarNuevaConexion(fd_set* tempset, int segundosDeEspera);
	bool mensajeParaElServidor(int idJug, string s);

	void correr();

	void finalizar();
	~Server();
};

#endif /* RED_SERVER_H_ */
