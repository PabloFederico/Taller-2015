/*
 * Proxy.h
 *
 *  Created on: Oct 26, 2015
 *      Author: martin
 */

#ifndef RED_PROXY_H_
#define RED_PROXY_H_

#include "../modelo/Juego.h"
#include "../red/Connection.h"
#include "../utils/Structs.h"


class Proxy {

private:
	static void procesarNombre(Juego* juego, string encodeado);
	static void procesarMensaje(string encodeado);
	static void procesarEscenario(Juego* juego, string encodeado);
	static void procesarCamino(Juego* juego, string encodeado);
	static void procesarNuevaEntidad(Juego* juego, string encodeado);
	static void procesarRecurso(Juego* juego, string encodeado);
	static void procesarRecursoComido(Juego* juego, string posEnc);
	static void procesarToggle(Juego* juego, string encodeado);
	//static void procesarAtaque(Juego* juego, string encodeado)

public:
	static Coordenada* clienteEsperarComienzo(Connection* lan);
	static TipoMensajeRed actualizarMultiplayer(Juego* juego);

	// Funciones para enviar cada tipo de cosa.
	static void enviarNombre(Connection* lan, string s);
	static void enviar(Connection* lan, string s);	// Enviar mensaje
	static void enviar(Connection* lan, InfoEscenario ie);		// Enviar datos iniciales de Escenario
	static void enviar(Connection* lan, Camino cam);	// Enviar movimientos del protagonista
	static void enviar(Connection* lan, PosEntidad ent);		// Enviar información de entidad
	static void comiRecurso(Connection* lan, Coordenada c);
	static void completePaso(Connection* lan, int id_jug);
	//static void enviar(Connection* lan, Ataque)
};

#endif /* RED_PROXY_H_ */
