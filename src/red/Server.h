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
	Entidad entidad;
	Coordenada posProtag;
	Camino camino;
	int completaronPaso;

	DataCliente(): id(0), conectado(true), nombre("default"),
			entidad(Entidad(SOLDADO,0)), posProtag(Coordenada(0,0)), completaronPaso(MAX_CONEXIONES) {};

	DataCliente(int idJug, string nom, Coordenada posicionInicial, Entidad ent):
		id(idJug), nombre(nom), entidad(ent), posProtag(posicionInicial) {
		conectado = true;
		completaronPaso = MAX_CONEXIONES;
	}

	void setCamino(Camino cam) {
		quieto();
		while (!cam.empty())
			camino.agregar(cam.sacarProximaCoordenada());
		posProtag = cam.ultimo();
	}
	Coordenada avanzar(int cantConectados) {
		if (camino.empty() || completaronPaso < cantConectados)
			throw CaminoVacio();
		completaronPaso = 0;
		return camino.sacarProximaCoordenada();
	}
	void alguienDioElPaso() {
		completaronPaso += 1;
	}
	void quieto() {
		camino.borrar();
		completaronPaso = MAX_CONEXIONES;
	}
};

struct Clientes {
	map<int,DataCliente> m;
	int cantConectados;

	Clientes(): cantConectados(0) {}

	int agregar(int socket, string nombre, Entidad* pEntidad = NULL) {
		int idJug = cant()+1;
		Entidad entidad = Entidad(SOLDADO, idJug);
		if (pEntidad != NULL)
			 entidad = *pEntidad;

		// Genera una coordenada aleatoria no repetida.
		Coordenada coordRandom = Calculador::generarPosRandom(50,0, 50,0, 9); //hardcodeo
		for (map<int,DataCliente>::iterator it = m.begin(); it != m.end(); ++it)
			if (it->second.posProtag == coordRandom) {
				coordRandom = Calculador::generarPosRandom(50,0, 50,0, 2); //hardcodeo
				it = m.begin();
			}
		m.insert(pair<int,DataCliente>(socket, DataCliente(idJug, nombre, coordRandom, entidad)));

		cantConectados++;
		return idJug;
	}

	int cant() { return m.size(); }

	DataCliente getJugadorDesconectadoLlamado(string nombreBuscado) {
		for (map<int,DataCliente>::iterator it = m.begin(); it != m.end(); ++it)
			if ((it->second.nombre == nombreBuscado) && (!it->second.conectado))
				return (it->second);
		throw NoExiste();
	}
	DataCliente& operator[](int k) { return m[k]; }

	void jugadorConectado(int socket) {
		m[socket].conectado = true;
		cantConectados++;
	}
	void jugadorDesconectado(int socket) {
		m[socket].conectado = false;
		m[socket].quieto();
		cantConectados--;
	}

	string mensajeDeEntidadDeJugador(int socket) {
		return Red::agregarPrefijoYFinal("ENT", PosEntidad(m[socket].posProtag,&m[socket].entidad).enc());
	}

	string mensajeDeTodasLasEntidadesConectadas() {
		ostringstream msj;
		for (map<int,DataCliente>::iterator it = m.begin(); it != m.end(); ++it)
			if (it->second.conectado)
				msj << mensajeDeEntidadDeJugador(it->first);
		return msj.str();
	}

	string nuevoNombrePara(int socket, string nombrePedido) {
		ostringstream nom(nombrePedido);
		for (map<int,DataCliente>::iterator it = m.begin(); it != m.end(); ++it)
			if (it->second.nombre == nom.str() && it->first != socket) {
				nom << nom.str()<<m[socket].id; // Si el nombre está repetido, se le agrega
				it = m.begin(); // su número de jugador hasta que no lo esté. (ej.: Jorge222)
			}
		m[socket].nombre = nom.str();
		return nom.str();
	}

	void alguienCompletoPasoDe(int numJug) {
		for (map<int,DataCliente>::iterator it = m.begin(); it != m.end(); ++it)
			if (it->second.id == numJug) {
				it->second.alguienDioElPaso(); // Aumenta la cantidad de gente que completó un determinado paso.
				break;
			}
	}

	string mensajeParaAvanzarJug(int socket) {
		return Red::agregarPrefijoYJugYFinal("MOV", m[socket].id, m[socket].avanzar(cantConectados).enc());
	}
};



class Server: public Connection {
private:
	SocketServidor* socket;
	fd_set readset;
	int maxfd;
	Clientes clientes;

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
