/*
 * ControladorMouse.cpp
 *
 *  Created on: 8 de oct. de 2015
 *      Author: emanuel
 */

#include "ControladorMouse.h"
#include "../utils/Calculador.h"
#include "../modelo/DetectorDeColisiones.h"
//#include "../red/Connection.h"


ControladorMouse::ControladorMouse(Juego *juego) {
	this->juego = juego;
}

void ControladorMouse::procesarEvento(SDL_Event &event, int MouseX, int MouseY){//, Connection* lan){

	Coordenada coord_pixel_ceros(*juego->getCeros().first + DISTANCIA_ENTRE_X, *juego->getCeros().second);

	/*********** Análisis del clic del mouse *************/
	if (event.type == SDL_MOUSEBUTTONDOWN && (event.button.button == SDL_BUTTON_LEFT || event.button.button == SDL_BUTTON_RIGHT)){

		bool clicValido = false;
		Tile* tile_clic;
		Coordenada c_tile_clic;
		Escenario *escenario = juego->getEscenario();

		int limiteY_camara = juego->getDimensionVentana().second - juego->getBarraEstado()->getDimension().second;

		/* Para que no cliquee fuera de la camara */
		if (limiteY_camara < MouseY) clicValido = false;
		else{
				try{
					c_tile_clic = Calculador::tileParaPixel(Coordenada(MouseX,MouseY), coord_pixel_ceros);
					clicValido = escenario->coordEnEscenario(c_tile_clic);

					tile_clic = escenario->getTile(c_tile_clic.x, c_tile_clic.y);
					escenario->setearCoordTileClic(c_tile_clic);	//en desuso

				}catch(FueraDeEscenario &e) {
					clicValido = false;
					escenario->setearTileClic(NULL, Coordenada(0,0));
				}
		}

		if (clicValido){

			if (event.button.button == SDL_BUTTON_LEFT) {

				escenario->setearTileClic(tile_clic, c_tile_clic);

				if (escenario->getEntidadSeleccionada() != NULL) {
					if (escenario->getEntidadSeleccionada()->getIDJug() == juego->getIDJugador()) {
						Sprite* spriteUnidad = juego->getSpritesEntidades()->find(escenario->getEntidadSeleccionada())->second;
						Coordenada coord_pixel_sprite = spriteUnidad->getPosPies();
						try {
							Camino camino = Calculador::obtenerCaminoMin(escenario, coord_pixel_sprite, Coordenada(MouseX, MouseY), coord_pixel_ceros);

							if (camino.size() > 0) {
								/* Si se está jugando en red, enviar el movimiento a los demás jugadores. */
								//if (juego->esCliente())
								//	Proxy::enviar(juego->getConnection(), camino);
								//else
									spriteUnidad->setearNuevoCamino(camino, coord_pixel_ceros);
									// /* Activamos localmente el movimiento del sprite y seteamos el nuevo camino que debe recorrer. */
							}
						} catch ( FueraDeEscenario &e ) {}
					}

					// Cargar info de la entidad seleccionada en la barra.
					juego->getBarraEstado()->setInformacion(escenario->getEntidadSeleccionada()->getInfo());
				}

			} else if (event.button.button == SDL_BUTTON_RIGHT) {
				// Si hay una Entidad propia preseleccionada, que interactúe con la Entidad recién cliqueada.
				if (escenario->getEntidadSeleccionada() != NULL && escenario->getEntidadSeleccionada()->getIDJug() == juego->getIDJugador()) {
					Entidad* entidadReceptora = escenario->obtenerEntidadOcupadoraEnTile(tile_clic);
					if (entidadReceptora != NULL)
						escenario->getEntidadSeleccionada()->interactuarCon(entidadReceptora);
				}
			}

		} /* Fin clicValido */

	} /* Fin if SDL_MOUSEBUTTONDOWN */


	vector<Sprite*> spritesProtagonistas = juego->getSpritesProtagonistas();
	for (vector<Sprite*>::iterator it = spritesProtagonistas.begin(); it < spritesProtagonistas.end(); ++it) {
		if ((*it)->estaEnMovimiento())
			try {
				(*it)->update(juego->getVelocidad());
			} catch ( PasoCompletado &e ) {
				//Proxy::completePaso(lan, e.id);
			}
		if ((*it)->getEntidad()->getIDJug() != juego->getIDJugador()) {
			try {
				Coordenada coord_tile_sprite = Calculador::tileParaPixel((*it)->getPosPies(), coord_pixel_ceros);
				juego->getEscenario()->actualizarPosicionEnemigo((*it)->getEntidad(), coord_tile_sprite);
			} catch ( FueraDeEscenario &e ) {}
		}
	}

}

ControladorMouse::~ControladorMouse() {
	// TODO Auto-generated destructor stub
}

