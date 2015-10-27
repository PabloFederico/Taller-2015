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

	return true;
}


void Server::correr() {

	fd_set readset, tempset;
	int srvsock, maxfd, result, peersock, sent, justsent, cantConectados = 0;
	unsigned int len;
	timeval tv;
	char buffer[MAX_BYTES_LECTURA+1];
	sockaddr_in* p_addr = this->socket->getpInfoDir();

	srvsock = this->socket->getDescriptor();
	FD_ZERO(&readset);
	FD_SET(srvsock, &readset);
	maxfd = srvsock;

	std::cout << "Aceptando hasta "<<MAX_CONEXIONES<<" jugadores."<<std::endl;
	std::cout << "Cada jugador tiene 20 segundos para conectarse."<<std::endl;

	for (int i = 0; i < MAX_CONEXIONES; i++) {
		std::cout << "#"<<i+1<<" ... ";
		memcpy(&tempset, &readset, sizeof(tempset));
		tv.tv_sec = 20;//
		tv.tv_usec = 0;
		// Espera 30 segundos a aparezca una conexión.
		result = select(maxfd+1, &tempset, NULL, NULL, &tv);

		if (result == 0) {
			std::cout << "timed out!"<<std::endl;
		} else if (result < 0 && errno != EINTR) {
			std::cout << "Error in select(): "<<strerror(errno)<<std::endl;
		} else if (result > 0) {

			if (FD_ISSET(srvsock, &tempset)) {
				len = sizeof(*p_addr);
				peersock = accept(srvsock, (sockaddr*)p_addr, &len);
				if (peersock < 0) {
					std::cout << "Error in accept(): "<<strerror(errno)<<std::endl;
				} else {
					cantConectados++;

					ostringstream ss;
					ss << cantConectados<<"~";
					send(peersock, ss.str().c_str(), 5, MSG_NOSIGNAL);
					// Envío # de jugador.

					fcntl(peersock, F_SETFL, O_NONBLOCK); // non-blocking mode
					FD_SET(peersock, &readset);
					maxfd = (maxfd > peersock)?maxfd:peersock;
					std::cout << "Jugador "<<cantConectados<<" conectado!"<<std::endl;
				}
			}
		}
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
			strcpy(buffer, "<COM>~");
			sent = 0;
			do {
				std::cout << "&&" << sizeof(buffer) << buffer << std::endl;//
				justsent = send(j, buffer+sent, sizeof(buffer)-sent, MSG_NOSIGNAL);
				if (justsent > 0)
					sent += justsent;
				else if (justsent < 0 && errno != EINTR)
					break; //Podría llegar a enviarse solo parte de la data?...
			} while (sent < result);
			send(j, buffer, sizeof(buffer), MSG_NOSIGNAL);
		}
	}

	std::cout << "Comenzando juego..."<<std::endl<<std::endl;

	while (cantConectados > 0) {
		for (int j = 0; j < maxfd+1; j++) {
			if (FD_ISSET(j, &readset)) {

				do {
					result = recv(j, buffer, MAX_BYTES_LECTURA, 0);
				} while (result == -1 && errno == EINTR);

				if (result > 0) {

					buffer[result] = 0;
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

				} else if (result == 0/* && errno != EWOULDBLOCK*/) {
					close(j);
					FD_CLR(j, &readset);
					std::cout << "Un jugador (socket "<<j<<") se ha desconectado."<<std::endl;
					cantConectados--;
				} else if (errno != EWOULDBLOCK) {
					std::cout << "Error in recv(): "<<strerror(errno)<<std::endl;
				}
			} // end if (FD_ISSET(j, &readset))
		} // rof
	} // end while

	std::cout << std::endl << "Se han desconectado todos los jugadores. Fin de la partida."<<std::endl;
	sleep(5);
}


void Server::finalizar() {
	int fd = this->socket->getDescriptor();
	Connection::finalizar();
	this->socket->cerrarSocket(fd);
}

Server::~Server() {
	std::cout << "====== /SERVIDOR/ ======" << std::endl;
}
