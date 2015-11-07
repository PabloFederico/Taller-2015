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
}

void* ControladorConexion::run(){
	// la función validar LogIn procesa el nombre de usuario y espera hasta que haya
	// un mínimo válido de conexiones para jugar
	// dentro de validarLogIn hay un void esperarConexiones()
	bool userValido = controladorServidor->validarLogIn(sock_cliente);
	if (!userValido) return NULL;
	char buffer[MAX_BYTES_LECTURA];
	bool quit = false;
	while (!quit){
		int len = Red::recibirInformacion(sock_cliente,buffer);
		if (len <= 0){
			controladorServidor->clienteSeDesconecto(sock_cliente);
			quit = true;
		}else{
			buffer[len] = 0;
			if (controladorServidor->procesarComoServidor(sock_cliente,std::string(buffer))){
				std::cout << "Echoing: "<<buffer<<std::endl;//
				controladorServidor->enviarATodosMenos(sock_cliente, buffer);
			}
		}

		/* Aca debería estar lo de los recursos random y usar una especie de mutex
		 * para que no todos los hilos creen recursos a cada momento, usando variables
		 * booleanas que indiquen si ya se genero algún recurso en cierto tiempo.
		 * (ver si el mutex debería ir dentro de la función controladorServidor->generarRecursosRandom() ó
		 *  en este controladorConexion) */
		//controladorServidor->generacionDeRecursosRandom();

		/* basandome en lo de martin, aca seguiría lo de continuar movimientos
		 * o sea, mandar las coordenadas guardadas (creo) */
	}
	return NULL;
}

ControladorConexion::~ControladorConexion() {
	// TODO Auto-generated destructor stub
}

