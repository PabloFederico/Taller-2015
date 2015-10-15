/*
 * Socket.h
 *
 *  Created on: 6 de oct. de 2015
 *      Author: emanuel
 */

#ifndef RED_SOCKET_H_
#define RED_SOCKET_H_
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

class Socket {
protected:
	int descriptor;
	struct sockaddr_in infoDir;

public:
	Socket();

	bool creadoCorrectamente();

	int getDescriptor();

	sockaddr_in getInfoDir();

	void cerrarSocket();

	virtual ~Socket();
};

#endif /* RED_SOCKET_H_ */
