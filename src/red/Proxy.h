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
#include "../utils/ConfigDefault.h"


class Juego;

class Proxy {

private:
	static void procesarMensaje(string encodeado);
	static void procesarToggle(Juego* juego, string encodeado);
	static void procesarNuevaEntidad(Juego* juego, string encodeado);
	static void procesarInteraccion(Juego* juego, string encodeado);
	static void procesarConversion(Juego* juego, string encodeado);
	static void procesarCamino(Juego* juego, string encodeado);
	static void procesarRecurso(Juego* juego, string encodeado);
	static void procesarFin(Juego* juego, string sGanador);
	//static void procesarEscenario(Juego* juego, string encodeado);
	//static void procesarNombre(Juego* juego, string encodeado);
	//static void procesarRecursoComido(Juego* juego, string posEnc);

public:
	static ObjetivoEscenario clienteEsperarComienzoYModoDeJuego(Connection* lan);
	//static ConfiguracionJuego clienteEsperarConfigGame(Connection* lan);
	static TipoMensajeRed actualizarMultiplayer(Juego* juego);

	// Funciones para enviar cada tipo de cosa.
	static void enviar(Connection* lan, string s);			// Enviar mensaje
	static void enviar(Connection* lan, ConfiguracionJuego cj);	// Enviar configuración inicial de Juego, incluyendo Escenario
	static void enviar(Connection* lan, Entidad ent);		// Enviar información de entidad
	static void enviar(Connection* lan, Entidad ejecutor, Entidad receptor); // Enviar interacción
	static void enviar(Connection* lan, Entidad ent, Camino cam);	// Enviar movimientos
	static void enviarConversion(Connection* lan, int id_jug_conversor, int id_jug_convertido);	// Enviar conversión de unidades de un jugador
	static void enviar(Connection* lan, int id_jug);	// Enviar toggle
	//static void completePaso(Connection* lan, int id_jug);
	//static void enviarNombre(Connection* lan, string s);
	//static void comiRecurso(Connection* lan, Coordenada c);
	//static void enviar(Connection* lan, Ataque)
};

#endif /* RED_PROXY_H_ */
