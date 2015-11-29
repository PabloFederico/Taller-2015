/*
 * ControladorJuego.cpp
 *
 *  Created on: 14 de nov. de 2015
 *      Author: emanuel
 */

#include "ControladorJuego.h"
#include "../modelo/Flecha.h"
//#include "../red/Proxy.h"

ControladorJuego::ControladorJuego(Juego* juego) {
	this->juego = juego;
}

void ControladorJuego::actualizarJuego(Mouse* mouse){
	/* Se despeja la niebla para todas las unidades del jugador */
	CapaFog* capaFog = juego->getEscenario()->getCapa();
	capaFog->actualizarCapaGris();

	int idJug_local = juego->getIDJugador();
	Coordenada coord_ceros = juego->getCoordCeros();
	vector<Sprite*> spritesEntidadesJugadores = juego->getSpritesEntidadesJugadores();
	for (vector<Sprite*>::iterator it = spritesEntidadesJugadores.begin(); it < spritesEntidadesJugadores.end(); ++it) {
		if ((*it)->estaEnMovimiento()) {
			try {
				(*it)->update(juego->getVelocidad(), juego->getSonidoTipo(CAMINAR));
			} catch ( PasoCompletado &e ) {
				//Proxy::completePaso(lan, e.id);
			}
		}
		try {	//¿mover todo esto adentro del if estaEnMovimiento() de arriba? (MC)
			Entidad* entidad = (*it)->getEntidad();
			Coordenada coord_tile_sprite;
			if (entidad->esUnidad()){
				coord_tile_sprite = Calculador::tileParaPixel((*it)->getPosPies(), coord_ceros);
				juego->getEscenario()->actualizarPosicionParaEntidad(coord_tile_sprite, (*it)->getEntidad());
			}else
				 coord_tile_sprite = entidad->getPosicion();

			if (entidad->perteneceAJugador(idJug_local)) {
				/********** Actualización de la capa negra ***********/
				int ancho = entidad->getTam().first;
				int alto = entidad->getTam().second;
				for (Coordenada c1 = coord_tile_sprite; c1.x < coord_tile_sprite.x + ancho; c1.x++)
					for (Coordenada c2 = c1; c2.y < coord_tile_sprite.y + alto; c2.y++)
						capaFog->descubrirDesdePunto(c2.x, c2.y);
			}
		} catch ( FueraDeEscenario &e ) {}
	}

	// Actualización de objetos atacantes (FLECHA, etc(si agregamos más))
	map<Entidad*,Sprite*>::iterator itAtaque = juego->getMapObjetosAtacantes()->begin();
	while (itAtaque != juego->getMapObjetosAtacantes()->end()){
		Direccion direccion = ((Flecha*)itAtaque->first)->getDireccion();
		int cant_x = 0;
		int cant_y = 0;
		float factor_x;
		float factor_y;

		switch (direccion){
			case NORTE:
					  factor_x = 0.5;
					  factor_y = 0.2;
					  cant_y = -1;
					  break;
			case SUR:
					  factor_x = 0.5;
					  factor_y = 0.8;
					  cant_y = 1;
					  break;
			case ESTE:
					  factor_x = 0.8;
					  factor_y = 0.5;
					  cant_x = 1;
					  break;
			case OESTE:
					  factor_x = 0.2;
					  factor_y = 0.5;
					  cant_x = -1;
					  break;
			case NORESTE:
					  factor_x = 0.8;
					  factor_y = 0.2;
					  cant_x = 2;
					  cant_y = -1;
					  break;
			case NOROESTE:
					  factor_x = 0.2;
					  factor_y = 0.2;
					  cant_x = -2;
					  cant_y = -1;
					  break;
			case SURESTE:
					  factor_x = 0.8;
					  factor_y = 0.8;
					  cant_x = 2;
					  cant_y = 1;
					  break;
			default : // SUROESTE
				  factor_x = 0.2;
				  factor_y = 0.8;
				  cant_x = -2;
				  cant_y = 1;
				  break;
		}
		// multiplicamos por un factor de velocidad de flecha
		int factor_velociodad = 1;
		itAtaque->second->mover(cant_x*factor_velociodad,cant_y*factor_velociodad);

		SDL_Rect pos = itAtaque->second->getPosicion();
		Coordenada c_pixel_actual(pos.x + pos.w * factor_x, pos.y + pos.h * factor_y);
		try {
			Coordenada coord_tile = Calculador::tileParaPixel(c_pixel_actual, coord_ceros);
			itAtaque->first->setPosicion(coord_tile);
		} catch ( FueraDeEscenario &e ) {
			itAtaque->first->setPosicion(Coordenada(-1,-1));
		}

		itAtaque++;
	}
}

ControladorJuego::~ControladorJuego() {
	// TODO Auto-generated destructor stub
}

