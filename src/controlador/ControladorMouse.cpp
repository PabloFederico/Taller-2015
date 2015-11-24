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

		// Actualizamos la barra
		if (juego->getEscenario()->getEntidadSeleccionada() != NULL){
			juego->getBarraEstado()->setInformacion(juego->getEscenario()->getEntidadSeleccionada()->getInfo());
		} else juego->getBarraEstado()->setInformacion(" ");
	}
}


bool ControladorMouse::procesarClickEnVentana(Mouse* mouse, Tile** tile_clic, Coordenada* c_tile_clic) {
	bool clicEnMapa = true;
	bool clicSobreBarra = false ;
	Coordenada coord_pixel_ceros = juego->getCoordCeros();
	Escenario *escenario = juego->getEscenario();

	int limiteY_camara = juego->getDimensionVentana().second - juego->getBarraEstado()->getDimension().second;

	if (limiteY_camara < mouse->getXY().y) {
		return false;
		//clicSobreBarra = true;
		//clicEnMapa = false;
	}

	int cant_unid_seleccionadas = juego->getJugador()->getUnidadesSeleccionadas().size();

	if (clicSobreBarra && cant_unid_seleccionadas > 0){
		// Agregar qué va a hacer (contruír, tomar alguna herramienta, etc) // Procesar click en Barra
		// Cambiar el estado de la unidad
	}
	if (clicEnMapa){
		if (mouse->getEstado() == CLICK_IZQUIERDO) juego->getJugador()->liberarUnidadesSeleccionadas();
		try{
			*c_tile_clic = Calculador::tileParaPixel(mouse->getXY(), coord_pixel_ceros);
			if (Calculador::puntoContenidoEnEscenario(*c_tile_clic, escenario)) {
				//Seteo tile clic:
				*tile_clic = escenario->getTile(c_tile_clic->x, c_tile_clic->y);
				//escenario->setearCoordTileClic(*c_tile_clic); En desuso
				return true;
			}
		}catch(FueraDeEscenario &e) {
			escenario->setearTileClic(NULL, Coordenada(0,0));
			*tile_clic = NULL;
			return false;
		}
	}
	return false;
}

TipoEntidad iconoSeleccionado(Mouse* mouse, Juego* juego){
	TipoEntidad tipo = DEFAULT;
	Coordenada c_mouse = mouse->getXY();
	Imagen* imagenIconosEdificios = juego->getContenedorRecursos()->getImagenUtilTipo(HERRAMIENTAS_ALDEANO);
	int ancho_imagen = imagenIconosEdificios->getPixelsX();
	int alto_imagen = 30;
	int cant_iconos = 4;
	int ancho_icono = ancho_imagen / cant_iconos;
	int x_icono = 30;
	int y_icono = juego->getDimensionVentana().second - juego->getBarraEstado()->getDimension().second + 40;
	int edificio_int = (int)BARRACK_1;
	// Va a recorrer BARRACK_1, BARRACK_2, BARRACK_3, CUARTEL
	for (int i = 0; i < cant_iconos; i++){
		if (c_mouse.x > x_icono &&
			c_mouse.x < x_icono + ancho_icono &&
			c_mouse.y > y_icono &&
			c_mouse.y < y_icono + alto_imagen){
			tipo = (TipoEntidad)edificio_int;
			break;
		}
		x_icono += ancho_icono;
		edificio_int++;
	}
	return tipo;
}

void ControladorMouse::procesarClickIzquierdo(Mouse* mouse){
	Escenario *escenario = juego->getEscenario();
	Tile *tile_clic = NULL;
	Coordenada c_tile_clic;

	if (procesarClickEnVentana(mouse, &tile_clic, &c_tile_clic)) {
		// Elige la entidad
		escenario->setearTileClic(tile_clic, c_tile_clic);
		if (escenario->getEntidadSeleccionada() != NULL && juego->getIDJugador() == escenario->getEntidadSeleccionada()->getIDJug())
			juego->getJugador()->agregarUnidadSeleccionada((Unidad*)escenario->getEntidadSeleccionada());
	}else{
		// CASO CLICK EN LA BARRA DE ESTADO (si selecciona icono de edificio a contruír)
		BarraEstado* barra = juego->getBarraEstado();
		// TODO Hardcodeo mal
		if (barra->getUnidadActualEnBarra() != NULL && barra->getUnidadActualEnBarra()->getTipo() == ALDEANO){
			TipoEntidad edificioAConstruir = iconoSeleccionado(mouse,juego);
			if (edificioAConstruir != DEFAULT){
				std::cout << "se eligió un edificio para contruír : "<<edificioAConstruir<<"\n";
			}
		}
	}

	// Actualizamos la barra (MC: lo dejaría acá (borrarlo en procesarMouse) para que solo una nueva selección cambie la información. No alcanza para que el sprite abajo a la izquierda quede.)
	//if (juego->getEscenario()->getEntidadSeleccionada() != NULL){
	//	juego->getBarraEstado()->setInformacion(juego->getEscenario()->getEntidadSeleccionada()->getInfo());
	//} else juego->getBarraEstado()->setInformacion(" ");
}

void ControladorMouse::procesarClickDerecho(Mouse* mouse){
	Coordenada c_tile_clic, coord_pixel_ceros = juego->getCoordCeros();
	Escenario *escenario = juego->getEscenario();
	Tile *tile_clic = NULL;

	vector<Unidad*> unidades = juego->getJugador()->getUnidadesSeleccionadas();
	int cant_unid_seleccionadas = unidades.size();

	if (cant_unid_seleccionadas > 0 && procesarClickEnVentana(mouse, &tile_clic, &c_tile_clic)) {
		escenario->setearTileClic(tile_clic, c_tile_clic);
		Entidad* entidadReceptora = escenario->getEntidadSeleccionada();
		juego->getEscenario()->setearTileClic(NULL,Coordenada(0,0)); //Para qué es esto???
		for (int i = 0; i < cant_unid_seleccionadas; i++){

			// Moverse a una posición vacía
			Sprite* spriteUnidad = juego->getSpritesEntidades()->find(unidades[i])->second;
			Coordenada coord_pixel_sprite = spriteUnidad->getPosPies();
			try {
				Camino camino = Calculador::obtenerCaminoMin(escenario, coord_pixel_sprite, mouse->getXY(), coord_pixel_ceros);
				if (camino.size() > 0) {
					/* Si se está jugando en red, enviar el movimiento a los demás jugadores. */
					//if (juego->esCliente())
						//Proxy::enviar(juego->getConnection(), camino);
					//else
						/* Activamos localmente el movimiento del sprite y seteamos el nuevo camino que debe recorrer. */
						spriteUnidad->setearNuevoCamino(camino, coord_pixel_ceros);
						// Si yo muevo la(s) unidad(es), espero que deje de interactuar con su último receptor.
						unidades[i]->olvidarInteraccion();
				}

				// Si la hay, settear interacción con nueva entidad cliqueada.
				if (entidadReceptora != NULL) {
					unidades[i]->interactuarCon(entidadReceptora);
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
						int id = entidades[i]->getIDJug();
						// Se guardan las unidades seleccionadas en el Jugador
						if (id == juego->getIDJugador() && entidades[i]->esUnidad()) {//(tipo == ALDEANO || tipo == SOLDADO || tipo == ARQUERO)){	// bastante hardcodeador; necesario?
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

