/*
 * Controller.cpp
 *
 *  Created on: 6 de sept. de 2015
 *      Author: emanuel
 */

#include "../controlador/Controller.h"


Controller::Controller(Connection* lan = NULL) {
	mouse = new Mouse();	
	this->lan = lan;
	ConfiguracionJuego infoJuego;
	try {
		if (lan != NULL) {
			//infoJuego = Proxy::clienteEsperarConfigGame(lan);
			int modoDeJuego = Proxy::clienteEsperarComienzoYModoDeJuego(lan);
			this->juego = new Juego(lan, modoDeJuego, NULL);//, &infoJuego);
		}
	} catch ( Disconnected &e ) {
		std::cout << "Desconectado."<<std::endl;
		lan = NULL;
	}
	if (lan == NULL)
		this->juego = new Juego(NULL, 0, NULL);

	this->controladorMouse = new ControladorMouse(juego);
	this->controladorCamara = new ControladorCamara(juego);
	this->controladorJuego = new ControladorJuego(juego);
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
	Entidad *entidad = (Entidad*)juego->getJugador()->getUnidadActiva();
	if (!entidad)
		entidad = juego->getEntidad(ALDEANO, juego->getIDJugador(), 1);
	if (!entidad) return;
	Sprite *spriteUnidad = juego->getSpriteDeEntidad(entidad);

	while (spriteUnidad && !playerEstaEnElCentro){	//el juego sobrevive si nunca entra a este ciclo?
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

Mouse* Controller::getMouse(){
	return mouse;
}

void Controller::capturarEvento(SDL_Event &event){

	while (SDL_PollEvent(&event)){
		int x,y;
		SDL_GetMouseState(&x,&y);
		mouse->setXY(Coordenada(x,y));
		switch (event.type){
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT){
				mouse->setEstado(CLICK_IZQUIERDO);
			}
			else{
				if (mouse->getEstado() != CLICK_DER_MOV)
					mouse->setEstado(CLICK_DERECHO);
/*
				bool siguePresionado = true;
				SDL_GetMouseState(&x,&y);
				mouse->inicializarCoordenadaAnterior(Coordenada(x,y));
				while (siguePresionado){
					SDL_GetMouseState(&x,&y);
					SDL_PollEvent(&event);
					mouse->setXY(Coordenada(x,y));
					if (event.type == SDL_MOUSEBUTTONUP)
						siguePresionado = false;
				}

				if (mouse->getXY() != mouse->getXY_anterior()){
					std::cout <<"se arrastro mouse con click derecho presionado\n";
					std::cout <<mouse->getXY_anterior().x<<":"<<mouse->getXY_anterior().y<<" ";
					std::cout <<mouse->getXY().x<<":"<<mouse->getXY().y<<"\n";
					mouse->setEstado(CLICK_DER_MOV);
				}else mouse->setEstado(CLICK_DERECHO);
*/
			}
			break;

		case SDL_MOUSEBUTTONUP:
			mouse->setEstado(NO_CLICK);
			break;

		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&x,&y);
			break;

		default:
			break;
		}

		if (event.type == SDL_MOUSEMOTION){
			if (mouse->getEstado() == CLICK_DERECHO){
				mouse->setXY_anterior(Coordenada(x,y));
				mouse->setEstado(CLICK_DER_MOV);
			}
		}

	} // End while

	juego->continuar();
}

void Controller::procesarEvento(){
	controladorMouse->procesarMouse(mouse);
}

void Controller::actualizarEstadoDelJuego(){
	controladorJuego->actualizarJuego(mouse);
	controladorCamara->actualizarScroll(mouse);

	if (this->juego->esCliente()) {
		try {
			Proxy::actualizarMultiplayer(this->juego);
		} catch ( NoSeRecibio &e ) {
		} catch ( Disconnected &e ) {
			this->juego->olvidarConnection();
			// todo: terminar juego
		}
	}
}

void Controller::agregarCamara(Camara *cam){
	this->controladorCamara->setCamara(cam);
}

void Controller::reiniciarJuego(){
	this->juego->reiniciar();
	delete this->controladorCamara;
	delete this->controladorMouse;
	this->controladorMouse = new ControladorMouse(juego);
	this->controladorCamara = new ControladorCamara(juego);
}


Controller::~Controller() {
	delete this->juego;
	delete this->controladorCamara;
	delete this->controladorMouse;
	delete this->controladorJuego;
	delete this->mouse;
}

