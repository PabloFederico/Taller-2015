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

VentanaJuego::VentanaJuego(Juego *juego){
	this->juego = juego;
	this->cargarJuego(juego);
}

/********************************************************************************/
void VentanaJuego::cargarJuego(Juego *juego){
	pair<int,int> dimensionVentana = juego->dimensionVentana();
	this->SCREEN_WIDTH = dimensionVentana.first;
	this->SCREEN_HEIGHT = dimensionVentana.second;

	this->MARGEN_SCROLL = juego->getMargenScroll();

	this->tipoProtagonista = juego->getProtagonista()->getTipo();

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

		/* map donde se almacenan los sprites que se van a usar.
		 * Es muy génerico, ya que también se guardar imagenes de PASTO, TIERRA, ETC. */
		this->mapSprites = new Map<TipoEntidad,Sprite*>();
		this->cargarImagenes(juego->getInfoTiposEntidades());

		this->dibujador = new Dibujador(renderer,mapSprites,cero_x,cero_y);
		this->dibujador->setMapInfoEntidad(this->mapInfoEntidades);

		this->vectorPosiciones = new vector<DataPos>();
		this->cargarPosicionesEntidades(juego->getEscenario()->getVectorEntidades());
	}
}

/********************************************************************************/
bool VentanaJuego::init(){
	 this->window = SDL_CreateWindow("Taller de Programación I: AoF",60,60,this->SCREEN_WIDTH,this->SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
	 if (this->window == NULL) return false;

	 this->renderer = SDL_CreateRenderer(this->window,-1,SDL_RENDERER_ACCELERATED);
	 if (this->renderer == NULL) return false;
	 else {
		  SDL_RenderSetLogicalSize(this->renderer,this->SCREEN_WIDTH,this->SCREEN_HEIGHT);
		  SDL_SetRenderDrawColor(this->renderer,0,0,0,0);
		 }
	return true;
}

/********************************************************************************/
void VentanaJuego::cargarImagenes(vector<InfoEntidad> vectorInfo){
	Loader loader;
	/* Primero cargamos la imagen del relieve por default */
	Imagen *pasto = loader.cargarImagen(this->renderer,"images/pasto.png");
	Sprite *spritePasto = new Sprite(1,1,pasto);
	this->mapSprites->insert(DEFAULT,spritePasto);

	for (unsigned i = 0; i < vectorInfo.size(); i++){
		TipoEntidad tipo = vectorInfo[i].tipo;
		string path = vectorInfo[i].path;

		/* Nos guardamos la información de las entidades */
		this->mapInfoEntidades[tipo] = vectorInfo[i];

		Imagen *imagen = loader.cargarImagen(this->renderer,path);
		Sprite *sprite;

		/* Guardamos una referencia directa al sprite protagonista */
		switch (tipo){
		case (SOLDADO):
			this->spritePlayer = new Sprite(DIRECCIONES,IMAGENES_DIFERENTES,imagen);
			this->spritePlayer->setDelay(vectorInfo[i].delay);
			this->spritePlayer->setFps(vectorInfo[i].fps);
			break;
		case (ANIMAL):
			sprite = new Sprite(DIRECCIONES, IMAGENES_DIFERENTES,imagen);
			sprite->setDelay(vectorInfo[i].delay);
			sprite->setFps(vectorInfo[i].fps);
			this->mapSprites->insert(tipo,sprite);
			break;
		default:
			/* Si no, guardamos en sprite en un map de sprites */
			sprite = new Sprite(vectorInfo[i].ancho,vectorInfo[i].ancho,imagen);
			this->mapSprites->insert(tipo,sprite);
			break;
		}
	}
}

/********************************************************************************/
void VentanaJuego::cargarPosicionesEntidades(vector<PosEntidad>* posEntidades){
	for (unsigned i = 0; i < posEntidades->size(); i++){
		int tile_x = (*posEntidades)[i].x;
		int tile_y = (*posEntidades)[i].y;
		Entidad* entidad = (*posEntidades)[i].entidad;
		pair<int,int> coordenada = this->calculador->calcularPosicionRelativa(tile_x,tile_y);

		SDL_Rect posicion;
    	/* Cargamos por default los siguientes valores para TIERRA ó AGUA */
		posicion.x = coordenada.first;
		posicion.y = coordenada.second;
		posicion.w = ANCHO_PIXEL_PASTO;
		posicion.h = ALTO_PIXEL_PASTO;

		switch (entidad->getTipo()){
			case ARBOL 	  : posicion.y -= (1.5 * DISTANCIA_ENTRE_Y);
							posicion.w = ANCHO_PIXEL_PASTO;
							posicion.h = 2 * ALTO_PIXEL_PASTO;
							break;
	    		/* Modificamos los tamanios de la imagen castillo para que ocupe
	    		 * los tiles que le corresponden */
			case CASTILLO : posicion.x -= DISTANCIA_ENTRE_X * (this->mapInfoEntidades[CASTILLO].ancho - 1);
						    posicion.y = posicion.y - ALTO_PIXEL_PASTO +  DISTANCIA_ENTRE_Y / 4;
						    posicion.w = ANCHO_PIXEL_PASTO;
						    posicion.h = (ALTO_PIXEL_PASTO * this->mapInfoEntidades[CASTILLO].ancho + ALTO_PIXEL_PASTO) / this->mapInfoEntidades[CASTILLO].ancho;
						    break;
			case SOLDADO  :
			case JUANA_DE_ARCO :
							posicion.x += ANCHO_PIXEL_PASTO / 4;
							posicion.w = ANCHO_PIXEL_PASTO / 3;
							posicion.h = ALTO_PIXEL_PASTO * 3 / 4;
							break;
			case ANIMAL :
							posicion.x += ANCHO_PIXEL_PASTO / 4;
							posicion.y -= (DISTANCIA_ENTRE_Y);
							posicion.w = ANCHO_PIXEL_PASTO;
							posicion.h = 2 * ALTO_PIXEL_PASTO;
							break;
	    	default       : break;
	    }

		/* Guardamos una referencia directa al SDL_Rect del protagonista */
		if (entidad->getTipo() == this->tipoProtagonista) {
			this->posicionPlayer = posicion;
	    }
		else{
			/* Guardamos la posicion de la entidad (SDL_Rect) y el tipo de entidad*/
			DataPos data(posicion,entidad->getTipo());
			this->vectorPosiciones->push_back(data);
	    }
	}
}

/********************************************************************************/
void VentanaJuego::close(){
	SDL_DestroyRenderer(this->renderer);
	this->renderer = NULL;

	SDL_DestroyWindow(this->window);
	this->window = NULL;
}

/********************************************************************************/
void VentanaJuego::dibujar(){
	SDL_RenderClear(this->renderer);

	dibujador->dibujarRelieve(TILES_X,TILES_Y);
	dibujador->dibujarEntidadesNoMovibles(vectorPosiciones);
	dibujador->dibujarProtagonista(posicionPlayer,spritePlayer);
}

/********************************************************************************/
void VentanaJuego::mostrar(){
		float dt;
		Uint32 frame_ant = 0;
		Uint32 frame_act = 0;
		Uint32 mil_fps = 1000/this->spritePlayer->getFps();
		bool run = true;
		SDL_Event event;
		SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
		if (cursor == NULL) printf("Fallo la creacion del cursor %s",SDL_GetError());
		SDL_SetCursor(cursor);
		float posX_player = float(this->posicionPlayer.x);
		float posY_player = float(this->posicionPlayer.y);

		int MouseX, MouseY;

		/* Indica si el protagonista debe seguir moviendose */
		bool Follow = false;

	    /* Siguiente punto a donde debería moverse el protagonista */
	    int Follow_Point_X;
	    int Follow_Point_Y;

	    int x_anterior;
	    int y_anterior;
	    frame_act = SDL_GetTicks();


		while (run && event.type != SDL_QUIT){
				frame_ant = frame_act;
				frame_act = SDL_GetTicks();
				dt = (float) (frame_act - frame_ant)/1000;

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

	            if (mil_fps > (SDL_GetTicks() - frame_act)) SDL_Delay(mil_fps -(SDL_GetTicks() - frame_act));
	            //SDL_Delay(1000/this->spritePlayer->getFps());

	            if (event.type == SDL_KEYDOWN){
	            	if (event.key.keysym.sym == 'r'){
	            		this->reiniciar();
	            		posX_player = this->posicionPlayer.x;
	            		posY_player = this->posicionPlayer.y;
	            		x_anterior = posX_player;
	            		y_anterior = posY_player;
	            		Follow = false;
	            	}
	            }

		} /* Fin del while*/
}

/********************************************************************************/
void VentanaJuego::actualizarPosicionesEntidades(int corrimiento_x, int corrimiento_y){
	vector<DataPos> *vect = new vector<DataPos>();

	for (unsigned i = 0; i < this->vectorPosiciones->size(); i++){
		DataPos data = (*vectorPosiciones)[i];
		data.posicion.x += corrimiento_x;
		data.posicion.y += corrimiento_y;
		vect->push_back(data);
	}
	delete this->vectorPosiciones;
	this->vectorPosiciones = vect;
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
				this->posicionPlayer.x += cantidad;
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
				this->posicionPlayer.x -= cantidad;
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
				this->posicionPlayer.y += cantidad;
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
				this->posicionPlayer.y -= cantidad;
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
            	posicionPlayer.x = int(x_result);
            	posX_player = x_result;
            }

            if (posY_player != Follow_Point_Y) {
                float y_result = (posY_player - ((posY_player - Follow_Point_Y) / distance) * (this->velocidad_personaje) * dt);
                posicionPlayer.y = int(y_result);
                posY_player = y_result;
            }
            this->spritePlayer->efectuarMovimiento();
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
	this->mapInfoEntidades.clear();
	map<TipoEntidad,Sprite*>::iterator p = this->mapSprites->begin();
	while (p != this->mapSprites->end()){
		Imagen *imagen = (*p).second->getImagen();
		Sprite *sprite = (*p).second;
		p++;
		delete imagen;
		delete sprite;
	}

	delete this->spritePlayer;

	delete this->mapSprites;

	delete this->vectorPosiciones;

	delete this->calculador;

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
