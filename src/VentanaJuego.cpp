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
	this->LIMITE_DESPLAZAMIENTO_EN_X = 10;
	this->LIMITE_DESPLAZAMIENTO_EN_Y = 10;
	this->velocidad_personaje = 1.0;
	this->calculador = NULL;
	this->dibujador = NULL;
	this->contenedor = NULL;
	this->cero_x = NULL;
	this->cero_y = NULL;
	this->spritePlayer = NULL;
	this->protagonista = NULL;
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

	this->LIMITE_DESPLAZAMIENTO_EN_X = ANCHO_PIXEL_PASTO * this->TILES_X / 2;
	this->LIMITE_DESPLAZAMIENTO_EN_Y = ALTO_PIXEL_PASTO * this->TILES_Y / 2;

	this->velocidad_personaje = juego->getVelocidad();

	if (init()){
		/* El (0,0) relativo del mapa respecto a la ventana principal */
		int centro_x = SCREEN_WIDTH / 2;
		int centro_y = SCREEN_HEIGHT / 2;
		this->cero_x = new int(centro_x - DISTANCIA_ENTRE_X);
		this->cero_y = new int(centro_y - LIMITE_DESPLAZAMIENTO_EN_Y);

		this->calculador = new Calculador(this->cero_x, this->cero_y);

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

				this->procesarClick(event,MouseX,MouseY,
										  posX_player,posY_player,
										  x_anterior,y_anterior,
										  Follow_Point_X,Follow_Point_Y,Follow,dt);

				int orig_inicial_x = *this->cero_x;
				int orig_inicial_y = *this->cero_y;
				this->procesarScroll(MouseX,MouseY,
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
void VentanaJuego::procesarScroll(int MouseX, int MouseY,
								  float &posPlayerX, float &posPlayerY,
								  int &x_anterior, int &y_anterior,
								  int &followX, int &followY){

	if (MouseX < MARGEN_SCROLL || MouseX > this->SCREEN_WIDTH-MARGEN_SCROLL || MouseY < MARGEN_SCROLL || MouseY > this->SCREEN_HEIGHT- MARGEN_SCROLL){
		int cantidad = 0;
		/* La camara se mueve hacia la izquierda */
		if (MouseX < MARGEN_SCROLL && MouseX > 0){
			if (MouseX < MARGEN_SCROLL / 2) cantidad = 2 * MARGEN_SCROLL;
			else cantidad = MARGEN_SCROLL / 2;

			if ((*this->cero_x) < LIMITE_DESPLAZAMIENTO_EN_X ){
				*cero_x += cantidad;
				//this->posicionPlayer.x += cantidad;
				this->spritePlayer->mover(cantidad,0);
				posPlayerX += cantidad;
				x_anterior += cantidad;
				followX += cantidad;
			}
		}

		/* La camara se mueve hacia la derecha */
		if (MouseX > SCREEN_WIDTH - MARGEN_SCROLL && MouseX < SCREEN_WIDTH){
			if (MouseX > SCREEN_WIDTH - MARGEN_SCROLL / 2)
				cantidad = 2 * MARGEN_SCROLL;
			else cantidad = MARGEN_SCROLL / 2;

			if (SCREEN_WIDTH - *this->cero_x < LIMITE_DESPLAZAMIENTO_EN_X + ANCHO_PIXEL_PASTO){
				*cero_x -= cantidad;
				//this->posicionPlayer.x -= cantidad;
				this->spritePlayer->mover(-cantidad,0);
				posPlayerX -= cantidad;
				x_anterior -= cantidad;
				followX -= cantidad;
			}
		}

		/* La camara se mueve hacia arriba */
		if (MouseY < MARGEN_SCROLL && MouseY > 0){
			if (MouseY < MARGEN_SCROLL / 2) cantidad = 2 * MARGEN_SCROLL;
			else cantidad = MARGEN_SCROLL / 2;

			if (*this->cero_y < 0){
				*cero_y += cantidad;
				//this->posicionPlayer.y += cantidad;
				this->spritePlayer->mover(0,cantidad);
				posPlayerY += cantidad;
				y_anterior += cantidad;
				followY += cantidad;
			}
		}

		/* La camara se mueve hacia abajo */
		if (MouseY > SCREEN_HEIGHT - MARGEN_SCROLL && MouseY < SCREEN_HEIGHT){
			if (MouseY > SCREEN_HEIGHT - MARGEN_SCROLL / 2) cantidad = 2 * MARGEN_SCROLL;
			else cantidad = MARGEN_SCROLL / 2;

			if (abs(*this->cero_y) < 2*LIMITE_DESPLAZAMIENTO_EN_Y - SCREEN_HEIGHT){
				*cero_y -= cantidad;
				//this->posicionPlayer.y -= cantidad;
				this->spritePlayer->mover(0,-cantidad);
				posPlayerY -= cantidad;
				y_anterior -= cantidad;
				followY -= cantidad;
			}
		}
	}
}
/********************************************************************************/
void VentanaJuego::procesarClick(SDL_Event event, int MouseX, int MouseY,
												  float &posX_player, float &posY_player,
												  int &x_anterior, int &y_anterior,
												  int &Follow_Point_X, int &Follow_Point_Y, bool &Follow, float dt){

	//======Analisis del evento de movimiento======//
	if (event.type == SDL_MOUSEBUTTONDOWN){
		if (event.button.button == SDL_BUTTON_LEFT){
			SDL_Rect posicionPlayer = this->spritePlayer->getPosicion();
            Follow_Point_X = MouseX - posicionPlayer.w / 2;
            Follow_Point_Y = MouseY - posicionPlayer.h ;

            /* Validación de click dentro del escenario */
            if (this->calculador->puntoContenidoEnEscenario(Follow_Point_X+posicionPlayer.w,Follow_Point_Y+posicionPlayer.h,this->TILES_X, this->TILES_Y)){
                Follow = true;
                x_anterior = Follow_Point_X;
                y_anterior = Follow_Point_Y;
                Direccion direccion = this->calculador->calcularDireccion(Follow_Point_X, Follow_Point_Y, posicionPlayer.x, posicionPlayer.y);
                this->spritePlayer->setDireccion(direccion);
            }else {
            	/* Si el click esta fuera del escenario, su punto destino será
            	 * el anterior al click (si es que se encontraba en movimiento) */
            	Follow_Point_X = x_anterior;
            	Follow_Point_Y = y_anterior;
            }

		}
	}

    if (Follow) {
    	float distance = this->calculador->calcularDistancia(posX_player, posY_player, Follow_Point_X, Follow_Point_Y);

		if (distance > 1){
            if (posX_player != Follow_Point_X) {
            	float x_result = (posX_player - ((posX_player - Follow_Point_X) / distance) *(this->velocidad_personaje)  * dt);
            	//posicionPlayer.x = int(x_result);
            	this->spritePlayer->setPosX(int(x_result));
            	posX_player = x_result;
            }

            if (posY_player != Follow_Point_Y) {
                float y_result = (posY_player - ((posY_player - Follow_Point_Y) / distance) * (this->velocidad_personaje) * dt);
                //posicionPlayer.y = int(y_result);
                this-> spritePlayer->setPosY(int(y_result));
                posY_player = y_result;
            }
            if (this->spritePlayer->currentTime() > (1000/this->spritePlayer->getFps())){
            	this->spritePlayer->efectuarMovimiento();
            }

		}else  Follow = false;
    }else{
    	/* Cuando se deja de mover, se queda en una posición firme */
    	 this->spritePlayer->acomodar();
    }
    //=======Fin de analisis de evento de movimiento=========//
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
