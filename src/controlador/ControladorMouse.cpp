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
		if (juego->getBarraEstado()->getEntidadActualEnBarra() != NULL){
			juego->getBarraEstado()->setInformacion(juego->getBarraEstado()->getEntidadActualEnBarra()->getInfo());
		}else{
			if (juego->getEscenario()->getEntidadSeleccionada() != NULL){
				juego->getBarraEstado()->setInformacion(juego->getEscenario()->getEntidadSeleccionada()->getInfo());
			} else juego->getBarraEstado()->setInformacion(" ");
		}

	}else{
		if (mouse->estaMoviendoImagen()){
			// Si devuelve true, quiere decir que tiene un edificio seleccionado
			// para contruír, actualizamos la posición (posible) para la construcción
			try{
				Coordenada c_tile = Calculador::tileParaPixel(mouse->getXY(),juego->getCoordCeros());
				juego->getEscenario()->getEntidadTemporal()->setPosicion(c_tile);
			}catch (FueraDeEscenario &e){}

		}

	}
}


bool ControladorMouse::procesarClickEnVentana(Mouse* mouse, Tile** tile_clic, Coordenada* c_tile_clic) {
	bool clicEnMapa = true;
	Coordenada coord_pixel_ceros = juego->getCoordCeros();
	Escenario *escenario = juego->getEscenario();

	int limiteY_camara = juego->getDimensionVentana().second - juego->getBarraEstado()->getDimension().second;

	if (limiteY_camara < mouse->getXY().y) {
		return false;
	}

	if (clicEnMapa){	//chequeo completamente inútil
		// Si tiene un edificio para construír, lo agrega al escenario, generando su sprite correspondiente
		if (mouse->getEstado() == CLICK_IZQUIERDO && mouse->estaMoviendoImagen()){
			try {
				Entidad* entidad = escenario->getEntidadTemporal();
				Coordenada c = entidad->getPosicion();
				if (escenario->lugarHabilitadoParaConstruir(c,entidad)){
					mouse->setearMoviendoImagen(false);
					escenario->resetEntidadTemporal();
					TipoEntidad tipoEdif = ((Construccion*)entidad)->getTipoEdificio();
					delete entidad;

					entidad = juego->comenzarNuevaConstruccion(tipoEdif, c);
					if (!entidad) throw NoExiste();

					// TODO Las unidades seleccionadas anteriormente (aldeanos)
					//      deberían comenzar a interactuar
					Entidad* entidadReceptora = entidad;
					vector<Unidad*> unidades = juego->getJugador()->getUnidadesSeleccionadas();
					for (unsigned i = 0; i < unidades.size() && unidades[i]->esConstructor(); i++){
						Sprite* spriteUnidad = juego->getSpritesEntidades()->find(unidades[i])->second;
						Coordenada coord_pixel_sprite = spriteUnidad->getPosPies();
						// Por qué todo esto está copipeistiado del clic derecho, no deberían funcionar distinto?
						// En cualquier caso, por qué no llamar a la función del clic derecho, o modularizar la funcionalidad coincidente? (MC)
						try {
							Camino camino = Calculador::obtenerCaminoMin(escenario, coord_pixel_sprite, mouse->getXY(), coord_pixel_ceros);
							if (camino.size() > 0) {
								// Si yo muevo la(s) unidad(es), espero que deje de interactuar con su último receptor.
								unidades[i]->olvidarInteraccion();
								/* Activamos localmente el movimiento del sprite y seteamos el nuevo camino que debe recorrer. */
								if (entidadReceptora == NULL) {
									/* Si se está jugando en red, enviar el movimiento a los demás jugadores. */
									if (juego->esCliente())
										Proxy::enviar(juego->getConnection(), *unidades[i], camino);
							//		else
										spriteUnidad->setearNuevoCamino(camino, coord_pixel_ceros);	// para intentar mejorar el sincronismo
								}
							}

							// Si la hay, settear interacción con nueva entidad cliqueada.
							if (entidadReceptora != NULL) {
								if (juego->esCliente())
									Proxy::enviar(juego->getConnection(), *unidades[i], *entidadReceptora);
					//			else
									unidades[i]->interactuarCon(entidadReceptora);	// Intentando mejorar el sincronismo
							}
						} catch ( FueraDeEscenario &e ) {}
					}
				}
				return true;
			} catch ( NoExiste &e ) { /* No se pudo crear la construcción por alguna razón */ }
		}
		if (mouse->getEstado() == CLICK_IZQUIERDO) {
			juego->getJugador()->liberarUnidadesSeleccionadas();
			juego->getJugador()->liberarEdificioSeleccionado();
		}

		try{
			*c_tile_clic = Calculador::tileParaPixel(mouse->getXY(), coord_pixel_ceros);
			if (Calculador::puntoContenidoEnEscenario(*c_tile_clic, escenario)) {
				//Seteo tile clic:
				*tile_clic = escenario->getTile(c_tile_clic->x, c_tile_clic->y);
				//escenario->setearCoordTileClic(*c_tile_clic); En desuso
				return true;
			}
		}catch(FueraDeEscenario &e) {
			escenario->setearTileClic(NULL, Coordenada(0,0));	// No hay peligro en pasar esta coordenada?
			*tile_clic = NULL;
			return false;
		}
	}
	return false;
}

