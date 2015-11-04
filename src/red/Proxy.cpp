/*
 * Proxy.cpp
 *
 *  Created on: Oct 26, 2015
 *      Author: martin
 */

#include "Proxy.h"


Coordenada* Proxy::clienteEsperarComienzo(Connection* lan) {
	TipoMensajeRed tipo = MENSAJE;
	string unContenido, recibido;
	do {
		try {
			recibido = lan->recibir();
			while (Red::parsearSiguienteMensaje(&recibido, &tipo, &unContenido))
				if (tipo == COMIENZO)
					break;
		} catch ( NoSeRecibio &e ) {}
	} while (tipo != COMIENZO);
	return new Coordenada(Coordenada::dec(unContenido));
}


TipoMensajeRed Proxy::actualizarMultiplayer(Juego* juego) {
	string unContenido, recibido = juego->getConnection()->recibir();
	// Si no se recibe nada, recibir() lanza NoSeRecibio y se saltea el resto.
	TipoMensajeRed tipo;

	while (Red::parsearSiguienteMensaje(&recibido, &tipo, &unContenido)) {
		if (unContenido.length() > 0) {

			switch (tipo) {
			case MENSAJE: procesarMensaje(unContenido);
				break;
			case PING: juego->getConnection()->chequearPing();
				break;
			case COMIENZO: procesarNombre(juego, unContenido);
				break;
			case ESCENARIO: procesarEscenario(juego, unContenido);
				break;
			case MOVIMIENTO: procesarCamino(juego, unContenido);
				break;
			case PASO_COMPLETO:
				break;
			case NUEVA_ENTIDAD: procesarNuevaEntidad(juego, unContenido);
				break;
			case RECURSO: procesarRecurso(juego, unContenido);
				break;
			case TOGGLE: procesarToggle(juego, unContenido);
				break;
			case ATAQUE:
				break;
			case GLOTON: procesarRecursoComido(juego, unContenido);
				break;
			case FIN:
				break;
			}
		}
	}
	return tipo;	// devuelve sólo el último; pero ni se le da uso
}



void Proxy::procesarMensaje(string encodeado) {
	string resto;
	int jug = Red::extraerNumeroYResto(encodeado, &resto);
	Log::imprimirALog(INFO, jug+"> "+resto);
}

void Proxy::procesarNombre(Juego* juego, string encodeado) {
	string nombre;
	if (Red::extraerNumeroYResto(encodeado, &nombre) == juego->getIDJugador())
		juego->setNombreJugador(nombre);
}

void Proxy::procesarEscenario(Juego* juego, string encodeado) {
	// todo
}

void Proxy::procesarCamino(Juego* juego, string encodeado) {
	string camEnc;
	int jug = Red::extraerNumeroYResto(encodeado, &camEnc);
	try {
		juego->getSpritePlayer(jug)->setearNuevoCamino(Camino::dec(camEnc), juego->getCoordCeros());
	} catch ( NoSeRecibio &e ) {
		juego->getSpritePlayer()->setearNuevoCamino(Camino::dec(camEnc), juego->getCoordCeros());
	}
}

void Proxy::procesarNuevaEntidad(Juego* juego, string encodeado) {
	try {
		juego->cargarEnemigo(PosEntidad::dec(encodeado));
	} catch ( FueraDeEscenario &e ) { Log::imprimirALog(WAR, "Enemigo fuera del escenario"); }
}

void Proxy::procesarRecurso(Juego* juego, string encodeado) {
	string posEnc;
	int nTipo = Red::extraerNumeroYResto(encodeado, &posEnc);
	juego->agregarRecurso(TipoEntidad(nTipo), Coordenada::dec(posEnc));
}

void Proxy::procesarRecursoComido(Juego* juego, string posEnc) {
	Coordenada c = Coordenada::dec(posEnc);
	try {
		Entidad* recurso = juego->getEscenario()->getTile(c)->devolverRecurso();
		delete juego->getSpritesEntidades()->find(recurso)->second;
		juego->getEscenario()->quitarRecurso(c, recurso);
	} catch ( NoTieneRecurso &e ) {}
}

void Proxy::procesarToggle(Juego* juego, string encodeado) {
	string aux;
	juego->toggleEnemigo(Red::extraerNumeroYResto(encodeado, &aux));
}

//void procesarAtaque(Juego* juego, string encodeado)

/***************************************************
***************************************************/

void Proxy::enviarNombre(Connection* lan, string s) {
	string t = Red::agregarPrefijoYFinal("COM", s);
	lan->enviar(t);
}

void Proxy::enviar(Connection* lan, string s) {
	string t = Red::agregarPrefijoYJugYFinal("MSJ", lan->getIDJugador(), s);
	lan->enviar(t);
}

void Proxy::enviar(Connection* lan, InfoEscenario ie) {
	string t = Red::agregarPrefijoYFinal("ESC", ie.enc());
	lan->enviar(t);
}

void Proxy::enviar(Connection* lan, Camino cam) {
	string t = Red::agregarPrefijoYJugYFinal("MOV", lan->getIDJugador(), cam.enc());
	lan->enviar(t);
}

void Proxy::enviar(Connection* lan, PosEntidad ent) {
	string t = Red::agregarPrefijoYFinal("ENT", ent.enc());
	lan->enviar(t);
}

void Proxy::comiRecurso(Connection* lan, Coordenada c) {
	string t = Red::agregarPrefijoYFinal("GLO", c.enc());
	lan->enviar(t);
}

void Proxy::completePaso(Connection* lan, int id_jug) {
	string t = Red::agregarPrefijoYFinal("PAS", id_jug);
	lan->enviar(t);
}

//void Proxy::enviar(Ataque)
