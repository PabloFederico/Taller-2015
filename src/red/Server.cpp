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
		return false; // No hubo conexiones.

//	std::cout << "30 segundos para conectarse..."<<std::endl;
//	sleep(30); // necesitaría threads o algo así si no.
//	std::cout << "waiting for incoming connections..."<<std::endl;

	std::cout << "waiting for incoming connections..."<<std::endl;

	/* accept() */
	int new_descriptor = Red::aceptarCliente(this->socket);

	if (new_descriptor < 0) {
		std::cout << "ERROR: accept failed."<<std::endl;
		return false;
	}
//	fcntl(new_descriptor, F_SETFL, O_NONBLOCK); // non-blocking mode

	this->lastDescriptor = new_descriptor;
	std::cout << "Connected."<<std::endl;

/*	int i;
	for (i = 0; i < MAX_CONEXIONES; i++) {
		int new_descriptor = Red::aceptarCliente(this->socket);

		if (new_descriptor < 0){
			//return false;
			if (i == 0)		// Si no hubo ninguna conexión lograda, falló.
				return false;
			break;
		}
//		fcntl(new_descriptor, F_SETFL, O_NONBLOCK); // non-blocking mode

		//this->descriptors.(new_descriptor); Agregar descriptors
		this->lastDescriptor = new_descriptor;
		std::cout << "Connected to "<<new_descriptor<<"."<<std::endl;
	}
	std::cout << "Se recibieron "<<i<<" conexiones."<<std::endl;
*/	return true;
}


void Server::correr() {
//	while (true) {		// Para algo así, necesitamos threads.
//	for (vector<int>::iterator i = descriptors.begin(); i < descriptors.end(); ++i) {
//		int descriptor = *i;
//		char buf[MAX_BYTES_LECTURA];
//		if (!(Red::recibirInformacion(descriptor, buf) < 0)) {
//			std::cout << "Reenviando:"<<std::endl <<buf<<std::endl <<"-----"<<std::endl;//
//			if (Red::enviarInformacion(descriptor, buf) < 0)
//				std::cout << "ERROR: sending failed."<<std::endl;
//			else
//				std::cout << "Message sent"<<std::endl;//
//		}
//	}

	fd_set readset, tempset;
	int srvsock, maxfd, result, peersock, sent, justsent;
	unsigned int len;
	timeval tv;
	char buffer[MAX_BYTES_LECTURA+1];
	sockaddr_in* p_addr = this->socket->getpInfoDir();

	srvsock = this->socket->getDescriptor();
	FD_ZERO(&readset);
	FD_SET(srvsock, &readset);
	maxfd = srvsock;

	for (int i = 0; i < MAX_CONEXIONES; i++) {
		memcpy(&tempset, &readset, sizeof(tempset));
		tv.tv_sec = 30;
		tv.tv_usec = 0;
		// Espera 30 segundos a aparezca una conexión.
		result = select(maxfd+1, &tempset, NULL, NULL, &tv);

		if (result == 0) {
			std::cout << "select() timed out!"<<std::endl;
		} else if (result < 0 && errno != EINTR) {
			std::cout << "Error in select(): "<<strerror(errno)<<std::endl;
		} else if (result > 0) {

			if (FD_ISSET(srvsock, &tempset)) {
				len = sizeof(*p_addr);
				peersock = accept(srvsock, (sockaddr*)p_addr, &len);
				if (peersock < 0) {
					std::cout << "Error in accept(): "<<strerror(errno)<<std::endl;
				} else {
					FD_SET(peersock, &readset);
					maxfd = (maxfd > peersock)?maxfd:peersock;
				}
				FD_CLR(srvsock, &tempset);
			}

			for (int j = 0; j < maxfd+1; j++) {
				if (FD_ISSET(j, &tempset)) {

					do {
						result = recv(j, buffer, MAX_BYTES_LECTURA, 0);
					} while (result == -1 && errno == EINTR);

					if (result > 0) {

						buffer[result] = 0;
						std::cout << "Echoing: "<<buffer<<std::endl;
						sent = 0;
						do {
							justsent = send(j, buffer+sent, result-sent, MSG_NOSIGNAL); //solo al mismo socket
							if (justsent > 0)
								sent += justsent;
							else if (justsent < 0 && errno != EINTR)
								break; //Puede enviarse solo parte de la data?...
						} while (sent < result);

					} else if (result == 0) {
						close(j);
						FD_CLR(j, &readset);
					} else {
						std::cout << "Error in recv(): "<<strerror(errno)<<std::endl;
					}
				} // end if (FD_ISSET(j, &tempset))
			} // rof
		} // end else if (result > 0)
	}
}


void Server::finalizar() {
	int fd = this->socket->getDescriptor();
	Connection::finalizar();
	this->socket->cerrarSocket(fd);
}

Server::~Server() {
	std::cout << "====== /SERVIDOR/ ======" << std::endl;
}
