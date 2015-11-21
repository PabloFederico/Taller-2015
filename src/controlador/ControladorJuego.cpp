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
	vector<Unidad*> unidades = juego->getJugador()->getUnidades();

	Coordenada coord_pixel_ceros(*juego->getCeros().first + DISTANCIA_ENTRE_X, *juego->getCeros().second);

	/* Se despeja la niebla para todas las unidades del jugador */
	juego->getEscenario()->getCapa()->actualizarCapaGris();

	for (unsigned i = 0; i < unidades.size(); i++) {
			Sprite* spriteUnidad = juego->getSpritesEntidades()->find(unidades[i])->second;
			/********** Actualización de la capa negra ***********/
			Coordenada coord_pixel_sprite = spriteUnidad->getPosPies();

			try {
				Coordenada coord_tile_sprite = Calculador::tileParaPixel(coord_pixel_sprite,coord_pixel_ceros);
				//juego->getEscenario()->actualizarPosicionProtagonista(coord_tile_sprite);
				juego->getEscenario()->actualizarPosicionParaEntidad(coord_tile_sprite,unidades[i]);
				juego->getEscenario()->getCapa()->descubrirDesdePunto(coord_tile_sprite.x, coord_tile_sprite.y);
			}catch (FueraDeEscenario &e) {}

			 /*Chequeo de si se come un recurso */
//			Escenario* escenario = juego->getEscenario();
//			BarraEstado* barra = juego->getBarraEstado();
//			Coordenada pos_unidad = unidades[i]->getPosicion();
//			Tile* tile = escenario->getTile(pos_unidad);
//			if (tile->tieneRecurso()){
//				Entidad* recurso = tile->devolverRecurso();
//				barra->agregarRecursoEconomico(recurso->getTipo());

				// Elimina el sprite del recurso TODO
//				Map<Entidad*,Sprite*>* mapaSprites = juego->getSpritesEntidades();
//				map<Entidad*,Sprite*>::iterator p = mapaSprites->find(recurso);
//				if (p != mapaSprites->end()) {
//					delete p->second;
//					mapaSprites->erase(recurso);
//				}
//				escenario->quitarRecurso(pos_unidad,recurso);
//				//Proxy::comiRecurso(lan, pos_unidad);
//			}
	}// end while

	vector<Sprite*> spritesUnidades = juego->getSpritesUnidades();
	for (vector<Sprite*>::iterator it = spritesUnidades.begin(); it < spritesUnidades.end(); ++it) {
		if ((*it)->estaEnMovimiento())
			try {
				(*it)->update(juego->getVelocidad(), juego->getSonidoTipo(CAMINAR));
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

	// CADA X TIEMPO? O CREARLE SU PROPIO THREAD? (MC) jugador->interaccionesDeUnidades(); TODO
}

ControladorJuego::~ControladorJuego() {
	// TODO Auto-generated destructor stub
}

