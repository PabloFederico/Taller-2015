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

string Proxy::agregarPrefijoYJugYFinal(string prefijo, int jug, string mensaje) {
	ostringstream Encode;
	Encode << "<"<<prefijo<<">"<<jug<<":"<<mensaje<<"~";
	return Encode.str();
}

TipoMensajeRed Proxy::extraerPrefijoYMensaje(string recibido, string* mensaje) {
	stringstream ss(recibido);
	char sTipo[5], charMensaje[MAX_BYTES_LECTURA];
	ss.ignore();			// "<"
	ss.get(sTipo, 4, '>');	// "tipo"
	ss.ignore();			// ">"
	ss.get(charMensaje, MAX_BYTES_LECTURA, '\0');
	*mensaje = string(charMensaje);
	return StringToTipoMensajeRed(sTipo);
}


Coordenada* Proxy::esperarComienzo(Connection* lan) {
	TipoMensajeRed tipo = TipoMensajeRed(0);
	string unContenido;
	do {
		try {
			string recibido = lan->recibir();
			stringstream ss(recibido);
			char charUnMensaje[MAX_BYTES_LECTURA];
			if (ss.peek() != '<') // Me deshago de posible basura.
				ss.get(charUnMensaje, MAX_BYTES_LECTURA, '<');
			ss.get(charUnMensaje, MAX_BYTES_LECTURA, '~');
			string unMensaje(charUnMensaje);
			tipo = extraerPrefijoYMensaje(unMensaje, &unContenido);
		} catch ( NoSeRecibio &e ) {}
	} while (tipo != COMIENZO);
	return new Coordenada(Coordenada::dec(unContenido));
}

TipoMensajeRed Proxy::actualizarMultiplayer(Juego* juego) {
	string unContenido, recibido = juego->getConnection()->recibir();
	// Si no se recibe nada, recibir() lanza NoSeRecibio y se saltea el resto.
	TipoMensajeRed tipo = TipoMensajeRed(0);

	stringstream ss(recibido);
	char charUnMensaje[MAX_BYTES_LECTURA];
	if (ss.peek() != '<') // Me deshago de posible basura.
		ss.get(charUnMensaje, MAX_BYTES_LECTURA, '<');
	ss.get(charUnMensaje, MAX_BYTES_LECTURA, '~');

	while (charUnMensaje[0] == '<') {
		string unMensaje(charUnMensaje);
		tipo = extraerPrefijoYMensaje(unMensaje, &unContenido);

		switch (tipo) {
		case COMIENZO: procesarNombre(juego, unContenido);
			break;
		case ESCENARIO: procesarEscenario(juego, unContenido);
			break;
		case MOVIMIENTO: procesarCamino(juego, unContenido);
			break;
		case NUEVA_ENTIDAD: procesarNuevaEntidad(juego, unContenido);
			break;
		case RECURSO: procesarRecurso(juego, unContenido);
			break;
		case TOGGLE: procesarToggle(juego, unContenido);
			break;
		case ATAQUE:
			break;
		case MENSAJE: procesarMensaje(unContenido);
			break;
		case FIN:
			break;
		}

		ss.ignore();	// '~'
		if (ss.peek() != '<') // Me deshago de posible basura.
				ss.get(charUnMensaje, MAX_BYTES_LECTURA, '<');
		ss.get(charUnMensaje, MAX_BYTES_LECTURA, '~');
	}

	return tipo;	// devuelve sólo el último; pero no tiene uso igual
}


void Proxy::procesarMensaje(string encodeado) {
	stringstream ss(encodeado);
	int jug; ss >> jug; ss.ignore(); // ':'
	Log::imprimirALog(INFO, jug+"> "+ss.str());
}

void Proxy::procesarNombre(Juego* juego, string encodeado) {
	stringstream ss(encodeado);
	int jug; ss >> jug; ss.ignore(); // ':'
	char nom[MAX_BYTES_LECTURA];
	ss.get(nom, MAX_BYTES_LECTURA, '~');
	if (jug == juego->getIDJugador())
		juego->setNombreJugador(string(nom));
}

void Proxy::procesarEscenario(Juego* juego, string encodeado) {
	// todo
}

void Proxy::procesarCamino(Juego* juego, string encodeado) {
	stringstream ss(encodeado);
	int jug; ss >> jug; ss.ignore(); // ':'
	char camEnc[MAX_BYTES_LECTURA];
	ss.get(camEnc, MAX_BYTES_LECTURA, '~');
	Camino cam = Camino::dec(camEnc);
	juego->getSpritePlayer(jug)->setearNuevoCamino(cam, juego->getCoordCeros());
}

void Proxy::procesarNuevaEntidad(Juego* juego, string encodeado) {
	juego->cargarEnemigo(PosEntidad::dec(encodeado));
}

void Proxy::procesarRecurso(Juego* juego, string encodeado) {
	stringstream ss(encodeado);
	int nTipo; ss >> nTipo; ss.ignore(); // ','
	char posEnc[MAX_BYTES_LECTURA];
	ss.get(posEnc, MAX_BYTES_LECTURA, '~');
	//juego->agregarRecurso(TipoEntidad(nTipo), Coordenada::dec(posEnc));
}

void Proxy::procesarToggle(Juego* juego, string encodeado) {
	stringstream ss(encodeado);
	int jug; ss >> jug;
	juego->toggleEnemigo(jug);
}

//void procesarAtaque(Juego* juego, string encodeado)

/***************************************************
***************************************************/

void Proxy::enviarNombre(Connection* lan, string s) {
	string t = agregarPrefijoYFinal("COM", s);
	lan->enviar(t);
}

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