TipoEntidad iconoEdificioSeleccionado(Mouse* mouse, Juego* juego){
	TipoEntidad tipo = DEFAULT;
	Coordenada c_mouse = mouse->getXY();
	Imagen* imagenIconosEdificios = juego->getContenedorRecursos()->getImagenUtilTipo(HERRAMIENTAS_ALDEANO);
	int ancho_imagen = imagenIconosEdificios->getPixelsX();
	int alto_imagen = 30;
	int cant_iconos = 2; // (CUARTEL , BARRACA_ARQUERO)
	int ancho_icono = ancho_imagen / cant_iconos;
	int x_icono = 30;
	int y_icono = juego->getDimensionVentana().second - juego->getBarraEstado()->getDimension().second + 40;
	int edificio_int = (int)CUARTEL;
	// Va a recorrer BARRACK_1, BARRACK_2, BARRACK_3, CUARTEL (edificios que puede construír)
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

TipoEntidad iconoSeleccionado(Mouse* mouse, Juego* juego){
	TipoEntidad tipo = DEFAULT;
	Coordenada c_mouse = mouse->getXY();
	Imagen* imagenIconos = NULL;
	int cant_iconos;
	int index;
	TipoEntidad tipoEntidadSeleccionada = juego->getBarraEstado()->getEntidadActualEnBarra()->getTipo();
	switch (tipoEntidadSeleccionada){
	case ALDEANO:
				imagenIconos = juego->getContenedorRecursos()->getImagenUtilTipo(HERRAMIENTAS_ALDEANO);
				index = (int)CUARTEL;
				cant_iconos = 2;
				break;

	case CENTRO_CIVICO :
	case CUARTEL:
	case BARRACK :
				imagenIconos = juego->getContenedorRecursos()->getImagenUtilTipo(ARCO_ARQUERO);
				if (tipoEntidadSeleccionada == CENTRO_CIVICO) index = (int)ALDEANO;
				else if (tipoEntidadSeleccionada == CUARTEL) index = (int)SOLDADO;
				else index = (int)ARQUERO;
				cant_iconos = 1;
				break;

	default :
			return DEFAULT;
			break;
	}
	int ancho_imagen = imagenIconos->getPixelsX();
	int alto_imagen = 30;
	int ancho_icono = ancho_imagen / cant_iconos;
	int x_icono = 30;
	int y_icono = juego->getDimensionVentana().second - juego->getBarraEstado()->getDimension().second + 40;
	// Va a recorrer CUARTEL , BARRACK (edificios que puede construír) ó
	//				 ALDEANO, ARQUERO, SOLDADO
	for (int i = 0; i < cant_iconos; i++){
		if (c_mouse.x > x_icono &&
			c_mouse.x < x_icono + ancho_icono &&
			c_mouse.y > y_icono &&
			c_mouse.y < y_icono + alto_imagen){
			tipo = (TipoEntidad)index;
			break;
		}
		x_icono += ancho_icono;
		index++;
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
		if (escenario->getEntidadSeleccionada() != NULL &&
			juego->getIDJugador() == escenario->getEntidadSeleccionada()->getIDJug())
			// Puede agregar un edificio o unidad (para ejecutar acciones)
			juego->getJugador()->agregarEntidadSeleccionada(escenario->getEntidadSeleccionada());
	}else{
		// CASO CLICK EN LA BARRA DE ESTADO (si selecciona icono de edificio a contruír)
		BarraEstado* barra = juego->getBarraEstado();
		// TODO Hardcodeo mal (para procesar si se quiere contruír ó crear nueva unidad)
		if (barra->getEntidadActualEnBarra() != NULL){

			TipoEntidad tipoEntidadSeleccion = barra->getEntidadActualEnBarra()->getTipo();
			if (barra->getEntidadActualEnBarra()->esUnidad() && tipoEntidadSeleccion == ALDEANO){
				TipoEntidad edificioAConstruir = iconoSeleccionado(mouse,juego);
				if (edificioAConstruir != DEFAULT){
					//std::cout << "se eligió un edificio para contruír : "<<edificioAConstruir<<"\n";
					// TODO debería crearse una entidad tipo CONSTRUCCIÓN y cuando finalice convertirlo a edificio
					if (escenario->getEntidadTemporal() != NULL) delete escenario->getEntidadTemporal();
					Entidad* entidadConstruccion = new Construccion(edificioAConstruir,juego->getIDJugador());	// Llamar a juego->comenzarNuevaConstruccion() ???
					entidadConstruccion->setPosicion(Coordenada(0,0));
					entidadConstruccion->setTam(4,4); //hardcodeo prueba
					juego->getEscenario()->iniciarEntidadTemporal(entidadConstruccion);
					mouse->setearMoviendoImagen(true);
				}
			}else{
				// Es un edificio
				TipoEntidad unidadACrear = iconoSeleccionado(mouse,juego);
				if (unidadACrear != DEFAULT){
					if (!juego->crearNuevaUnidadApartirDeEdificioSeleccionado(unidadACrear))
						std::cout << "No se poseen recursos suficientes para crear la unidad."<<std::endl;
				}

			}

		}
	}
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
		juego->getEscenario()->setearTileClic(NULL,Coordenada(0,0)); //Para qué es esto??? (MC)
		for (int i = 0; i < cant_unid_seleccionadas; i++){

			// Moverse a una posición vacía
			Sprite* spriteUnidad = juego->getSpriteDeEntidad(unidades[i]); //juego->getSpritesEntidades()->find(unidades[i])->second;
			Coordenada coord_pixel_sprite = spriteUnidad->getPosPies();
			try {
				Camino camino = Calculador::obtenerCaminoMin(escenario, coord_pixel_sprite, mouse->getXY(), coord_pixel_ceros);
				if (camino.size() > 0) {
					// Si yo muevo la(s) unidad(es), espero que deje de interactuar con su último receptor.
					unidades[i]->olvidarInteraccion();
					/* Activamos localmente el movimiento del sprite y seteamos el nuevo camino que debe recorrer. */
					if (entidadReceptora == NULL) {
						/* Si se está jugando en red, enviar el movimiento a los demás jugadores. */
						if (juego->esCliente())
							Proxy::enviar(juego->getConnection(), *unidades[i], camino);
				//		else
							spriteUnidad->setearNuevoCamino(camino, coord_pixel_ceros);	// Intentando mejorar el sincronismo
					}
				}

				// Si la hay, settear interacción con nueva entidad cliqueada.
				if (entidadReceptora != NULL) {
					if (juego->esCliente())
						Proxy::enviar(juego->getConnection(), *unidades[i], *entidadReceptora);
			//		else
						unidades[i]->interactuarCon(entidadReceptora);	// Intentando mejorar el sincronismo
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
						if (id == juego->getIDJugador()){
							juego->getJugador()->agregarEntidadSeleccionada(entidades[i]);
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

