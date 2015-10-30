/*
 * Server.cpp
 *
 *  Created on: 15 de oct. de 2015
 *      Author: emanuel
 */

#include "../red/Server.h"


Server::Server() {
	std::cout << "======= SERVIDOR =======" << std::endl;
	if (!iniciar()) {
		throw ConnectionProblem();
	}
}

bool Server::iniciar() {
	this->socket = new SocketServidor();
	if (this->socket->creadoCorrectamente() < 0) {
		std::cout << "ERROR: No se pudo crear socket."<<std::endl;
		return false;
	}
	if (Red::enlazarSocket(this->socket) < 0) {
		std::cout << "ERROR: bind failed."<<std::endl;
		return false;
	}
	if (Red::escucharConexiones(this->socket->getDescriptor(),MAX_CONEXIONES) < 0)
		return false;
	cantConectados = 0;
	return true;
}


bool Server::mensajeParaElServidor(int sockfd, string recibido) {
	bool rebotarlo = true;
	string unMensaje, unContenido;
	TipoMensajeRed tipo;
	while (Red::parsearSiguienteMensaje(&recibido, &tipo, &unContenido)) {

		switch (tipo) {
		case COMIENZO: {
				ostringstream nom(unContenido);
				bool late;
				do {
					late = false;
					for (map<int,DataCliente>::iterator it = clientes.begin(); it != clientes.end(); ++it)
						if (it->second.nombre == nom.str() && it->first != sockfd)
							late = true;
					if (late) // Si el nombre está repetido, se le agrega su número de jugador hasta que no lo esté. (Jorge222)
						nom << clientes[sockfd].id;
				} while (late == true);
				if (nom.str() != unContenido) {
					string enviar = Red::agregarPrefijoYJugYFinal("COM", clientes[sockfd].id, unContenido);
					send(sockfd, &enviar, sizeof(enviar), MSG_NOSIGNAL);
				}
				clientes[sockfd].nombre = nom.str(); // Guardar nuevo nombre.
				rebotarlo = false; // El mensaje era SOLO para el servidor
			} break;

		case MOVIMIENTO: {
				string camEnc;
				Red::extraerNumeroYResto(unContenido, &camEnc);
				clientes[sockfd].posProtag = Camino::dec(camEnc).v.back();
				rebotarlo = true;
			} break;

		case NUEVA_ENTIDAD: { // agregar correspondiente?
				rebotarlo = true;
			} break;

		//MENSAJE, ESCENARIO, RECURSO, TOGGLE, ATAQUE, GLOTON, FIN
		default: rebotarlo = true;
		}
	}
	return rebotarlo;
}


TipoEntidad generarRecursoYCoordRandom(Coordenada* c) {
	*c = Calculador::generarPosRandom(50,0,50,0,7);
	Coordenada aux = Calculador::generarPosRandom(ORO+1,MADERA,1,0,42);
	return TipoEntidad(aux.x);	   //último recurso^	  ^primer recurso
}


