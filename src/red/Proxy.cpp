/*
 * Proxy.cpp
 *
 *  Created on: Oct 26, 2015
 *      Author: martin
 */

#include "Proxy.h"


string agregarPrefijoYFinal(string prefijo, string mensaje) {
	ostringstream Encode;
	Encode << "<"<<prefijo<<">"<<mensaje<<"~";
	return Encode.str();
}

string agregarPrefijoYJugYFinal(string prefijo, int jug, string mensaje) {
	ostringstream Encode;
	Encode << "<"<<prefijo<<">"<<jug<<":"<<mensaje<<"~";
	return Encode.str();
}

TipoMensajeRed extraerPrefijoYMensaje(string recibido, string* mensaje) {
	stringstream ss(recibido);
	char sTipo[5], charMensaje[MAX_BYTES_LECTURA];
	ss.ignore();			// "<"
	ss.get(sTipo, 4, '>');	// "tipo"
	ss.ignore();			// ">"
	ss.get(charMensaje, MAX_BYTES_LECTURA, '\0');
	*mensaje = string(charMensaje);
	return StringToTipoMensajeRed(sTipo);
}


TipoMensajeRed Proxy::esperarComienzo(Connection* lan) {
	TipoMensajeRed tipo = TipoMensajeRed(0);
	do {
		try {
			string unContenido, recibido = lan->recibir();
			stringstream ss(recibido);
			char charUnMensaje[MAX_BYTES_LECTURA];
			ss.get(charUnMensaje, MAX_BYTES_LECTURA, '~');
			string unMensaje(charUnMensaje);
			tipo = extraerPrefijoYMensaje(unMensaje, &unContenido);
		} catch ( NoSeRecibio &e ) {}
	} while (tipo != COMIENZO);
	return tipo;
}

TipoMensajeRed Proxy::actualizarMultiplayer(Juego* juego) {
	string unContenido, recibido = juego->getConnection()->recibir();
	// Si no se recibe nada, recibir() lanza NoSeRecibio y se saltea el resto.
	TipoMensajeRed tipo = TipoMensajeRed(0);

	stringstream ss(recibido);
	char charUnMensaje[MAX_BYTES_LECTURA];
	ss.get(charUnMensaje, MAX_BYTES_LECTURA, '~');

	while (charUnMensaje[0] == '<') {
		string unMensaje(charUnMensaje);
		tipo = extraerPrefijoYMensaje(unMensaje, &unContenido);

		switch (tipo) {
		case COMIENZO:
			break;
		case ESCENARIO: procesarEscenario(juego, unContenido);
			break;
		case MOVIMIENTO: procesarCamino(juego, unContenido);
			break;
		case NUEVA_ENTIDAD:
			break;
		case ATAQUE:
			break;
		case MENSAJE:
		default: procesarMensaje(unContenido);
		}

		ss.ignore();	// '~'
		ss.get(charUnMensaje, MAX_BYTES_LECTURA, '~');
	}

	return tipo;	// devuelve sólo el último; pero no tiene mucho uso igual
}


void Proxy::procesarMensaje(string encodeado) {
	stringstream ss(encodeado);
	int jug; ss >> jug; ss.ignore(); // ':'
	Log::imprimirALog(INFO, jug+"> "+ss.str());
}

void Proxy::procesarEscenario(Juego* juego, string encodeado) {
	// TODO
}

void Proxy::procesarCamino(Juego* juego, string encodeado) {
	stringstream ss(encodeado);
	int jug; ss >> jug; ss.ignore(); // ':'
	char camEnc[MAX_BYTES_LECTURA];
	ss.get(camEnc, MAX_BYTES_LECTURA, '~');
	std::cout << jug << "|||" << camEnc << std::endl;//
	Camino cam = Camino::dec(camEnc);
	// TODO extender a varios jugadores
	juego->getSpritePlayer()->setearNuevoCamino(cam, juego->getCoordCeros());
	//juego->getSpritePlayer(jug)->setearNuevoCamino(cam, juego->getCoordCeros());
}

void Proxy::procesarNuevaEntidad(Juego* juego, string encodeado) {
	juego->cargarEnemigo(PosEntidad::dec(encodeado));
}

//void procesarAtaque(Juego* juego, string encodeado)

/***************************************************
***************************************************/

void Proxy::enviar(Connection* lan, string s) {
	string t = agregarPrefijoYJugYFinal("MSJ", lan->getIDJugador(), s);
	lan->enviar(t);
}

void Proxy::enviar(Connection* lan, InfoEscenario ie) {
	string t = agregarPrefijoYFinal("ESC", ie.enc());
	lan->enviar(t);
}

void Proxy::enviar(Connection* lan, Camino cam) {
	string t = agregarPrefijoYJugYFinal("MOV", lan->getIDJugador(), cam.enc());
	lan->enviar(t);
}

void Proxy::enviar(Connection* lan, PosEntidad ent) {
	string t = agregarPrefijoYFinal("ENT", ent.enc());
	lan->enviar(t);
}
//void Proxy::enviar(Ataque)
