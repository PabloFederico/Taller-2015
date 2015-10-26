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
	Coordenada coord_pixel_sprite = sprite->getPosPies();
	Coordenada coord_pixel_ceros(*juego->getCeros().first + DISTANCIA_ENTRE_X, *juego->getCeros().second);

	try {
		Coordenada coord_tile_sprite = Calculador::tileParaPixel(coord_pixel_sprite,coord_pixel_ceros);
		juego->getEscenario()->actualizarPosicionProtagonista(coord_tile_sprite);
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
				//Seteo tile clic:
				Tile* tile_clic = escenario->getTile(c_tile_clic.x, c_tile_clic.y);
				escenario->setearTileClic(tile_clic);

			}catch(FueraDeEscenario &e) {
				clicValido = false;
				escenario->setearTileClic(NULL);
			}

			/* Si el clic es válido, buscamos el camino mínimo. */
			if (clicValido){
				//SDL_Rect posicionPlayer = sprite->getPosicion();
	            //int Follow_Point_X = MouseX - posicionPlayer.w / 2;
	            //int Follow_Point_Y = MouseY - posicionPlayer.h;

	            ///pruebas
	            //Coordenada mouse = Calculador::tileParaPixel(Coordenada(MouseX,MouseY),coord_pixel_ceros);
	            //Coordenada follow = Calculador::tileParaPixel(Coordenada(Follow_Point_X,Follow_Point_Y), coord_pixel_ceros);
	            //std::cout << "mouse: "<<mouse.x<<";"<<mouse.y<<'\t'<<"follow: "<<follow.x<<";"<<follow.y<<std::endl;

	            try {
	        		//camino = Calculador::obtenerCaminoMin(escenario, coord_pixel_sprite, Coordenada(Follow_Point_X, Follow_Point_Y), coord_pixel_ceros);
	        		Camino camino = Calculador::obtenerCaminoMin(escenario, coord_pixel_sprite, Coordenada(MouseX, MouseY), coord_pixel_ceros);

					/* Activamos el movimiento del sprite y seteamos el nuevo camino que debe recorrer. */
					if (camino.size() > 0) {
						if (juego->esCliente())
							Proxy::enviar(juego->getConnection(), camino);
						sprite->setearNuevoCamino(camino, coord_pixel_ceros);
					}
				} catch ( FueraDeEscenario &e ) {}
			}
		}
	} /* Fin if SDL_MOUSEBUTTONDOWN */

	//try {
	//	Camino camino = juego->recibirCamino();
	//	sprite->setearNuevoCamino(camino, coord_pixel_ceros);
	//} catch ( NoSeRecibio &e ) {}

	if (sprite->estaEnMovimiento())		//verificar que esta optimización no rompa nada
		sprite->update(juego->getVelocidad());

}

ControladorMouse::~ControladorMouse() {
	// TODO Auto-generated destructor stub
}

