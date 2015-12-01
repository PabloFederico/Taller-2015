/*
 * Server.h
 *
 *  Created on: 15 de oct. de 2015
 *      Author: emanuel
 */

#ifndef RED_RED_H_
#define RED_RED_H_
#include "../utils/Constantes.h"
#include "../utils/Exceptions.h"
#include "../utils/Enumerados.h"
#include "../red/Socket.h"
#include <string.h>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <cerrno>
using namespace std;

class Red {
public:

	/* bind() */
	static int enlazarSocket(Socket *socket){
		int descriptor = socket->getDescriptor();
		sockaddr_in* infoDir = socket->getpInfoDir();
		return bind(descriptor, (sockaddr*)infoDir, sizeof(*infoDir));
	};


	/* listen() */
	static int escucharConexiones(int descriptor, int cantidadConexiones){
		return listen(descriptor, cantidadConexiones+1);
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
		return connect(descriptor, (sockaddr*)infoDir, sizeof(*infoDir));
	};


	/* send() */
	static int enviarInformacion(int descriptor, std::string info){
		return send(descriptor, info.c_str(), strlen(info.c_str()), 0);
	};

	/* recv() */
	static int recibirInformacion(int descriptor, char *info) {
		int success = recv(descriptor, info, MAX_BYTES_LECTURA, 0);
		if (success == 0/* && errno == ENOTCONN*/)
			throw Disconnected();
		if (success > 0)
			info[success] = 0;
		return success;
	};



	/*************************** FUNCIONES DE ENCO Y DECODEADO *******************************/

	static string agregarPrefijoYFinal(string prefijo, string mensaje) {
		ostringstream Encode;
		Encode << "<"<<prefijo<<">"<<mensaje<<"~";
		return Encode.str();
	}
	static string agregarPrefijoYFinal(string prefijo, int numero) {
			ostringstream Encode;
			Encode << "<"<<prefijo<<">"<<numero<<"~";
			return Encode.str();
		}

	static string agregarPrefijoYJugYFinal(string prefijo, int jug, string mensaje) {
		ostringstream Encode;
		Encode << "<"<<prefijo<<">"<<jug<<":"<<mensaje<<"~";
		return Encode.str();
	}


	static TipoMensajeRed extraerPrefijoYMensaje(string mensaje, string* contenido) {
		stringstream ss(mensaje);
		char sTipo[5], charContenido[MAX_BYTES_LECTURA];
		ss.ignore();			// "<"
		ss.get(sTipo, 4, '>');	// "tipo"
		ss.ignore();			// ">"
		ss.get(charContenido, MAX_BYTES_LECTURA, '~');
		*contenido = string(charContenido);
		return StringToTipoMensajeRed(sTipo);
	}

	static int extraerNumeroYResto(string contenido, string* resto) {
		char charResto[MAX_BYTES_LECTURA];
		stringstream ss(contenido);
		int jug; ss >> jug; ss.ignore(); // ':'
		ss.get(charResto, MAX_BYTES_LECTURA, '\0');
		*resto = string(charResto);
		return jug;
	}


	// Diferencio aquello Recibido como el conjunto de Mensajes, cada cual posee la etiqueta de TipoMensajeRed, un Contenido decodificable y un Final ('~').
	// Extrae de recibidoSinParsear un mensaje entero, del cual parsea tipo y contenido.	<TIP>Contenido~
	static bool parsearSiguienteMensaje(string* recibidoSinParsear, TipoMensajeRed* tipo, string* unContenido) {
		if (recibidoSinParsear->length() == 0) return false;
		stringstream ss(*recibidoSinParsear);
		char charUnMensaje[MAX_BYTES_LECTURA], charRestoRecibido[MAX_BYTES_LECTURA];
		string unMensaje;
		*tipo = TipoMensajeRed(0);

		if (ss.peek() != '<') // Me deshago de posible basura.
			ss.get(charUnMensaje, MAX_BYTES_LECTURA, '<');
		ss.get(charUnMensaje, MAX_BYTES_LECTURA, '~');
		ss.ignore();	// '~'
		ss.get(charRestoRecibido, MAX_BYTES_LECTURA);
		*recibidoSinParsear = string(charRestoRecibido);
		unMensaje = string(charUnMensaje);

		bool res = (charUnMensaje[0] == '<');
		if (res)
			*tipo = extraerPrefijoYMensaje(unMensaje, unContenido);
		return res;
	}

	/*****************************************************************************************/
};

#endif /* RED_RED_H_ */
