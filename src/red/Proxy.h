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
	static void procesarMensaje(string encodeado);
	static void procesarEscenario(Juego* juego, string encodeado);
	static void procesarCamino(Juego* juego, string encodeado);
	//static procesarNuevaEntidad(Juego* juego, string encodeado)
	//static procesarAtaque(Juego* juego, string encodeado)

public:
	static TipoMensajeRed esperarComienzo(Connection* lan);
	// Recibir todo.
	static TipoMensajeRed actualizarMultiplayer(Juego* juego);

	static void enviarComienzo(Connection* lan);			// Enviar señal de comienzo (solo servidor)
	static void enviar(Connection* lan, string s);			// Enviar mensaje
	static void enviar(Connection* lan, InfoEscenario ie);	// Enviar datos iniciales de Escenario
	static void enviar(Connection* lan, Camino cam);		// Enviar movimiento de cierta entidad
	//static void enviar(Entidad*)
	//static void enviar(Ataque)

};

#endif /* RED_PROXY_H_ */
