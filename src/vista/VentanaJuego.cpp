/*
 * VentanaJuego.cpp
 *
 *  Created on: 2 de sept. de 2015
 *      Author: emanuel
 */

#include "../vista/VentanaJuego.h"
#include <SDL2/SDL_image.h>
#include "../utils/Constantes.h"
#include "../vista/Camara.h"

VentanaJuego::VentanaJuego(Controller *controlador):Ventana(controlador){
	this->dibujador = NULL;
	this->cargarJuego(controlador->getJuego());
}

/********************************************************************************/
void VentanaJuego::cargarJuego(Juego *juego){
	int TILES_X = juego->getEscenario()->getDimension().first;
	int TILES_Y = juego->getEscenario()->getDimension().second;

	int LIMITE_DESPLAZAMIENTO_EN_X = ANCHO_PIXEL_PASTO * TILES_X / 2;
	int LIMITE_DESPLAZAMIENTO_EN_Y = ALTO_PIXEL_PASTO * TILES_Y / 2;

	if (init()){
		/* El (0,0) relativo del mapa respecto a la ventana principal */
		int centro_x = SCREEN_WIDTH / 2;
		int centro_y = SCREEN_HEIGHT / 2;
		int *cero_x = new int(centro_x - DISTANCIA_ENTRE_X);
		int *cero_y = new int(centro_y - LIMITE_DESPLAZAMIENTO_EN_Y);

		juego->setCeros(cero_x,cero_y);

		Camara* camara = new Camara(cero_x,cero_y);
		camara->setDimension(SCREEN_WIDTH,SCREEN_HEIGHT);
		camara->setMargenScrolling(juego->getMargenScroll());
		camara->setLimites(LIMITE_DESPLAZAMIENTO_EN_X,LIMITE_DESPLAZAMIENTO_EN_Y);

		this->controlador->agregarCamara(camara);

		this->dibujador = new Dibujador(renderer);
		this->dibujador->setOrigen(cero_x,cero_y);

		/* Las imagenes , los sprites y los SDL_Rect (para cada entidad)
		 * se cargar en el contenedor */

		this->cargarImagenesYSprites(juego);
	}
}


/********************************************************************************/
void VentanaJuego::cargarImagenesYSprites(Juego* juego){
	ContenedorDeRecursos * contenedor = new ContenedorDeRecursos(renderer);
	contenedor->cargarImagenesEntidades(juego->getInfoTiposEntidades());

	vector<PosEntidad>* posEntidades = juego->getEscenario()->getVectorEntidades();
	contenedor->generarYGuardarSpritesEntidades(posEntidades,juego->getCeros(),juego->getEscenario());

	this->controlador->getJuego()->cargarRecursos(contenedor);
	this->dibujador->setContenedorDeRecursos(contenedor);
}

/********************************************************************************/
void VentanaJuego::dibujar(){
	SDL_RenderClear(this->renderer);

	int ancho = controlador->getJuego()->getEscenario()->getDimension().first;
	int largo = controlador->getJuego()->getEscenario()->getDimension().second;
	CapaNegra* capa = controlador->getJuego()->getEscenario()->getCapa();

	dibujador->dibujarRelieve(ancho,largo);
	dibujador->dibujarEntidades();
	dibujador->dibujarCapaNegra(capa);
}

/********************************************************************************/
void VentanaJuego::mostrar(){
		bool run = true;
		SDL_Event event;

		SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
		if (cursor == NULL) printf("Fallo la creacion del cursor %s",SDL_GetError());
		SDL_SetCursor(cursor);

		while ( run ){

				SDL_PollEvent(&event);

				if (event.type == SDL_QUIT) run = false;

				controlador->procesarEvento(event);

	            /* Actualiza el renderer */
	            this->dibujar();

	            SDL_RenderPresent(this->renderer);

	            SDL_Delay(15);

	            if (event.type == SDL_KEYDOWN){
	            	if (event.key.keysym.sym == KEY_RESET){
	            		this->reiniciar();
	            	}
	            }

		} /* Fin del while*/
}

/********************************************************************************/
void VentanaJuego::reiniciar(){
	this->controlador->reiniciarJuego();
	this->liberarRecursos();
	this->cargarJuego(controlador->getJuego());
}

/********************************************************************************/
void VentanaJuego::liberarRecursos(){
	delete this->dibujador;
	this->close();
}


/********************************************************************************/
VentanaJuego::~VentanaJuego() {
	this->liberarRecursos();
}
