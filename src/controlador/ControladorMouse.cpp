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

//void ControladorMouse::procesarEvento(SDL_Event &event, int MouseX, int MouseY, Connection* lan){
void ControladorMouse::procesarMouse(Mouse* mouse){
	/*********** Análisis del clic del mouse *************/
	if (mouse->click()){
		// Analizar tipo de click
		switch (mouse->getEstado()){
			case CLICK_IZQUIERDO: procesarClickIzquierdo(mouse);
								  break;
			case CLICK_DERECHO:   procesarClickDerecho(mouse);
								  break;
			case CLICK_DER_MOV:   procesarArrastreClickDerecho(mouse);
								  break;
			case CLICK_IZQ_MOV:   break;
			default : break;
		}

		mouse->setEstado(NO_CLICK);
	}
}
/*
		if (mouse->getEstado() == CLICK_IZQUIERDO){
			bool clicValido;
			Coordenada coord_pixel_ceros = juego->getCoordCeros();
			Escenario *escenario = juego->getEscenario();

			int limiteY_camara = juego->getDimensionVentana().second - juego->getBarraEstado()->getDimension().second;

			// Para que no cliquee fuera de la camara
			if (limiteY_camara < mouse->getXY().y) clicValido = false;
			else{
					try{
						Coordenada c_tile_clic = Calculador::tileParaPixel(mouse->getXY(), coord_pixel_ceros);
						clicValido = Calculador::puntoContenidoEnEscenario(c_tile_clic, escenario);
						//Seteo tile clic:
						Tile* tile_clic = escenario->getTile(c_tile_clic.x, c_tile_clic.y);
						escenario->setearTileClic(tile_clic, c_tile_clic);
						escenario->setearCoordTileClic(c_tile_clic);

					}catch(FueraDeEscenario &e) {
						clicValido = false;
						escenario->setearTileClic(NULL, Coordenada(0,0));
					}
			}
			// Si el clic es válido, buscamos el camino mínimo.
			if (clicValido){
	            ///pruebas
	            //Coordenada mouse = Calculador::tileParaPixel(Coordenada(MouseX,MouseY),coord_pixel_ceros);
	            //std::cout << "mouse: "<<mouse.x<<";"<<mouse.y<<'\t'<<"follow: "<<follow.x<<";"<<follow.y<<std::endl;
				if (escenario->getEntidadSeleccionada() != NULL) {
				  if (escenario->getEntidadSeleccionada()->getIDJug() == juego->getIDJugador()) {
					Sprite* spriteUnidad = juego->getSpritesEntidades()->find(escenario->getEntidadSeleccionada())->second;
					Coordenada coord_pixel_sprite = spriteUnidad->getPosPies();
					try {
						Camino camino = Calculador::obtenerCaminoMin(escenario, coord_pixel_sprite, mouse->getXY(), coord_pixel_ceros);

						if (camino.size() > 0) {
							// Si se está jugando en red, enviar el movimiento a los demás jugadores.
							//if (juego->esCliente())
								//Proxy::enviar(juego->getConnection(), camino);
							//else
								spriteUnidad->setearNuevoCamino(camino, coord_pixel_ceros);
								// Activamos localmente el movimiento del sprite y seteamos el nuevo camino que debe recorrer.
						}
					} catch ( FueraDeEscenario &e ) {}
				  }
				}

				if (escenario->getEntidadSeleccionada() != NULL){
					juego->getBarraEstado()->setInformacion(escenario->getEntidadSeleccionada()->getInfo());
				}
			} // Fin clicValido
		}
		mouse->setEstado(NO_CLICK);
	} // Fin if SDL_MOUSEBUTTONDOWN
}
*/

bool ControladorMouse::procesarClickEnVentana(Mouse* mouse, Tile** tile_clic, Coordenada* c_tile_clic) {
	//bool clicValido;
	bool clicSobreBarra = false ;
	Coordenada coord_pixel_ceros = juego->getCoordCeros();
	Escenario *escenario = juego->getEscenario();

	int limiteY_camara = juego->getDimensionVentana().second - juego->getBarraEstado()->getDimension().second;

	if (limiteY_camara < mouse->getXY().y) clicSobreBarra = true;

	int cant_unid_seleccionadas = juego->getJugador()->getUnidadesSeleccionadas().size();

	if (cant_unid_seleccionadas > 0 && clicSobreBarra){
		// Agregar que va a hacer (contruír, tomar alguna herramienta, etc) // Procesar click en Barra
	}
	if (cant_unid_seleccionadas == 0 && !clicSobreBarra){
			try{
				*c_tile_clic = Calculador::tileParaPixel(mouse->getXY(), coord_pixel_ceros);
				Calculador::puntoContenidoEnEscenario(*c_tile_clic, escenario);
				//Seteo tile clic:
				*tile_clic = escenario->getTile(c_tile_clic->x, c_tile_clic->y);
				escenario->setearCoordTileClic(*c_tile_clic);

			}catch(FueraDeEscenario &e) {
				escenario->setearTileClic(NULL, Coordenada(0,0));
				*tile_clic = NULL;
				return false;
			}
	}
	return true;
}

void ControladorMouse::procesarClickIzquierdo(Mouse* mouse){
	Tile* tile_clic = NULL;
	Coordenada c_tile_clic;

	if (procesarClickEnVentana(mouse, &tile_clic, &c_tile_clic))
		juego->getEscenario()->setearTileClic(tile_clic, c_tile_clic);
}

void ControladorMouse::procesarClickDerecho(Mouse* mouse){
	Coordenada coord_pixel_ceros = juego->getCoordCeros();
	Escenario *escenario = juego->getEscenario();

	// dependiendo si ahora elige una entidad o no, mediante obtenerEntidadOcupadoraEnTile, mover o interactuar TODO-ING

	if (escenario->getEntidadSeleccionada() != NULL) {
	  if (escenario->getEntidadSeleccionada()->getIDJug() == juego->getIDJugador()) {
		Sprite* spriteUnidad = juego->getSpritesEntidades()->find(escenario->getEntidadSeleccionada())->second;
		Coordenada coord_pixel_sprite = spriteUnidad->getPosPies();
		try {
			Camino camino = Calculador::obtenerCaminoMin(escenario, coord_pixel_sprite, mouse->getXY(), coord_pixel_ceros);

			if (camino.size() > 0) {
				/* Si se está jugando en red, enviar el movimiento a los demás jugadores. */
				//if (juego->esCliente())
					//Proxy::enviar(juego->getConnection(), camino);
				//else
					spriteUnidad->setearNuevoCamino(camino, coord_pixel_ceros);
					/* Activamos localmente el movimiento del sprite y seteamos el nuevo camino que debe recorrer. */
			}
		} catch ( FueraDeEscenario &e ) {}
	  }
	}

	if (escenario->getEntidadSeleccionada() != NULL){
		juego->getBarraEstado()->setInformacion(escenario->getEntidadSeleccionada()->getInfo());
	}
}

void ControladorMouse::procesarArrastreClickDerecho(Mouse* mouse){

}

ControladorMouse::~ControladorMouse() {
	// TODO Auto-generated destructor stub
}

