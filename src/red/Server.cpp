/*
 * Server.cpp
 *
 *  Created on: 15 de oct. de 2015
 *      Author: emanuel
 */

#include "../red/Server.h"


Server::Server() {
	if (!iniciar()) {
		throw ConnectionProblem();
	}
}

bool Server::iniciar() {
	std::cout << "======= SERVIDOR =======" << std::endl;
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
				bool late;
				do {
					late = false;
					ostringstream nom(unContenido);
					for (map<int,DataCliente>::iterator it = clientes.begin(); it != clientes.end(); ++it)
						if (it->second.nombre == nom.str())
							late = true;
					if (late) {
						// Si el nombre está repetido, se le agrega su número de jugador hasta que no lo esté. (Jorge222)
						nom << clientes[sockfd].id;
					} else {
						clientes[sockfd].nombre = nom.str();
						if (nom.str() != unContenido) {
							string enviar = Red::agregarPrefijoYJugYFinal("COM", clientes[sockfd].id, unContenido);
							send(sockfd, &enviar, sizeof(enviar), MSG_NOSIGNAL);
						}
					}
				} while (late == true);

				rebotarlo = false; // El mensaje es SOLO para el servidor
			} break;

		case MOVIMIENTO: {
				string camEnc;
				Red::extraerNumeroYResto(unContenido, &camEnc);
				clientes[sockfd].posProtag = Camino::dec(camEnc).v.back();
			} break;

		case NUEVA_ENTIDAD: { // agregar correspondiente?
			} break;

		//MENSAJE, ESCENARIO, RECURSO, TOGGLE, ATAQUE, GLOTON, FIN
		default: break;
		}
	}
	return rebotarlo;
}


TipoEntidad generarRecursoYCoordRandom(Coordenada* c) {
	*c = Calculador::generarPosRandom(50,0,50,0,0);
	Coordenada aux = Calculador::generarPosRandom(ORO+1,MADERA,1,0,42);
	return TipoEntidad(aux.x);	// último recurso, primer recurso
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
	} else if (result > 0) {
		if (FD_ISSET(srvsock, p_tempset)) {
			unsigned int len = sizeof(*p_addr);
			peersock = accept(srvsock, (sockaddr*)p_addr, &len);
			if (peersock < 0) {
				std::cout << "Error in accept(): "<<strerror(errno)<<std::endl;
			} else {
				cantConectados++;

				ostringstream ss; string nombreJug;
				char buffer[MAX_BYTES_LECTURA];
				recv(peersock, buffer, sizeof(buffer), 0);
				Red::extraerPrefijoYMensaje(buffer, &nombreJug);
				map<int,DataCliente>::iterator it;// = clientes.find(peersock);
				for (it = clientes.begin(); it != clientes.end(); ++it)
					if ((it->second.nombre == nombreJug) && (!it->second.conectado))
						break;
				if (it != clientes.end()) {
					ss << it->second.id<<"~";
					it->second.conectado = true;
					send(peersock, ss.str().c_str(), 10, MSG_NOSIGNAL);
					//ss.str( std::string() ); ss.clear();
					//ss << "<COM>"<<it->second.nombre<<"~";
					//send(peersock, ss.str().c_str(), 10, MSG_NOSIGNAL);
					ss.str( std::string() ); ss.clear();
					ss << "<TOG>"<<it->second.id<<"~";
					for (int j = 0; j < maxfd+1; j++)
						if (j != peersock && FD_ISSET(j, &readset))
							send(j, ss.str().c_str(), 10, MSG_NOSIGNAL);
					ss.str( std::string() ); ss.clear();
					//sleep(1);
					ss << "<COM>"<<it->second.posProtag.enc()<<"~";
					// Le recuerdo su posición inicial.
					send(peersock, ss.str().c_str(), MAX_BYTES_LECTURA, MSG_NOSIGNAL);
					ss.str( std::string() ); ss.clear();
					//sleep(5);
					for (it = clientes.begin(); it != clientes.end(); ++it)
						if ((it->second.nombre != nombreJug) && (it->second.conectado)) {
							ss.str( std::string() ); ss.clear();
							ss << "<ENT>"<<it->second.posProtag.enc()<<"["<<it->second.id<<","<<SOLDADO<<","<<1<<","<<1<<"]"<<"~";
							send(peersock, ss.str().c_str(), MAX_BYTES_LECTURA, MSG_NOSIGNAL);
						}
				} else {
					string nomTemp = "Jugador"+cantConectados;	// alto hardcodeo
					// Adaptable a secciones del mapa basado en MAX_CONEXIONES y cantConectados
					// Caso contrario, aunque improbable, deberííía verificarse que no sea una posición repetida
					Coordenada coordRandom = Calculador::generarPosRandom(50,0,50,0, cantConectados);
					//std::cout << "Pos inicial para jugador "<<cantConectados<<" es: "<<coordRandom.x<<";"<<coordRandom.y<<std::endl;//
					DataCliente cli = DataCliente(cantConectados, nomTemp, coordRandom);
					clientes.insert(pair<int,DataCliente>(peersock,cli));

					ss << cantConectados<<"~";
					// Envío # de jugador.
					send(peersock, ss.str().c_str(), 5, MSG_NOSIGNAL);
				}

				fcntl(peersock, F_SETFL, O_NONBLOCK); // non-blocking mode
				FD_SET(peersock, &readset);
				maxfd = (maxfd > peersock)?maxfd:peersock;
				std::cout << "Jugador "<<cantConectados<<" conectado!"<<std::endl;
				return peersock;
			}
		}
	}
	return -1;
}



