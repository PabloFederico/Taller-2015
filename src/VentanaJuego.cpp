/*
 * VentanaJuego.cpp
 *
 *  Created on: 2 de sept. de 2015
 *      Author: emanuel
 */

#include "VentanaJuego.h"
#include "Loader.h"
#include "Constantes.h"
#include <SDL2/SDL_image.h>

VentanaJuego::VentanaJuego(Juego *juego):Ventana(juego){
	this->MARGEN_SCROLL = 30;
	this->TILES_X = 1;
	this->TILES_Y = 1;
	this->velocidad_personaje = 1.0;
	this->calculador = NULL;
	this->dibujador = NULL;
	this->contenedor = NULL;
	this->cero_x = NULL;
	this->cero_y = NULL;
	this->spritePlayer = NULL;
	this->protagonista = NULL;
	this->scroll = NULL;
	this->capa = NULL;
	this->procesador = NULL;
	this->juego = juego;
	this->cargarJuego(juego);
}

/********************************************************************************/
void VentanaJuego::cargarJuego(Juego *juego){
	/*pair<int,int> dimensionVentana = juego->dimensionVentana();
	this->SCREEN_WIDTH = dimensionVentana.first;
	this->SCREEN_HEIGHT = dimensionVentana.second;
	*/
	this->MARGEN_SCROLL = juego->getMargenScroll();

	this->protagonista = juego->getProtagonista();

	this->TILES_X = juego->getEscenario()->getDimension().first;
	this->TILES_Y = juego->getEscenario()->getDimension().second;

	int LIMITE_DESPLAZAMIENTO_EN_X = ANCHO_PIXEL_PASTO * this->TILES_X / 2;
	int LIMITE_DESPLAZAMIENTO_EN_Y = ALTO_PIXEL_PASTO * this->TILES_Y / 2;

	this->velocidad_personaje = juego->getVelocidad();

	if (init()){
		/* El (0,0) relativo del mapa respecto a la ventana principal */
		int centro_x = SCREEN_WIDTH / 2;
		int centro_y = SCREEN_HEIGHT / 2;
		this->cero_x = new int(centro_x - DISTANCIA_ENTRE_X);
		this->cero_y = new int(centro_y - LIMITE_DESPLAZAMIENTO_EN_Y);

		this->calculador = new Calculador(this->cero_x, this->cero_y, juego->getEscenario()->getDimension());

		this->procesador = new Procesador(this->calculador, velocidad_personaje);

		this->scroll = new Scroll(cero_x,cero_y,SCREEN_WIDTH,SCREEN_HEIGHT,MARGEN_SCROLL);
		this->scroll->setearLimites(LIMITE_DESPLAZAMIENTO_EN_X,LIMITE_DESPLAZAMIENTO_EN_Y);

		this->capa = new CapaNegra(TILES_X,TILES_Y);

		this->contenedor = new ContenedorDeRecursos(this->renderer,this->calculador);

		/* Las imagenes , los sprites y los SDL_Rect (para cada entidad)
		 * se cargar en el contenedor */
		this->cargarImagenes(juego->getInfoTiposEntidades());
		this->cargarPosicionesEntidades(juego->getEscenario()->getVectorEntidades());

		this->dibujador = new Dibujador(renderer,contenedor,cero_x,cero_y);
	}
}

/********************************************************************************/
void VentanaJuego::cargarImagenes(vector<InfoEntidad> vectorInfo){
	this->contenedor->cargarImagenesEntidades(vectorInfo);
}

/********************************************************************************/
void VentanaJuego::cargarPosicionesEntidades(vector<PosEntidad>* posEntidades){
	this->contenedor->generarYGuardarSpritesEntidades(posEntidades);
	Sprite *sprite = this->contenedor->getSpriteDeEntidad(this->protagonista);
	this->spritePlayer = sprite;
}

/********************************************************************************/
void VentanaJuego::dibujar(){
	SDL_RenderClear(this->renderer);

	dibujador->dibujarRelieve(TILES_X,TILES_Y);
	dibujador->dibujarEntidades();
	dibujador->dibujarProtagonista(spritePlayer);
	dibujador->dibujarCapaNegra(capa);
}