int Server::intentarNuevaConexion(fd_set* p_tempset, int segundosDeEspera) {
	int result, srvsock, peersock;
	sockaddr_in* p_addr = this->socket->getpInfoDir();
	srvsock = this->socket->getDescriptor();
	timeval tv;
	tv.tv_sec = segundosDeEspera;
	tv.tv_usec = 0;

	result = select(maxfd+1, p_tempset, NULL, NULL, &tv);

	if (result < 0 && errno != EINTR) {
		std::cout << "Error in select(): "<<strerror(errno)<<std::endl;
	} else if ((result > 0) && (FD_ISSET(srvsock, p_tempset))) {
		unsigned int len = sizeof(*p_addr);

		peersock = accept(srvsock, (sockaddr*)p_addr, &len);

		if (peersock < 0) {
			std::cout << "Error in accept(): "<<strerror(errno)<<std::endl;
			return -1;
		}

		map<int,DataCliente>::iterator it;
		ostringstream ss; string nombreJug, mensaje;
		char buffer[MAX_BYTES_LECTURA];
		cantConectados++;

		// Primer mensaje: Recibo nombre de jugador pedido.
		recv(peersock, buffer, sizeof(buffer), 0);
		Red::extraerPrefijoYMensaje(buffer, &nombreJug);
		for (it = clientes.begin(); it != clientes.end(); ++it)
			if ((it->second.nombre == nombreJug) && (!it->second.conectado))
				break;

		// Si es un jugador previo...
		if (it != clientes.end()) {
			it->second.conectado = true;

			// Segundo mensaje: Envío al jugador su anterior número de jugador.
			ss << it->second.id<<"~";
			send(peersock, ss.str().c_str(), 10, MSG_NOSIGNAL);
			ss.str( std::string() ); ss.clear();

			// // Paralelo: Envío a todos los demás jugadores que este volvió.
			ss << "<TOG>"<<it->second.id<<"~";
			for (int j = 0; j < maxfd+1; j++)
				if (FD_ISSET(j, &readset) && j != peersock)
					send(j, ss.str().c_str(), 10, MSG_NOSIGNAL);
			ss.str( std::string() ); ss.clear();

			// Tercer mensaje: Envío al jugador señal de comienzo y su última posición.
			mensaje = Red::agregarPrefijoYFinal("COM", it->second.posProtag.enc());
			send(peersock, mensaje.c_str(), MAX_BYTES_LECTURA, MSG_NOSIGNAL);

		// Si es un jugador nuevo...
		} else {
			bool late;
			Coordenada coordRandom;
			do {
				// Adaptable a secciones del mapa basado en MAX_CONEXIONES y/o cantConectados
				coordRandom = Calculador::generarPosRandom(50,0, 50,0, cantConectados);
				late = false;									// buen hardcodeo
				for (map<int,DataCliente>::iterator it = clientes.begin(); it != clientes.end(); ++it)
					if (it->second.posProtag == coordRandom)
						late = true;
			} while (late == true);

			// Registro nuevo jugador.
			int idJug = clientes.size() + 1;
			DataCliente infoCliente = DataCliente(idJug, nombreJug, coordRandom);
			clientes.insert(pair<int,DataCliente>(peersock,infoCliente));

			// Segundo mensaje: Envío número de jugador.
			ss << idJug<<"~";
			send(peersock, ss.str().c_str(), 5, MSG_NOSIGNAL);

			// Tercer mensaje: Si el juego ya ha comenzado, enviar señal de comienzo y nueva posición inmediatamente.
			if (segundosDeEspera < 3) {
				mensaje = Red::agregarPrefijoYFinal("COM", coordRandom.enc());
				send(peersock, mensaje.c_str(), 16, MSG_NOSIGNAL);
			}
		}

		// Más mensajes: Si el juego ha comenzado, envío al jugador las posiciones de todos los demás.
		if (segundosDeEspera < 3) {
			for (it = clientes.begin(); it != clientes.end(); ++it)
				if ((it->second.conectado) && (it->second.nombre != nombreJug)) {
					Entidad entAux = Entidad(SOLDADO, it->second.id);
					PosEntidad peAux(it->second.posProtag, &entAux);
					ss << Red::agregarPrefijoYFinal("ENT", peAux.enc());
				}
			send(peersock, mensaje.c_str(), MAX_BYTES_LECTURA, MSG_NOSIGNAL);
		}

		fcntl(peersock, F_SETFL, O_NONBLOCK); // non-blocking mode
		FD_SET(peersock, &readset);
		maxfd = (maxfd > peersock)?maxfd:peersock;
		std::cout << "Jugador "<<clientes[peersock].id<<" conectado!"<<std::endl;
		return peersock;
	}
	return -1;
}



