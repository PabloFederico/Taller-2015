/*
 * Proxy.cpp
 *
 *  Created on: Oct 26, 2015
 *      Author: martin
 */

#include "Proxy.h"


TipoMensajeRed Proxy::actualizarMultiplayer(Juego* juego) {
	string unContenido, recibido = juego->getConnection()->recibir();
	// Si no se recibe nada, lanza NoSeRecibio y se saltea el resto.
	TipoMensajeRed tipo;

	while (Red::parsearSiguienteMensaje(&recibido, &tipo, &unContenido)) {
		//if (unContenido.length() > 0) {
			switch (tipo) {
			case MENSAJE: procesarMensaje(unContenido);
				break;
			case TOGGLE: procesarToggle(juego, unContenido);
				break;
			case NUEVA_ENTIDAD: procesarNuevaEntidad(juego, unContenido);
				break;
			case INTERACCION: procesarInteraccion(juego, unContenido);
				break;
			case MOVIMIENTO: procesarCamino(juego, unContenido);
				break;
			//case PASO_COMPLETO:
			//	break;
			//case ESCENARIO: procesarEscenario(juego, unContenido);
			//	break;
			//case COMIENZO: //procesarNombre(juego, unContenido);
			//	break;
			//case RECURSO: procesarRecurso(juego, unContenido);
			//	break;
			//case GLOTON: procesarRecursoComido(juego, unContenido);
			//	break;
			//case PING: //juego->getConnection()->revisarPing();
			//	break;
			//case FIN:
			//	break;
			default : continue;
			}
		//}
	}
	return tipo;	// devuelve sólo el último; pero ni se le da uso
}


// Esperar indefinidamente hasta que llegue un mensaje de comienzo.
void Proxy::clienteEsperarComienzo(Connection* lan) {
	TipoMensajeRed tipo = MENSAJE;
	do {
		try {
			string unContenido, recibido = lan->recibir();
			while (Red::parsearSiguienteMensaje(&recibido, &tipo, &unContenido))
				if (tipo == COMIENZO)
					break;
		} catch ( NoSeRecibio &e ) {}
	} while (tipo != COMIENZO);
}

// Esperar indefinidamente hasta que llegue un mensaje de escenario.
//ConfiguracionJuego Proxy::clienteEsperarConfigGame(Connection* lan) {
//	TipoMensajeRed tipo = MENSAJE;
//	string unContenido, recibido;
//	do {
//		try {
//			recibido = lan->recibir();
//			while (Red::parsearSiguienteMensaje(&recibido, &tipo, &unContenido))
//				if (tipo == ESCENARIO)
//					break;
//		} catch ( NoSeRecibio &e ) {}
//	} while (tipo != ESCENARIO);
//	return ConfiguracionJuego::dec(unContenido);
//}

/***************************************************************
***************************************************************/

void Proxy::procesarMensaje(string encodeado) {
	string resto; ostringstream imprimir;
	int jug = Red::extraerNumeroYResto(encodeado, &resto);
	imprimir << "[ @"<<NombreDeJug(jug)<<" ("<<jug<<") > "<<resto<<" ]";
	Log::imprimirALog(INFO, imprimir.str());
}

void Proxy::procesarToggle(Juego* juego, string encodeado) {
	string aux; ostringstream parroquial;
	int id_jug = Red::extraerNumeroYResto(encodeado, &aux);
	juego->apagarEnemigo(id_jug);
	parroquial << id_jug<<":"<<"GRACIAS PA. CHAU CHAU ADIOS // SE HA DESCONECTADO"<<'\0';
	procesarMensaje(parroquial.str());
}

void Proxy::procesarNuevaEntidad(Juego* juego, string encodeado) {
	Entidad ent = Entidad::dec(encodeado);
	if (ent.getIDJug() == juego->getIDJugador()) return;
	try {
		Entidad *aux = NULL;
		if (ent.esRecurso())
			aux = juego->agregarRecurso(ent.getTipo(), ent.getPosicion(), ent.get_identificador());
		else if (ent.esUnidad())
			aux = juego->crearNuevaUnidad(ent.getTipo(), ent.getPosicion(), ent.getIDJug(), ent.get_identificador());
		else if (ent.esConstruccion())
			aux = juego->comenzarNuevaConstruccion(ent.getTipo(), ent.getPosicion(), ent.getIDJug(), ent.get_identificador());
		else if (ent.esEdificio())
			aux = juego->crearNuevoEdificio(ent.getTipo(), ent.getPosicion(), ent.getIDJug(), ent.get_identificador());
		// else imprimir error?
		if (!aux) throw FueraDeEscenario();	// y otros errores...
	} catch ( FueraDeEscenario &e ) { Log::imprimirALog(WAR, "No se pudo agregar enemigo."); }
}

