/*
 * Server.cpp
 *
 *  Created on: 15 de oct. de 2015
 *      Author: emanuel
 */

#include "../red/Server.h"
#include "../red/ControladorConexion.h"

Server::Server() {
	std::cout << "======= SERVIDOR =======" << std::endl;
	if (!iniciar()) {
		throw ConnectionProblem();
	}
	this->controladorServer = new ControladorServidor(this);
}

bool Server::iniciar() {
	this->socket = new SocketServidor();
	if (this->socket->creadoCorrectamente() < 0) {
		std::cout << "ERROR: No se pudo crear socket."<<std::endl;
		return false;
	}
	if (Red::enlazarSocket(this->socket) < 0) {
		std::cout << "ERROR: bind failed."<<std::endl;
		return false;
	}
	if (Red::escucharConexiones(this->socket->getDescriptor(),MAX_CONEXIONES) < 0)
		return false;
	clientes.cantConectados = 0;
	return true;
}


// Comunicar mensaje a todos (o casi) los sockets de readset.
void Server::enviarATodos(string mensaje) {
	enviarATodosMenos(-1, mensaje);
}

void Server::enviarATodosMenos(int socketNoRecibe, string mensaje) {
	for (int j = 0; j < maxfd+1; j++)
		if (FD_ISSET(j, &readset) && j != socketNoRecibe)
			send(j, mensaje.c_str(), mensaje.size()+1, MSG_NOSIGNAL);
}



// Intercambia mensajes iniciales; manda señales de comienzo si el juego ya empezó.
void Server::inicializarCliente(int peersock, int segundosDeEspera) {
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
		enviarATodos(mensaje);	// (La nueva conexión todavía ni está en el fd_set)
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

		// Tercer mensaje: Si el juego ya ha comenzado, enviar señal de comienzo y nueva posición inmediatamente.
		if (segundosDeEspera < 3) {
			sleep(1);
			mensaje = Red::agregarPrefijoYFinal("COM", clientes[peersock].posProtag.enc());
			send(peersock, mensaje.c_str(), 16, MSG_NOSIGNAL);
			// Además, envío el nuevo enemigo a todos los demás jugadores.
			mensaje = clientes.mensajeDeEntidadDeJugador(peersock);
			enviarATodosMenos(peersock, mensaje);
		}
	}

	// Si el juego ya ha comenzado, envío a la nueva conexión las posiciones de todos los demás.
	if (segundosDeEspera < 3) {
		mensaje = clientes.mensajeDeTodasLasEntidadesConectadas();
		send(peersock, mensaje.c_str(), MAX_BYTES_LECTURA, MSG_NOSIGNAL);
		send(peersock, mensaje.c_str(), MAX_BYTES_LECTURA, MSG_NOSIGNAL);
	}
}


int Server::intentarNuevaConexion(fd_set* p_tempset, int segundosDeEspera) {
	int result, srvsock, peersock;
	sockaddr_in* p_addr = this->socket->getpInfoDir();
	srvsock = this->socket->getDescriptor();
	timeval tv;
	tv.tv_sec = segundosDeEspera;
	tv.tv_usec = 0;

	result = select(maxfd+1, p_tempset, NULL, NULL, &tv);

	if (result < 0 && errno != EINTR) {
		std::cout << "Error in select(): "<<strerror(errno)<<std::endl;

	} else if ((result > 0) && (FD_ISSET(srvsock, p_tempset))) {
		unsigned int len = sizeof(*p_addr);

		peersock = accept(srvsock, (sockaddr*)p_addr, &len);

		if (peersock < 0) {
			std::cout << "Error in accept(): "<<strerror(errno)<<std::endl;
			return -1;
		}

		inicializarCliente(peersock, segundosDeEspera);

		fcntl(peersock, F_SETFL, O_NONBLOCK); // non-blocking mode
		FD_SET(peersock, &readset);
		maxfd = (maxfd > peersock)?maxfd:peersock;
		std::cout << "Jugador "<<clientes[peersock].id<<" conectado!"<<std::endl;
		return peersock;
	}
	return -1;
}

void Server::chequearPorNuevosClientes() {
	fd_set tempset;
	int srvsock = this->socket->getDescriptor();
	FD_SET(srvsock, &readset);
	memcpy(&tempset, &readset, sizeof(tempset));
	intentarNuevaConexion(&tempset, 0);
	memcpy(&tempset, &readset, sizeof(tempset));
	FD_CLR(srvsock, &readset);
}

