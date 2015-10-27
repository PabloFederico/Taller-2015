/*
 * Server.h
 *
 *  Created on: 15 de oct. de 2015
 *      Author: emanuel
 */

#ifndef RED_RED_H_
#define RED_RED_H_
#include "../utils/Constantes.h"
#include "../red/Socket.h"
#include <string.h>
#include <stdlib.h>
#include <string>
#include <cerrno>
#include "../modelo/Exceptions.h"
using namespace std;

class Red {
public:

	/* bind() */
	static int enlazarSocket(Socket *socket){
		int descriptor = socket->getDescriptor();
		sockaddr_in* infoDir = socket->getpInfoDir();

		int success = bind(descriptor, (sockaddr*)infoDir, sizeof(*infoDir));
		return success;
	};


	/* listen() */
	static int escucharConexiones(int descriptor, int cantidadConexiones){
		int success = listen(descriptor, cantidadConexiones);
		return success;
	};


	/* accept() */ // por 30 segundos //
	static int aceptarCliente(Socket* socket){
		int descriptorServidor = socket->getDescriptor();
		sockaddr_in* infoDir = socket->getpInfoDir();
		socklen_t tam = sizeof(*infoDir);

		int descriptorCliente = accept(descriptorServidor, (sockaddr*)infoDir, &tam);
		return descriptorCliente;
	};


	/* connect() */
	static int crearConexion(Socket* socket){
		int descriptor = socket->getDescriptor();
		sockaddr_in* infoDir = socket->getpInfoDir();
		int success = connect(descriptor, (sockaddr*)infoDir, sizeof(*infoDir));
		return success;
	};


	/* send() */
	static int enviarInformacion(int descriptor, std::string info){
		int success = send(descriptor, info.c_str(), strlen(info.c_str()), 0);
		return success;
	};

	/* recv() */
	static int recibirInformacion(int descriptor, char *info){
		int success = recv(descriptor, info, MAX_BYTES_LECTURA, 0);
		if (success == 0 && errno == ENOTCONN)
			throw Disconnected();
		return success;
	};


};

#endif /* RED_RED_H_ */