/********************************************************************************/
void VentanaJuego::mostrar(){
		float dt = 0.03;
/*
		Uint32 frame_ant = 0;
		Uint32 frame_act = 0;
		Uint32 mil_fps = 1000/this->spritePlayer->getFps();
*/
		bool run = true;
		SDL_Event event;
		SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
		if (cursor == NULL) printf("Fallo la creacion del cursor %s",SDL_GetError());
		SDL_SetCursor(cursor);
		float posX_player = float(this->spritePlayer->getPosicion().x);
		float posY_player = float(this->spritePlayer->getPosicion().y);

		int MouseX, MouseY;

		/* Indica si el protagonista debe seguir moviendose */
		bool Follow = false;

	    /* Siguiente punto a donde debería moverse el protagonista */
	    int Follow_Point_X;
	    int Follow_Point_Y;

	    int x_anterior;
	    int y_anterior;
	    //frame_act = SDL_GetTicks();


		while ( run ){
			/*	frame_ant = frame_act;
				frame_act = SDL_GetTicks();
				dt = (float) (frame_act - frame_ant)/1000;
			*/
				SDL_PollEvent(&event);

				if (event.type == SDL_QUIT) run = false;

				SDL_GetMouseState(&MouseX,&MouseY);

				this->procesador->procesarClick(event,MouseX,MouseY,spritePlayer,
						  	  	  	  	  	   posX_player,posY_player,
											   x_anterior,y_anterior,
											   Follow_Point_X,Follow_Point_Y,Follow,dt);

				/* Actualiza la capa negra */
				int x = spritePlayer->getPosicion().x;
				int y = spritePlayer->getPosicion().y;
				pair<int,int> coord = this->calculador->calcularPosicionInversa(x,y);
				capa->descubrirDesdePunto(coord.first,coord.second);

				/* Analisis de Scrolling */
				int orig_inicial_x = *this->cero_x;
				int orig_inicial_y = *this->cero_y;

				this->scroll->procesarScroll(MouseX,MouseY,spritePlayer,
											 posX_player,posY_player,
											 x_anterior,y_anterior,
											 Follow_Point_X,Follow_Point_Y);

				/* Analizamos si hubo algún corrimiento del scroll.
				 * si corrimiento_x ó corrimiento_y son distintos de cero,
				 * entonces hubo un corrimiento. */
				int corrimiento_x = *this->cero_x - orig_inicial_x;
				int corrimiento_y = *this->cero_y - orig_inicial_y;

				if (corrimiento_x != 0 || corrimiento_y != 0){
					this->actualizarPosicionesEntidades(corrimiento_x, corrimiento_y);
				}


	            /* Actualiza el renderer */
	            this->dibujar();

	            SDL_RenderPresent(this->renderer);

	            SDL_Delay(15);

	            if (event.type == SDL_KEYDOWN){
	            	if (event.key.keysym.sym == 'r'){
	            		this->reiniciar();
	            		posX_player = this->spritePlayer->getPosicion().x;
	            		posY_player = this->spritePlayer->getPosicion().y;
	            		x_anterior = posX_player;
	            		y_anterior = posY_player;
	            		Follow = false;
	            	}
	            }

		} /* Fin del while*/
}

/********************************************************************************/
void VentanaJuego::actualizarPosicionesEntidades(int corrimiento_x, int corrimiento_y){
	this->contenedor->actualizarPosicionesEntidades(corrimiento_x, corrimiento_y);
}

/********************************************************************************/
void VentanaJuego::reiniciar(){
	delete this->juego;
	this->liberarRecursos();
	this->juego = new Juego();
	this->cargarJuego(this->juego);
}

/********************************************************************************/
void VentanaJuego::liberarRecursos(){
	delete this->spritePlayer;

	delete this->calculador;

	delete this->scroll;
	delete this->capa;
	delete this->procesador;
	delete this->contenedor;

	delete this->dibujador;

	delete this->cero_x;
	delete this->cero_y;

	this->close();
}


/********************************************************************************/
VentanaJuego::~VentanaJuego() {
	this->liberarRecursos();
	delete this->juego;
}
