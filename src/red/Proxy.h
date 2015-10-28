/*
 * Proxy.h
 *
 *  Created on: Oct 26, 2015
 *      Author: martin
 */

#ifndef RED_PROXY_H_
#define RED_PROXY_H_

#include <string>
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
	static void procesarToggle(Juego* juego, string encodeado);
	//static void procesarAtaque(Juego* juego, string encodeado)

public:
	static Coordenada* esperarComienzo(Connection* lan);
	// Recibir todo.
	static TipoMensajeRed actualizarMultiplayer(Juego* juego);

	static void enviarNombre(Connection* lan, string s);
	static void enviar(Connection* lan, string s);	// Enviar mensaje
	static void enviar(Connection* lan, InfoEscenario ie);		// Enviar datos iniciales de Escenario
	static void enviar(Connection* lan, Camino cam);	// Enviar movimientos del protagonista
	static void enviar(Connection* lan, PosEntidad ent);		// Enviar información de entidad
	//static void enviar(Connection* lan, Ataque)

	static string agregarPrefijoYJugYFinal(string prefijo, int jug, string mensaje);
	static TipoMensajeRed extraerPrefijoYMensaje(string recibido, string* mensaje);
};

#endif /* RED_PROXY_H_ */
