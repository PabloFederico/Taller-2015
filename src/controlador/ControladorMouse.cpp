/*
 * ControladorMouse.cpp
 *
 *  Created on: 8 de oct. de 2015
 *      Author: emanuel
 */

#include "ControladorMouse.h"
#include "../utils/Calculador.h"
#include "../modelo/DetectorDeColisiones.h"

#include "../red/Server.h"
#include "../red/Connection.h"

ControladorMouse::ControladorMouse(Juego *juego) {
	this->juego = juego;
}

void ControladorMouse::procesarEvento(SDL_Event &event, int MouseX, int MouseY){
	Sprite *sprite = juego->getSpritePlayer();

	/********** Actualización de la capa negra ***********/
	int x = sprite->getPosicion().x + sprite->getPosicion().w / 2;
	int y = sprite->getPosicion().y + sprite->getPosicion().h / 2;

	Coordenada coord_pixel_sprite(x,y);
	Coordenada coord_pixel_ceros(*juego->getCeros().first + DISTANCIA_ENTRE_X, *juego->getCeros().second);

	try {
		Coordenada coord_tile_sprite = Calculador::tileParaPixel(coord_pixel_sprite,coord_pixel_ceros);
		juego->getEscenario()->getCapa()->descubrirDesdePunto(coord_tile_sprite.x, coord_tile_sprite.y);
	}catch (FueraDeEscenario &e){}


	/*********** Análisis del clic del mouse *************/
	if (event.type == SDL_MOUSEBUTTONDOWN){
		if (event.button.button == SDL_BUTTON_LEFT){
			bool clicValido;
			Escenario *escenario = juego->getEscenario();

			try{
				Coordenada c_tile_clic = Calculador::tileParaPixel(Coordenada(MouseX,MouseY), coord_pixel_ceros);
				clicValido = Calculador::puntoContenidoEnEscenario(c_tile_clic, escenario);
			}catch(FueraDeEscenario &e) {
				clicValido = false;
			}

			/* Si el clic es válido, buscamos el camino mínimo. */
			if (clicValido){
				SDL_Rect posicionPlayer = sprite->getPosicion();
	            int Follow_Point_X = MouseX - posicionPlayer.w / 2;
	            int Follow_Point_Y = MouseY - posicionPlayer.h;

				////código de prueba
	            Camino camino;
	        	try {
	        		//cout << MouseX << ";" << MouseY << "/" << Follow_Point_X << ";" << Follow_Point_Y << endl;
 	        		//Coordenada coord_tile_sprite = Calculador::tileParaPixel(coord_pixel_sprite, coord_pixel_ceros);
	        		//cout << coord_tile_sprite.x << ";" << coord_tile_sprite.y << " ";
	        		camino = Calculador::obtenerCaminoMin(escenario, coord_pixel_sprite, Coordenada(Follow_Point_X, Follow_Point_Y), coord_pixel_ceros);
					//for (vector<Coordenada>::iterator it = vec.begin(); it < vec.end(); ++it) {
					//	try {
					//		Coordenada tile = Calculador::tileParaPixel(*it, coord_pixel_ceros);
					//		cout << tile.x << ";" << tile.y << " ";
					//	} catch ( FueraDeEscenario &e ) { cout << "Fuera de escenario "; }
					//}
					//cout << endl;
				} catch ( FueraDeEscenario &e ) {}
	        	////

				/* Activamos el movimiento del sprite y seteamos el nuevo camino que debe recorrer. */
				if (camino.size() > 0) {

					///solo de prueba, los encodeados tendrían q tener por lo menos una letra al comienzo especificando el tipo [ej.: M(ovimiento)].
					// asumo servidor
					juego->enviar(camino);
					///

					sprite->setearNuevoCamino(camino, coord_pixel_ceros);
				 }

			}
		}
	} /* Fin if SDL_MOUSEBUTTONDOWN */

	try {
		Camino camino = juego->recibirCamino();
		sprite->setearNuevoCamino(camino, coord_pixel_ceros);
	} catch ( NoSeRecibio &e ) {}

	sprite->update(juego->getVelocidad());

}

ControladorMouse::~ControladorMouse() {
	// TODO Auto-generated destructor stub
}

