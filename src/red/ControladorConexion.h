/*
 * ControladorConexion.h
 *
 *  Created on: 7 de nov. de 2015
 *      Author: emanuel
 */

#ifndef SRC_RED_CONTROLADORCONEXION_H_
#define SRC_RED_CONTROLADORCONEXION_H_
#include "../utils/Thread.h"
#include "../red/ControladorServidor.h"
#include <list>

struct DataMsg{
	int sock_fd;
	std::string msg;

	DataMsg(int fd, std::string msg){
		sock_fd = fd;
		this->msg = msg;
	}
};

class ControladorConexion: public Thread {
private:
	ControladorServidor* controladorServidor;
	int sock_cliente;
	list<DataMsg> colaMsg;
	pthread_mutex_t mutex;

	void mutexLock();
	void mutexUnlock();

public:
	ControladorConexion(ControladorServidor* controlador, int sock_cliente);

	void* run();

	void acolarMensaje(DataMsg data);

	DataMsg desacolarMensaje();

	bool hayMensajesPendientes();

	virtual ~ControladorConexion();
};

#endif /* SRC_RED_CONTROLADORCONEXION_H_ */
