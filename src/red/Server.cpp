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
	//this->Connection::finalizar();
	/* código que debería ejecutar el servidor */
	std::cout << "======= SERVIDOR =======" << std::endl;

	this->socket = new SocketServidor();

	if (this->socket->creadoCorrectamente() < 0) {
		std::cout << "ERROR: No se pudo crear socket."<<std::endl;
		return false;
	}

	/* bind() */
	if (Red::enlazarSocket(this->socket) < 0) {
		std::cout << "ERROR: bind failed."<<std::endl;
		return false;
	}

	/* listen() */
	if (Red::escucharConexiones(this->socket->getDescriptor(),MAX_CONEXIONES) < 0)
		return false;

	cantConectados = 0;
	return true;
}


bool Server::mensajeParaElServidor(int sockfd, string s) {
	bool res = true;
	TipoMensajeRed tipo = TipoMensajeRed(0);
	string unContenido;	stringstream ss(s);
	char charUnMensaje[MAX_BYTES_LECTURA];
	if (ss.peek() != '<') // Me deshago de posible basura.
		ss.get(charUnMensaje, MAX_BYTES_LECTURA, '<');
	ss.get(charUnMensaje, MAX_BYTES_LECTURA, '~');

	while (charUnMensaje[0] == '<') {
		string unMensaje(charUnMensaje);
		tipo = Proxy::extraerPrefijoYMensaje(unMensaje, &unContenido);

		switch (tipo) {
		case COMIENZO:{ map<int,DataCliente>::iterator it; bool late = false;
						do {
							ostringstream nom(unContenido);
							for (it = clientes.begin(); it != clientes.end(); ++it)
								if (it->second.nombre == nom.str())
									late = true;
							if (late) {
								nom << clientes[sockfd].id;
								late = false;
							} else {
								clientes[sockfd].nombre = nom.str();
								string enviar = Proxy::agregarPrefijoYJugYFinal("COM", clientes[sockfd].id, unContenido);
								send(sockfd, &enviar, sizeof(enviar), MSG_NOSIGNAL);
							}
						} while (late == true);
						res = true; // El mensajes es SOLO para el servidor
					} break;
		case MOVIMIENTO: { stringstream ss(unContenido);
						int jug; ss >> jug; ss.ignore(); // ':'
						char camEnc[MAX_BYTES_LECTURA];
						ss.get(camEnc, MAX_BYTES_LECTURA, '~');
						clientes[sockfd].posProtag = Camino::dec(camEnc).v.back();
						res = false;
					} break;
		case NUEVA_ENTIDAD: { // agregar correspondiente?
						res = false;
					} break;
		default:		res = false;
		}

		ss.ignore();	// '~'
		ss.get(charUnMensaje, MAX_BYTES_LECTURA, '~');
	}
	return res;
}


TipoEntidad generarRecursoYCoordRandom(Coordenada* c) {
	*c = Calculador::generarPosRandom(50,0,50,0,0);
	Coordenada aux = Calculador::generarPosRandom(ORO,MADERA,1,0,0);
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
				ostringstream ss;

				map<int,DataCliente>::iterator it = clientes.find(peersock);
				if (it != clientes.end()) { // asumo q mismo peersock, mismo cliente // PARECERÍA SERLO...
					ss << "<TOG>"<<it->second.id<<"~";
					for (int j = 0; j < maxfd+1; j++)
						if (j != peersock && FD_ISSET(j, &readset))
							send(j, ss.str().c_str(), 10, MSG_NOSIGNAL);
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
						// Lo recibido de un socket, mandarlo a todos los demás
						for (int k = 0; k < maxfd+1; k++) {
							if ((k != j) && FD_ISSET(k, &readset)) {
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
	std::cout << "====== /SERVIDOR/ ======" << std::endl;
}