void Server::correr() {
	char buffer[MAX_BYTES_LECTURA+1];
	fd_set tempset;
	int srvsock = this->socket->getDescriptor();

	FD_ZERO(&readset);
	FD_SET(srvsock, &readset);
	maxfd = srvsock;

	std::cout << "Aceptando hasta "<<MAX_CONEXIONES<<" jugadores."<<std::endl;
	//std::cout << "Cada jugador tiene "<<MAX_ESPERA_CONEXION<<" segundos para conectarse."<<std::endl;

	// Cantidad de conexiones permitidas / intentos.
	for (int i = 0; i < MAX_CONEXIONES; i++) {
		std::cout << "#"<<i+1<<" ... ";
		memcpy(&tempset, &readset, sizeof(tempset));
		// Espera MAX_ESPERA_CONEXION segundos a que aparezca una conexión.
		intentarNuevaConexion(&tempset, MAX_ESPERA_CONEXION);
	}
	memcpy(&tempset, &readset, sizeof(tempset));
	FD_CLR(srvsock, &readset);

	if (cantConectados < 2) {
		std::cout << "No se conectaron suficientes jugadores para una partida en red."<<std::endl;
		std::cout << "Hazte algunos amigos y vuelve a intentarlo..."<<std::endl;
		sleep(5);
		return;
	}


	std::cout << std::endl<<"Se recibieron "<<cantConectados<<" conexiones."<<std::endl;

	for (int j = 0; j < maxfd+1; j++) {
		if (FD_ISSET(j, &readset)) {
			ostringstream encode;
			encode << "<COM>"<<clientes[j].posProtag.enc()<<"~";
			send(j, encode.str().c_str(), 16, MSG_NOSIGNAL);
		}	// NO FUNCIONANDO PARA EL MAX_CONEXIONES-ésimo!
	}

	std::cout << "Comenzando juego..."<<CLOCKS_PER_SEC<<std::endl<<std::endl;
	sleep(1);

	clock_t t = clock();
	int result, sent, justsent;
	while (cantConectados > 0) {

		// Generación de recursos random
		//if (int(1000000*((float)t/CLOCKS_PER_SEC)) % DELAY_RECURSOS == 0) {
		if ((clock()-t) > CLOCKS_PER_SEC*DELAY_RECURSOS) {
			t = clock();
			Coordenada c; ostringstream encode;
			TipoEntidad tipoRecurso = generarRecursoYCoordRandom(&c);
			encode << "<REC>"<<tipoRecurso<<","<<c.enc()<<"~";
			//std::cout << "Recurso "<<encode.str()<<std::endl;//
			for (int j = 0; j < maxfd+1; j++)
				if (FD_ISSET(j, &readset))
					send(j, encode.str().c_str(), 20, MSG_NOSIGNAL);
		}

		for (int j = 0; j < maxfd+1; j++) {
			if (FD_ISSET(j, &readset)) {

				do {
					result = recv(j, buffer, MAX_BYTES_LECTURA, 0);
				} while (result == -1 && errno == EINTR);

				if (result > 0) {
					buffer[result] = 0;
					if (!mensajeParaElServidor(j, string(buffer))) {
						std::cout << "Echoing: "<<buffer<<std::endl;
						string aux;
						// Lo recibido de un socket, mandarlo a todos los demás
						for (int k = 0; k < maxfd+1; k++) {
							if (((k != j)||(Red::extraerPrefijoYMensaje(buffer,&aux)==MOVIMIENTO)) && FD_ISSET(k, &readset)) {
								sent = 0;
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

				} else if (result == 0/* && errno != EWOULDBLOCK*/) {
					close(j);
					FD_CLR(j, &readset);
					std::cout << "Un jugador (socket "<<j<<") se ha desconectado."<<std::endl;
					ostringstream encode; encode << "<TOG>"<<clientes[j].id<<"~";
					strcpy(buffer, encode.str().c_str());
					for (int j = 0; j < maxfd+1; j++) {
						if (FD_ISSET(j, &readset)) {
							send(j, buffer, sizeof(buffer), MSG_NOSIGNAL);
						}
					}
					clientes[j].conectado = false;
					cantConectados--;
				} else if (errno != EWOULDBLOCK) {
					std::cout << "Error in recv(): "<<strerror(errno)<<std::endl;
				}
			} // end if (FD_ISSET(j, &readset))
		} // rof

		// Intenta (re)conectar (más) jugadores...
		if (cantConectados < MAX_CONEXIONES) {
			FD_SET(srvsock, &readset);
			memcpy(&tempset, &readset, sizeof(tempset));
			int peersock = intentarNuevaConexion(&tempset, 0);
			memcpy(&tempset, &readset, sizeof(tempset));
			FD_CLR(srvsock, &readset);
			if (peersock!=-1) {
				ostringstream encode;
				encode << "<COM>"<<clientes[peersock].posProtag.enc()<<"~";
				send(peersock, encode.str().c_str(), 16, MSG_NOSIGNAL);
			}
		}
	} // end while

	std::cout << std::endl << "Se han desconectado todos los jugadores. Fin de la partida."<<std::endl;
	sleep(4);
}


void Server::finalizar() {
	int fd = this->socket->getDescriptor();
	Connection::finalizar();
	this->socket->cerrarSocket(fd);
}

Server::~Server() {
	// Connection ya llama a finalizar
	std::cout << "====== /SERVIDOR/ ======" << std::endl;
}
