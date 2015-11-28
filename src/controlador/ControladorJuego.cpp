/*
 * ControladorJuego.cpp
 *
 *  Created on: 14 de nov. de 2015
 *      Author: emanuel
 */

#include "ControladorJuego.h"
//#include "../red/Proxy.h"

ControladorJuego::ControladorJuego(Juego* juego) {
	this->juego = juego;
}

void ControladorJuego::actualizarJuego(Mouse* mouse){
	/* Se despeja la niebla para todas las unidades del jugador */
	juego->getEscenario()->getCapa()->actualizarCapaGris();

	int idJug_local = juego->getIDJugador();
	Coordenada coord_ceros = juego->getCoordCeros();
	vector<Sprite*> spritesUnidades = juego->getSpritesUnidades();
	for (vector<Sprite*>::iterator it = spritesUnidades.begin(); it < spritesUnidades.end(); ++it) {
		if ((*it)->estaEnMovimiento()) {
			try {
				(*it)->update(juego->getVelocidad(), juego->getSonidoTipo(CAMINAR));
			} catch ( PasoCompletado &e ) {
				//Proxy::completePaso(lan, e.id);
			}
		}
		try {	//¿mover todo esto adentro del if estaEnMovimiento() de arriba? (MC)
			Coordenada coord_tile_sprite = Calculador::tileParaPixel((*it)->getPosPies(), coord_ceros);
			juego->getEscenario()->actualizarPosicionParaEntidad(coord_tile_sprite, (*it)->getEntidad());

			if ((*it)->getEntidad()->perteneceAJugador(idJug_local)) {
				/********** Actualización de la capa negra ***********/
				juego->getEscenario()->getCapa()->descubrirDesdePunto(coord_tile_sprite.x, coord_tile_sprite.y);
			}
		} catch ( FueraDeEscenario &e ) {}
	}
}

ControladorJuego::~ControladorJuego() {
	// TODO Auto-generated destructor stub
}