void Server::correr() {
	fd_set tempset;
	string mensaje;
	char buffer[MAX_BYTES_LECTURA+1];
	int j, k, srvsock = this->socket->getDescriptor();

	FD_ZERO(&readset);
	FD_SET(srvsock, &readset);
	maxfd = srvsock;

	std::cout << "Aceptando hasta "<<MAX_CONEXIONES<<" jugadores ("<<MAX_ESPERA_CONEXION<<" s para conectarse)."<<std::endl;

	// Cantidad de conexiones permitidas / intentos.
	for (int i = 0; i < MAX_CONEXIONES; i++) {
		std::cout << "#"<<i+1<<" ... ";
		memcpy(&tempset, &readset, sizeof(tempset));
		// Espera MAX_ESPERA_CONEXION segundos o hasta que aparezca una conexión.
		intentarNuevaConexion(&tempset, MAX_ESPERA_CONEXION);
	}
	memcpy(&tempset, &readset, sizeof(tempset));
	FD_CLR(srvsock, &readset);

	if (cantConectados < 2) {
		std::cout << "No se conectaron suficientes jugadores para una partida en red."<<std::endl;
		std::cout << "Hazte algunos amigos y vuelve a intentarlo.";
		sleep(1); std::cout << "."; sleep(1); std::cout << "."; sleep(1); std::cout << ".";
		sleep(1); std::cout << "."; sleep(1); std::cout << std::endl;
		return;
	}


	std::cout << std::endl << "Se recibieron "<<cantConectados<<" conexiones."<<std::endl;

	// Enviar señal de comienzo junto a posición inicial.
	for (j = 0; j < maxfd+1; j++) {
		if (FD_ISSET(j, &readset)) {
			mensaje = Red::agregarPrefijoYFinal("COM", clientes[j].posProtag.enc());
			send(j, mensaje.c_str(), 16, MSG_NOSIGNAL);
		}	//NO FUNCIONANDO PARA EL MAX_CONEXIONES-ésimo!?
	}


	std::cout << "Comenzando juego..."<<std::endl << std::endl;

	int result, sent, justsent;
	clock_t t = clock();

	/******** LOOP PRINCIPAL ********/
	while (cantConectados > 0) {

		// Por cada cliente conectado...
		for (j = 0; j < maxfd+1; j++) {
			if (FD_ISSET(j, &readset)) {

				do {
					result = recv(j, buffer, MAX_BYTES_LECTURA, 0);
				} while (errno == EINTR && result == -1);

				if (result > 0) {
					buffer[result] = 0;
					if (mensajeParaElServidor(j, string(buffer))) {
						// Lo recibido de un cliente, si no es únicamente para el servidor, mandarlo a todos los demás
						std::cout << "Echoing: "<<buffer<<std::endl;
						string aux;
						for (k = 0; k < maxfd+1; k++) {
							if (FD_ISSET(k, &readset) && ((k != j)||(Red::extraerPrefijoYMensaje(buffer,&aux)==MOVIMIENTO))) {
								sent = 0;				//semi hardcodeo para el mensaje de movimiento; a cambiar cuando vectorialice;//
								do {
									justsent = send(k, buffer+sent, result-sent, MSG_NOSIGNAL);
									if (justsent > 0)
										sent += justsent;
									else if (justsent < 0 && errno != EINTR)
										break; //Podría llegar a enviarse solo parte de la data?...
								} while (sent < result);
							}
						}
					}

				// Si el jugador se ha desconectado... ("The return value will be 0 when the peer has performed an orderly shutdown.")
				} else if (result == 0) {
					close(j);
					FD_CLR(j, &readset);

					//Avisarle a los demás.
					ostringstream encode; encode << "<TOG>"<<clientes[j].id<<"~";
					strcpy(buffer, encode.str().c_str());
					for (k = 0; k < maxfd+1; k++)
						if (FD_ISSET(j, &readset))
							send(k, buffer, sizeof(buffer), MSG_NOSIGNAL);

					clientes[j].conectado = false;
					std::cout << clientes[j].nombre<<" (jugador "<<clientes[j].id<<") se ha desconectado."<<std::endl;
					cantConectados--;

				} else if (errno != EWOULDBLOCK)
					std::cout << "Error in recv(): "<<strerror(errno)<<std::endl;

			} // fi (FD_ISSET(j, &readset))
		} // rof cada cliente

		// Generación de recursos random
		if ((clock() - t) > CLOCKS_PER_SEC*DELAY_RECURSOS) {
			Coordenada c; ostringstream ss;
			TipoEntidad tipoRecurso = generarRecursoYCoordRandom(&c);
			ss << "<REC>"<<tipoRecurso<<","<<c.enc()<<"~";
			//std::cout << "Recurso "<<mensaje<<std::endl;//
			for (int j = 0; j < maxfd+1; j++)
				if (FD_ISSET(j, &readset))
					send(j, ss.str().c_str(), 20, MSG_NOSIGNAL);
			t = clock();
		}

		// Intenta (re)conectar [más] jugadores...
		if (cantConectados < MAX_CONEXIONES) {
			FD_SET(srvsock, &readset);
			memcpy(&tempset, &readset, sizeof(tempset));
			intentarNuevaConexion(&tempset, 0);
			memcpy(&tempset, &readset, sizeof(tempset));
			FD_CLR(srvsock, &readset);
		}

	} // end while

	std::cout << std::endl << "Se han desconectado todos los jugadores."<<std::endl<<"Fin de la partida."<<std::endl;
	sleep(4);
}



void Server::finalizar() {
	int fd = this->socket->getDescriptor();
	Connection::finalizar();
	this->socket->cerrarSocket(fd);
}

Server::~Server() {
	// El destructor de Connection se ocupa de llamar a finalizar
	std::cout << "====== /SERVIDOR/ ======" << std::endl;
}
