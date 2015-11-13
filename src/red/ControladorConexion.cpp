/*
 * ControladorConexion.cpp
 *
 *  Created on: 7 de nov. de 2015
 *      Author: emanuel
 */

#include "ControladorConexion.h"
#include "../utils/Constantes.h"

ControladorConexion::ControladorConexion(ControladorServidor* c, int sock) {
	this->controladorServidor = c;
	this->sock_cliente = sock;
	pthread_mutex_init(&mutex,NULL);
}

void ControladorConexion::mutexLock(){
	pthread_mutex_lock(&mutex);
}

void ControladorConexion::mutexUnlock(){
	pthread_mutex_unlock(&mutex);
}

bool ControladorConexion::hayMensajesPendientes(){
	return (colaMsg.size() > 0);
}

void ControladorConexion::acolarMensaje(DataMsg data){
	//mutexLock();
	colaMsg.push_back(data);
	//mutexUnlock();
}

DataMsg ControladorConexion::desacolarMensaje(){
	//mutexLock();
	DataMsg data = colaMsg.front();
	colaMsg.pop_front();
	//mutexUnlock();
	return data;
}

void* ControladorConexion::run(){
	// la función validar LogIn procesa el nombre de usuario y espera hasta que haya
	// un mínimo válido de conexiones para jugar
	// dentro de validarLogIn hay un void esperarConexiones()
	bool userValido = controladorServidor->validarLogIn(sock_cliente);
	if (!userValido) return NULL;
	controladorServidor->fd_clr();
	char buffer[MAX_BYTES_LECTURA+1];
	string mensaje;
	bool quit = false;
	int result;
	while (!quit){
		//if (controladorServidor->fd_ISSET(sock_cliente)) {
			do {	// Recibir del socket actual cualquier mensaje que esté esperando.

				result = recv(sock_cliente, buffer, MAX_BYTES_LECTURA, 0);
				//std::cout<<"tratando de recibir data del cliente : "<<sock_cliente<<std::endl;
			} while (errno == EINTR && result == -1);
			//std::cout<<"sali del do-while : "<<sock_cliente<<std::endl;
			if (result > 0) {
				buffer[result] = 0;
				acolarMensaje(DataMsg(sock_cliente,buffer));
				std::cout << "Recibido: "<<buffer<<std::endl;//
				// Lo recibido de un cliente, si no es únicamente para el servidor, mandarlo a todos los demás

				if (hayMensajesPendientes()){
					DataMsg dataMsg = desacolarMensaje();
					//std::cout <<"procesando : "<<dataMsg.sock_fd<<" "<<dataMsg.msg<<std::endl;
					int fd_sock = dataMsg.sock_fd;
					string msg = dataMsg.msg;
					if (controladorServidor->procesarComoServidor(fd_sock, msg)) {
						std::cout << "Echoing: "<<msg<<std::endl;//
						//controladorServidor->enviarATodosMenos(fd_sock,msg);
						controladorServidor->enviarATodos(msg);
					}
				}

			} else if (result == 0) {
				controladorServidor->clienteSeDesconecto(sock_cliente);
				quit = true;
			} else if (errno != EWOULDBLOCK)
				std::cout << "Error in recv(): "<<strerror(errno)<<std::endl;

		//} // fi (FD_ISSET(j, &readset))


		/* Aca debería estar lo de los recursos random y usar una especie de mutex
		 * para que no todos los hilos creen recursos a cada momento, usando variables
		 * booleanas que indiquen si ya se genero algún recurso en cierto tiempo.
		 * (ver si el mutex debería ir dentro de la función controladorServidor->generarRecursosRandom() ó
		 *  en este controladorConexion) */
		controladorServidor->generacionDeRecursosRandom();

		/* basandome en lo de martin, aca seguiría lo de continuar movimientos
		 * o sea, mandar las coordenadas guardadas (creo) */
		/*
		if (controladorServidor->fd_ISSET(sock_cliente)) {
			try {
				//controladorServidor->mutexLock();
				mensaje = controladorServidor->mensajeParaAvanzarJug(sock_cliente);
				std::cout << "Envío paso "<<mensaje<<std::endl;//
				controladorServidor->enviarATodos(mensaje);
				//controladorServidor->mutexUnlock();
			} catch ( CaminoVacio &e ) {}
		}
		*/
	}
	return NULL;
}

ControladorConexion::~ControladorConexion() {
	pthread_mutex_destroy(&mutex);
}

