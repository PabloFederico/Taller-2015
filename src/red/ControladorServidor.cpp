/*
 * ControladorServidor.cpp
 *
 *  Created on: 7 de nov. de 2015
 *      Author: emanuel
 */

#include "ControladorServidor.h"
#include "../red/Red.h"

ControladorServidor::ControladorServidor(Server* server) {
	this->server = server;
	t = clock();
}

void ControladorServidor::clienteSeDesconecto(int sock_cliente){
	server->conexionPerdida(sock_cliente);
}

bool ControladorServidor::procesarComoServidor(int sock, std::string buffer){
	return server->procesarComoServidor(sock,buffer);
}

void ControladorServidor::enviarATodosMenos(int sock, std::string buffer){
	server->enviarATodosMenos(sock, buffer);
}

TipoEntidad generarRecursoYCoordRandom(Coordenada* c) {
	*c = Calculador::generarPosRandom(50,0,50,0,7);
	Coordenada aux = Calculador::generarPosRandom(ORO+1,MADERA,1,0,42);
	return TipoEntidad(aux.x);	   //último recurso^	  ^primer recurso
}

void ControladorServidor::generacionDeRecursosRandom(){
	if ((clock() - t) > CLOCKS_PER_SEC*DELAY_RECURSOS) {
		Coordenada c;
		TipoEntidad tipoRecurso = generarRecursoYCoordRandom(&c);
		server->enviarATodos(Red::agregarPrefijoYJugYFinal("REC", int(tipoRecurso), c.enc()));
		t = clock();
	}
}

bool ControladorServidor::validarLogIn(int peersock){
	ostringstream ss; string nombreJug, mensaje;
	char buffer[MAX_BYTES_LECTURA];

	// Primer mensaje: Recibo nombre de jugador pedido.
	recv(peersock, buffer, sizeof(buffer), 0);
	Red::extraerPrefijoYMensaje(buffer, &nombreJug);

	try {
		DataCliente cli = clientes.getJugadorDesconectadoLlamado(nombreJug);

		// Si no lanza exception, reconecto a un jugador previo.
		clientes.jugadorConectado(peersock);
		// Segundo mensaje: Envío al jugador su número de jugador.
		ss << cli.id<<"~";
		send(peersock, ss.str().c_str(), 10, MSG_NOSIGNAL);
		// Envío a todos los demás jugadores que este volvió.
		mensaje = Red::agregarPrefijoYFinal("TOG", cli.id);
		server->enviarATodos(mensaje);	// (La nueva conexión todavía ni está en el fd_set)
		sleep(1);
		// Tercer mensaje: Envío al jugador señal de comienzo y su última posición.
		mensaje = Red::agregarPrefijoYFinal("COM", cli.posProtag.enc());
		send(peersock, mensaje.c_str(), MAX_BYTES_LECTURA, MSG_NOSIGNAL);
		send(peersock, mensaje.c_str(), MAX_BYTES_LECTURA, MSG_NOSIGNAL);

	} catch ( NoExiste &e ) {
		// Es un jugador nuevo... lo registro e inicializo
		int idJug = clientes.agregar(peersock, nombreJug);
		// Segundo mensaje: Envío número de jugador.
		ss << idJug<<"~";
		send(peersock, ss.str().c_str(), 5, MSG_NOSIGNAL);

		esperarConexiones();

		sleep(1);
		mensaje = Red::agregarPrefijoYFinal("COM", clientes[peersock].posProtag.enc());
		send(peersock, mensaje.c_str(), 16, MSG_NOSIGNAL);
		// Además, envío el nuevo enemigo a todos los demás jugadores.
		mensaje = clientes.mensajeDeEntidadDeJugador(peersock);
		server->enviarATodosMenos(peersock, mensaje);
	}

	esperarConexiones();
	mensaje = clientes.mensajeDeTodasLasEntidadesConectadas();
	send(peersock, mensaje.c_str(), MAX_BYTES_LECTURA, MSG_NOSIGNAL);
	send(peersock, mensaje.c_str(), MAX_BYTES_LECTURA, MSG_NOSIGNAL);

	return true;
}

void ControladorServidor::esperarConexiones(){
	while (clientes.cant() < 2){
		sleep(2);
	}
}

ControladorServidor::~ControladorServidor() {
}

