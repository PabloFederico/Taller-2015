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
		juego->getEscenario()->quitarRecuadroSeleccion();
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
		if (mouse->getEstado() != CLICK_DER_MOV)
			mouse->setEstado(NO_CLICK);
	}
}
/*
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
	bool clicEnMapa = true;
	bool clicSobreBarra = false ;
	Coordenada coord_pixel_ceros = juego->getCoordCeros();
	Escenario *escenario = juego->getEscenario();

	int limiteY_camara = juego->getDimensionVentana().second - juego->getBarraEstado()->getDimension().second;

	if (limiteY_camara < mouse->getXY().y) {
		clicSobreBarra = true;
		clicEnMapa = false;
	}

	int cant_unid_seleccionadas = juego->getJugador()->getUnidadesSeleccionadas().size();

	if (cant_unid_seleccionadas > 0 && clicSobreBarra){
		// Agregar que va a hacer (contruír, tomar alguna herramienta, etc) // Procesar click en Barra
		// Cambiar el estado de la unidad
	}
	if (clicEnMapa){
			juego->getJugador()->liberarUnidadesSeleccionadas();
			try{
				*c_tile_clic = Calculador::tileParaPixel(mouse->getXY(), coord_pixel_ceros);
				Calculador::puntoContenidoEnEscenario(*c_tile_clic, escenario);
				//Seteo tile clic:
				*tile_clic = escenario->getTile(c_tile_clic->x, c_tile_clic->y);
				escenario->setearCoordTileClic(*c_tile_clic);

				/*Coordenada c_tile_clic = Calculador::tileParaPixel(mouse->getXY(), coord_pixel_ceros);
				bool clicValido = Calculador::puntoContenidoEnEscenario(c_tile_clic, escenario);
				if (clicValido){
					//Seteo tile clic:
					Tile* tile_clic = escenario->getTile(c_tile_clic.x, c_tile_clic.y);
					escenario->setearTileClic(tile_clic, c_tile_clic);
					escenario->setearCoordTileClic(c_tile_clic);
					if (escenario->getEntidadSeleccionada() != NULL && juego->getIDJugador() == escenario->getEntidadSeleccionada()->getIDJug())
						juego->getJugador()->agregarUnidadSeleccionada((Unidad*)escenario->getEntidadSeleccionada());
				}*/
			}catch(FueraDeEscenario &e) {
				escenario->setearTileClic(NULL, Coordenada(0,0));
				*tile_clic = NULL;
				return false;
			}
	}
	return true;
	
	/*if (escenario->getEntidadSeleccionada() != NULL){
		juego->getBarraEstado()->setInformacion(escenario->getEntidadSeleccionada()->getInfo());
	} else juego->getBarraEstado()->setInformacion(" ");*/
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

	vector<Unidad*> unidades = juego->getJugador()->getUnidadesSeleccionadas();
	int cant_unid_seleccionadas = unidades.size();

	if (cant_unid_seleccionadas > 0) {
	  juego->getEscenario()->setearTileClic(NULL,Coordenada(0,0));
	  for (int i = 0; i <cant_unid_seleccionadas; i++){
		Sprite* spriteUnidad = juego->getSpritesEntidades()->find(unidades[i])->second;
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
	  } // End for
	} // End if
}

void ControladorMouse::procesarArrastreClickDerecho(Mouse* mouse){
	Coordenada coord_pixel_ceros = juego->getCoordCeros();
	Escenario* escenario = juego->getEscenario();
	juego->getJugador()->liberarUnidadesSeleccionadas();
	try {
		Coordenada c_tile_final = Calculador::tileParaPixel(mouse->getXY(),coord_pixel_ceros);
		Coordenada c_tile_inicial = Calculador::tileParaPixel(mouse->getXY_anterior(),coord_pixel_ceros);
		if (!Calculador::puntoContenidoEnEscenario(c_tile_inicial,escenario) ||
			!Calculador::puntoContenidoEnEscenario(c_tile_final,escenario))
			    throw FueraDeEscenario();
		if (c_tile_inicial != c_tile_final){

			juego->getEscenario()->agregarCoordenadasRecuadroSeleccion(c_tile_inicial,c_tile_final);
			juego->getEscenario()->setearTileClic(NULL,Coordenada(0,0));
			for (Coordenada c1 = c_tile_inicial; c1.y <= c_tile_final.y; c1.y++){
				for (Coordenada c2 = c1; c2.x <= c_tile_final.x ; c2.x++){
					Tile* tile = juego->getEscenario()->getTile(c2);
					vector<Entidad*> entidades = tile->getEntidades();

					for (unsigned i = 0; i < entidades.size(); i++){
						TipoEntidad tipo = entidades[i]->getTipo();
						int id = entidades[i]->getIDJug();
						// Se guardan las unidades seleccionadas en el Jugador
						if (id == juego->getIDJugador() && (tipo == ALDEANO || tipo == SOLDADO)){
							juego->getJugador()->agregarUnidadSeleccionada((Unidad*)entidades[i]);
							break;
						}
					}

				}
			}
		} // End if (c_tile_inicial != c_tile_final)
	}catch (FueraDeEscenario &e){}
}

ControladorMouse::~ControladorMouse() {
	// TODO Auto-generated destructor stub
}