void Server::conexionPerdida(int j) {
	close(j);
	FD_CLR(j, &readset);
	enviarATodos(Red::agregarPrefijoYFinal("TOG", clientes[j].id));	//Avisarle a los demás.
	clientes.jugadorDesconectado(j);
	std::cout << "Socket "<<j<<": "<<clientes[j].nombre<<" (jugador "<<clientes[j].id<<") se ha desconectado."<<std::endl;
}


// POST: true: el mensaje era SOLO para el servidor y no debe ser repetido a los otros clientes
bool Server::procesarComoServidor(int sockfd, string recibido) {
	bool rebotarlo = true;
	string unMensaje, unContenido;
	TipoMensajeRed tipo;

	while (Red::parsearSiguienteMensaje(&recibido, &tipo, &unContenido)) {
		switch (tipo) {
		case COMIENZO: {	// Cambio de nombre de jugador
				string nuevoNombre = clientes.nuevoNombrePara(sockfd, string(unContenido));
				if (nuevoNombre != unContenido) {	// Si se ajustó el nombre, avisarle al jugador.
					string enviarNombre = Red::agregarPrefijoYJugYFinal("COM", clientes[sockfd].id, nuevoNombre);
					send(sockfd, enviarNombre.c_str(), sizeof(enviarNombre), MSG_NOSIGNAL);
				}
				rebotarlo = false;
			} break;

		case MOVIMIENTO: {
				string camEnc;
				Red::extraerNumeroYResto(unContenido, &camEnc);
				clientes[sockfd].setCamino(Camino::dec(camEnc));	// Settear nuevo camino que hace dicho jugador
				rebotarlo = false;
			} break;

		case PASO_COMPLETO: {
				stringstream ss(unContenido);
				int jug; ss >> jug;
				clientes.alguienCompletoPasoDe(jug);
				rebotarlo = false;
			} break;

		case NUEVA_ENTIDAD: {
				PosEntidad peAux = PosEntidad::dec(unContenido);
				clientes[sockfd].entidad = *peAux.entidad;
				clientes[sockfd].posProtag = peAux.coord();;
				rebotarlo = false;
			} break;

		//MENSAJE, ESCENARIO, RECURSO, TOGGLE, ATAQUE, GLOTON, FIN
		default: rebotarlo = true;
		}
	}
	return rebotarlo;
}


/*
TipoEntidad generarRecursoYCoordRandom(Coordenada* c) {
	*c = Calculador::generarPosRandom(50,0,50,0,7);
	Coordenada aux = Calculador::generarPosRandom(ORO+1,MADERA,1,0,42);
	return TipoEntidad(aux.x);	   //último recurso^	  ^primer recurso
}
*/


void Server::correr() {
	fd_set tempset;
	string mensaje;
	char buffer[MAX_BYTES_LECTURA+1];
	int j, result, srvsock = this->socket->getDescriptor();

	FD_ZERO(&readset);
	FD_SET(srvsock, &readset);
	maxfd = srvsock;

	/********************** CONEXIONES INICIALES **********************/
	std::cout << "Aceptando hasta "<<MAX_CONEXIONES<<" jugadores ("<<MAX_ESPERA_CONEXION<<" s para conectarse)."<<std::endl;


	while (true){
		int client_sock = Red::aceptarCliente(socket);
		if (client_sock > 0){
			ControladorConexion* controladorConexion = new ControladorConexion(controladorServer,client_sock);
			// Aca empieza la magia con hilos
			// El método ejecutar manda a ejecutar el método run de cualquier clase que herede de Thread
			controladorConexion->ejecutar();
		}
	} // end while
	/******************************************************************/

	std::cout << std::endl << "Se han desconectado todos los jugadores."<<std::endl<<"Fin de la partida."<<std::endl;
	sleep(4);
}



void Server::finalizar() {
	int fd = this->socket->getDescriptor();
	Connection::finalizar();
	this->socket->cerrarSocket(fd);
}

Server::~Server() {
	// El destructor de Connection se ocupa de llamar a finalizar
	std::cout << "====== /SERVIDOR/ ======" << std::endl;
}
