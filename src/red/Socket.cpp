/*
 * Socket.cpp
 *
 *  Created on: 6 de oct. de 2015
 *      Author: emanuel
 */

#include "../red/Socket.h"

Socket::Socket() {
	descriptor = socket(AF_INET,SOCK_STREAM,0);
}

bool Socket::creadoCorrectamente(){
	return (descriptor > -1);
}

int Socket::getDescriptor(){
	return descriptor;
}

sockaddr_in* Socket::getpInfoDir(){
	return (&infoDir);
}


void Socket::cerrarSocket(int fd) {
	shutdown(fd, 2);
	close(fd);
}

Socket::~Socket() {
	// TODO Auto-generated destructor stub
}

