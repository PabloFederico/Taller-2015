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
				escenario->setearCoordTileClic(c_tile_clic);

			}catch(FueraDeEscenario &e) {
				clicValido = false;
				escenario->setearTileClic(NULL);
			}

			/* Si el clic es válido, buscamos el camino mínimo. */
			if (clicValido){
	            ///pruebas
	            //Coordenada mouse = Calculador::tileParaPixel(Coordenada(MouseX,MouseY),coord_pixel_ceros);
	            //std::cout << "mouse: "<<mouse.x<<";"<<mouse.y<<'\t'<<"follow: "<<follow.x<<";"<<follow.y<<std::endl;

	            try {
	        		Camino camino = Calculador::obtenerCaminoMin(escenario, coord_pixel_sprite, Coordenada(MouseX, MouseY), coord_pixel_ceros);

					if (camino.size() > 0) {
						/* Si se está jugando en red, enviar el movimiento a los demás jugadores. */
						if (juego->esCliente())
							Proxy::enviar(juego->getConnection(), camino);
						/* Activamos el movimiento del sprite y seteamos el nuevo camino que debe recorrer. */
						sprite->setearNuevoCamino(camino, coord_pixel_ceros);
					}
				} catch ( FueraDeEscenario &e ) {}
			}
		}
	} /* Fin if SDL_MOUSEBUTTONDOWN */

	if (sprite->estaEnMovimiento())		//verificar que esta optimización no rompa nada
		sprite->update(juego->getVelocidad());

}

ControladorMouse::~ControladorMouse() {
	// TODO Auto-generated destructor stub
}