void Proxy::procesarInteraccion(Juego* juego, string encodeado) {
	int tip1, jug1, dni1, tip2, jug2, dni2;
	stringstream ss(encodeado);
	ss >> tip1; ss.ignore(); // ','
	ss >> jug1; ss.ignore(); // ','
	ss >> dni1; ss.ignore(); // ';'
	ss >> tip2; ss.ignore(); // ','
	ss >> jug2; ss.ignore(); // ','
	ss >> dni2;

	if (jug1 == juego->getIDJugador()) return;
	try {
		juego->iniciarInteraccionEntre(TipoEntidad(tip1), jug1, dni1, TipoEntidad(tip2), jug2, dni2);
	} catch ( NoExiste &e ) { Log::imprimirALog(ERR, "Interacción incluye entidad inexistente."); }
}

void Proxy::procesarCamino(Juego* juego, string encodeado) {
	int tipo, id_jug, dni;
	char camEnc[MAX_BYTES_LECTURA+1];
	stringstream ss(encodeado);
	ss >> tipo; ss.ignore();	// ','
	ss >> id_jug; ss.ignore();	// ','
	ss >> dni; ss.ignore();		// ';'
	ss.get(camEnc, MAX_BYTES_LECTURA, '\0');//'~');
	if (id_jug == juego->getIDJugador()) return;

	Entidad *walker = juego->getEntidad(TipoEntidad(tipo), id_jug, dni);
	if (!walker) {
		std::cout << "No se encontró la entidad para el camino."<<std::endl;//
		return; // o agregarlo?
	}
	walker->olvidarInteraccion();
	Sprite* sprite = juego->getSpriteDeEntidad(walker);
	if (!sprite) {
		std::cout << "No se encontró el sprite para la entidad para el camino."<<std::endl;//
		return; // o agregarlo?
	}
	sprite->setearNuevoCamino(Camino::dec(camEnc), juego->getCoordCeros());
}

//void Proxy::procesarEscenario(Juego* juego, string encodeado) {
//	// todo
//}

//void Proxy::procesarNombre(Juego* juego, string encodeado) {
//	string nombre;
//	if (Red::extraerNumeroYResto(encodeado, &nombre) == juego->getIDJugador())
//		juego->setNombreJugador(nombre);
//}

//void Proxy::procesarRecurso(Juego* juego, string encodeado) {
	//string posEnc;
	//int nTipo = Red::extraerNumeroYResto(encodeado, &posEnc);
	//juego->agregarRecurso(TipoEntidad(nTipo), Coordenada::dec(posEnc));
//}

//void Proxy::procesarRecursoComido(Juego* juego, string posEnc) {
//	Coordenada c = Coordenada::dec(posEnc);
//	try {
//		Entidad* recurso = juego->getEscenario()->getTile(c)->devolverRecurso();

		// Elimina el sprite del recurso
//		Map<Entidad*,Sprite*>* mapaSprites = juego->getSpritesEntidades();
		//map<Entidad*,Sprite*>::iterator p = mapaSprites->find(recurso);
		//if (p != mapaSprites->end()){
		//	delete p->second;
		//	mapaSprites->erase(recurso);
		//}

		//juego->getEscenario()->quitarRecurso(c, recurso);
//	} catch ( NoTieneRecurso &e ) {}
//}


/***************************************************************
***************************************************************/


void Proxy::enviar(Connection* lan, string s) {
	string t = Red::agregarPrefijoYJugYFinal("MSJ", lan->getIDJugador(), s);
	lan->enviar(t);
}

void Proxy::enviar(Connection* lan, ConfiguracionJuego cj) {
	string t = Red::agregarPrefijoYFinal("ESC", cj.enc());
	lan->enviar(t);
}

void Proxy::enviar(Connection* lan, Entidad ent) {
	string t = Red::agregarPrefijoYFinal("ENT", ent.enc());
	lan->enviar(t);
}

void Proxy::enviar(Connection* lan, Entidad ejecutor, Entidad receptor) {
	ostringstream interaccion_enc;
	interaccion_enc << ejecutor.getTipo()<<","<<ejecutor.getIDJug()<<","<<ejecutor.get_identificador()<<";";
	interaccion_enc << receptor.getTipo()<<","<<receptor.getIDJug()<<","<<receptor.get_identificador();
	string t = Red::agregarPrefijoYFinal("INT", interaccion_enc.str());
	lan->enviar(t);
}

void Proxy::enviar(Connection* lan, Entidad ent, Camino cam) {
	ostringstream camino_enc;
	camino_enc << ent.getTipo()<<","<<ent.getIDJug()<<","<<ent.get_identificador()<<";"<<cam.enc();
	string t = Red::agregarPrefijoYFinal("MOV", camino_enc.str());
	lan->enviar(t);
}


//void Proxy::completePaso(Connection* lan, int id_jug) {
//	string t = Red::agregarPrefijoYFinal("PAS", id_jug);
//	lan->enviar(t);
//}

//void Proxy::enviarNombre(Connection* lan, string s) {
//	string t = Red::agregarPrefijoYFinal("COM", s);
//	lan->enviar(t);
//}

//void Proxy::comiRecurso(Connection* lan, Coordenada c) {
//	string t = Red::agregarPrefijoYFinal("GLO", c.enc());
//	lan->enviar(t);
//}

