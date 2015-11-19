/*
 * Controller.cpp
 *
 *  Created on: 6 de sept. de 2015
 *      Author: emanuel
 */

#include "../controlador/Controller.h"


Controller::Controller(){//Connection* lan = NULL) {
	/*this->lan = lan;
	Coordenada* posInicial = NULL;
	try {
		if (lan != NULL)
			posInicial = Proxy::clienteEsperarComienzo(lan);
	} catch ( Disconnected &e ) {
		std::cout << "Desconectado."<<std::endl;
		lan = NULL;
	}
*/
	this->juego = new Juego();//lan, posInicial, NULL);
/*
	if (lan != NULL) {
		Proxy::enviarNombre(lan, juego->getNombreJugador());
		Proxy::enviar(lan, juego->getPosEntDeProtagonista());
	}
*/
	this->controladorMouse = new ControladorMouse(juego);
	this->controladorCamara = new ControladorCamara(juego);
	this->controladorEscenario = new ControladorEscenario(juego);
}

Juego* Controller::getJuego(){
	return this->juego;
}

void Controller::posicionarCamaraEnProtagonista(){
	bool playerEstaEnElCentro = false;
	int cant_mover = 10;
	int width_camera = juego->getDimensionVentana().first;
	int height_camera = juego->getDimensionVentana().second - juego->getBarraEstado()->getDimension().second;
	/* Agarra cualquier unidad y posiciona la camara sobre esa unidad */
	Sprite* spriteUnidad = juego->getSpriteDeEntidad((Entidad*)juego->getJugador()->getUnidadActiva());

	while (!playerEstaEnElCentro){
		SDL_Rect posicion = spriteUnidad->getPosicion();
		if (posicion.x < (width_camera/2 - 30)){
			controladorCamara->moverCamara(cant_mover,0);
		}
		posicion = spriteUnidad->getPosicion();
		if (posicion.x > (width_camera/2 + 30)){
			controladorCamara->moverCamara(-cant_mover,0);
		}
		posicion = spriteUnidad->getPosicion();
		if (posicion.y < (height_camera/2 - 30)){
			controladorCamara->moverCamara(0,cant_mover);
		}
		posicion = spriteUnidad->getPosicion();
		if (posicion.y > (height_camera/2 + 30)){
			controladorCamara->moverCamara(0,-cant_mover);
		}
		posicion = spriteUnidad->getPosicion();

		if (posicion.x > 0 &&
			posicion.x < width_camera &&
			posicion.y > 0 &&
			posicion.y < height_camera){
			playerEstaEnElCentro = true;
		}
	}
}

void Controller::procesarEvento(SDL_Event &event){
	int x,y;
	SDL_GetMouseState(&x,&y);
	controladorEscenario->actualizarEstadoEscenario();//juego->getConnection());
	controladorMouse->procesarEvento(event,x,y);//, this->juego->getConnection());
	controladorCamara->procesarPosicionMouse(x,y);
	juego->continuar();
/*
	if (this->juego->esCliente()) {
		try {
			Proxy::actualizarMultiplayer(this->juego);
		} catch ( NoSeRecibio &e ) {
			//try {
				//juego->getConnection()->chequearPing();
			//} catch ( Disconnected &e ) {
				//juego->olvidarConnection();
			//}
		} catch ( Disconnected &e ) {
			//juego->olvidarConnection();
		}
	}// else {
		//if (((Client*)this->lan)->reintentarConexion(juego->getNombreJugador()))
		//	juego->setConnection(this->lan);
		//else
		//	this->lan->finalizar();
	//}
*/
}

void Controller::agregarCamara(Camara *cam){
	this->controladorCamara->setCamara(cam);
}

void Controller::reiniciarJuego(){
	this->juego->reiniciar();
	delete this->controladorCamara;
	delete this->controladorMouse;
	delete this->controladorEscenario;
	this->controladorMouse = new ControladorMouse(juego);
	this->controladorCamara = new ControladorCamara(juego);
	this->controladorEscenario = new ControladorEscenario(juego);
}

int Controller::verificarConexion(std::string string_IP){
	return -1;	// !!! todo
}


Controller::~Controller() {
	delete this->juego;
	delete this->controladorCamara;
	delete this->controladorMouse;
	delete this->controladorEscenario;
}

