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
	pair<int,int> dimensionVentana = juego->dimensionVentana();
	this->SCREEN_WIDTH = dimensionVentana.first;
	this->SCREEN_HEIGHT = dimensionVentana.second;

	this->MARGEN_SCROLL = juego->getMargenScroll();

	this->tipoProtagonista = juego->getProtagonista()->getTipo();

	this->TILES_X = juego->getEscenario()->getDimension().first;
	this->TILES_Y = juego->getEscenario()->getDimension().second;

	this->LIMITE_DESPLAZAMIENTO_EN_X = ANCHO_PIXEL_PASTO * this->TILES_X / 2;
	this->LIMITE_DESPLAZAMIENTO_EN_Y = ALTO_PIXEL_PASTO * this->TILES_Y / 2;

	if (init()){

		/* El (0,0) relativo del mapa respecto a la ventana principal */
		int centro_x = SCREEN_WIDTH / 2;
		int centro_y = SCREEN_HEIGHT / 2;
		this->cero_x = new int(centro_x - DISTANCIA_ENTRE_X);
		this->cero_y = new int(centro_y - LIMITE_DESPLAZAMIENTO_EN_Y);

		this->calculador = new Calculador(this->cero_x, this->cero_y);

		/* map donde se almacenan las imágenes que se van a usar *
		this->mapImagenes = new std::map<std::string,Imagen*>();
		this->cargarImagenes(juego->getMapEntidades());
		
		this->vectorPosiciones = new vector<DataPos>();
		this->cargarPosicionesEntidades(this->escenario->getPosEntidades());

		/* Configurar de manera que se cargue el protagonista
		 * con su respectiva posicion*
		std::map<std::string,Imagen*>::iterator p = this->mapImagenes->find(juego->getProtagonista()->getTipo());
		Imagen* imagenPlayer = (*p).second;
		this->spritePlayer = new Sprite(8,6,imagenPlayer);

		this->posicionPlayer.y -= 5;		// Estos tres deberían ser también en proporción a escala_pixel_tile_
		this->posicionPlayer.w = 30;		// Por ejemplo el primero: "this->posicionPlayer.y -= this.escala_pixel_tile_y / 2;"
		this->posicionPlayer.h = 20;

		/* Borramos la referencia de la imagen del protagonista del map de imagenes
		 * y guardamos una referencia en imagenPlayer (en forma directa) *
		this->mapImagenes->erase(juego->getProtagonista()->getTipo());			// Esta implementación deberá revisarse cuando haya más de un personaje seleccionable
*/		
		/* map donde se almacenan las imagenes que se van a usar */
		this->mapSprites = new Map<TipoEntidad,Sprite*>();
		this->cargarImagenes(juego->getInfoTiposEntidades());

		this->vectorPosiciones = new vector<DataPos>();
		this->cargarPosicionesEntidades(juego->getEscenario()->getPosEntidades());
	}
	/* y si no init(), entonces deberíamos emitir un mensaje de error
	 * porque no se pudo inicializar SDL. */
}

/********************************************************************************/
bool VentanaJuego::init(){
	bool success = true;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) success = false;
	else{
		 this->window = SDL_CreateWindow("Taller de Programación I: AoF",60,60,this->SCREEN_WIDTH,this->SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
		 if (this->window == NULL) success = false;

		 this->renderer = SDL_CreateRenderer(this->window,-1,SDL_RENDERER_ACCELERATED);
		 if (this->renderer == NULL) success = false;
		 else {
			 SDL_RenderSetLogicalSize(this->renderer,this->SCREEN_WIDTH,this->SCREEN_HEIGHT);
			 SDL_SetRenderDrawColor(this->renderer,0,0,0,0);
		 }
	}
	return success;
}

/********************************************************************************
void VentanaJuego::cargarImagenes(std::map<std::string, VistaEntidad*> *entidades){
	std::map<std::string, VistaEntidad*>::iterator p = entidades->begin();
	while (p != entidades->end()){
		std::string tipo = (*p).first;
		VistaEntidad *ente = (*p).second;
		p++;
		Imagen *imagen = this->loader.cargarImagen(this->renderer,ente->getPath());
		this->mapImagenes->insert(std::make_pair(tipo,imagen));
	}
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
		if (tipo == this->tipoProtagonista){
			this->spritePlayer = new Sprite(DIRECCIONES,IMAGENES_DIFERENTES,imagen);
			this->spritePlayer->setDelay(vectorInfo[i].delay);
			this->spritePlayer->setFps(vectorInfo[i].fps);
		}else{
			/* Si no, guardamos en sprite en un map de sprites */
			sprite = new Sprite(vectorInfo[i].ancho,vectorInfo[i].ancho,imagen);
			this->mapSprites->insert(tipo,sprite);
		}
	}
}

/********************************************************************************/
void VentanaJuego::cargarPosicionesEntidades(Map<std::pair<int,int>, vector<Entidad*>* > *posEntidades){
	map<pair<int,int>, vector<Entidad*>* >::iterator p = posEntidades->begin();
	while (p != posEntidades->end()){

		vector<Entidad*> *vectorEntidades = (*p).second;
		int tile_x = (*p).first.first;
		int tile_y = (*p).first.second;
		//pair<int,int> coordenada = this->calculador->calcularPosicionRelativa(tile_x,tile_y);//this->posicionRelativa(tile_x,tile_y);

	    /* En una posición puede haber más de una entidad */
	    for (unsigned i = 0; i < vectorEntidades->size(); i++){
	    	Entidad *entidad = (*vectorEntidades)[i];

	    	pair<int,int> coordenada = this->calculador->calcularPosicionRelativa(tile_x,tile_y);//this->posicionRelativa(tile_x,tile_y);
	    	SDL_Rect posicion;
	    	/* Cargamos por default los siguientes valores para TIERRA ó AGUA */
	    	posicion.x = coordenada.first;
	    	posicion.y = coordenada.second;
	    	posicion.w = ANCHO_PIXEL_PASTO;
	    	posicion.h = ALTO_PIXEL_PASTO;

/*	    	DataPos data(posicion,entidad->getTipo());

	    	if (entidad->getTipo() == "soldado") {
	    		this->posicionPlayer = data.posicion;
	    	}
	    	else{
	    		this->vectorPosiciones->push_back(data);
	    	}
*/
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
	    		case SOLDADO  :	posicion.x += ANCHO_PIXEL_PASTO / 4;
	    						posicion.w = ANCHO_PIXEL_PASTO / 4;
	    						posicion.h = ALTO_PIXEL_PASTO * 3 / 4;
	    						break;
	    		case JUANA_DE_ARCO :
	    						posicion.x += ANCHO_PIXEL_PASTO / 4;
	    						posicion.w = ANCHO_PIXEL_PASTO / 2;
	    						posicion.h = ALTO_PIXEL_PASTO / 2;
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
	    p++;
	}
}

/********************************************************************************/
void VentanaJuego::close(){
	//delete this->imagenRelieve;

	SDL_DestroyRenderer(this->renderer);
	this->renderer = NULL;

	SDL_DestroyWindow(this->window);
	this->window = NULL;

	IMG_Quit();
	SDL_Quit();
}

/********************************************************************************/
//void VentanaJuego::render(){
void VentanaJuego::dibujar(){
	SDL_RenderClear(this->renderer);

	Imagen *imagenRelieve = this->mapSprites->find(DEFAULT)->second->getImagen();
	SDL_Rect posicion;

	int cero_relativo_x = *this->cero_x;
	int cero_relativo_y = *this->cero_y;

	posicion.w = ANCHO_PIXEL_PASTO;
	posicion.h = ALTO_PIXEL_PASTO;

	/* Dibujamos el relieve por Default */
	for(int j = 0; j < this->TILES_Y; j++){

		posicion.x = cero_relativo_x;
		posicion.y = cero_relativo_y;

		for(int i = 0; i < this->TILES_X; i++){
			SDL_RenderCopy(this->renderer,imagenRelieve->getTexture(),NULL,&posicion);

			posicion.x += DISTANCIA_ENTRE_X;
			posicion.y += DISTANCIA_ENTRE_Y;
		}
		cero_relativo_x -= DISTANCIA_ENTRE_X;
		cero_relativo_y += DISTANCIA_ENTRE_Y;
	}

	/* Dibujamos las entidades que no se mueven */
	for (unsigned i = 0; i < this->vectorPosiciones->size(); i++){
		DataPos data = (*vectorPosiciones)[i];
		SDL_Rect pos = data.posicion;
/*		std::map<std::string,Imagen*>::iterator itImg = this->mapImagenes->find(data.tipo);
		Imagen *imagenEntidad = itImg->second;
		SDL_RenderCopy(this->renderer,imagenEntidad->getTexture(),NULL,&pos);
*/
		TipoEntidad tipo = data.tipo;

		/* Buscamos la imagen de la entidad a traves del tipo */
		map<TipoEntidad,Sprite*>::iterator itImg = this->mapSprites->find(tipo);
		Imagen *imagenEntidad = (*itImg).second->getImagen();

		int x = pos.x;

		/* Estos solo se van a ejecutar una vez, salvo el caso de dibujar un castillo,
		 * eso dependera de cuantos tiles ocupe */
		for (int j = 0; j < itImg->second->cantidadDirecciones(); j++){
			for (int k = 0; k < itImg->second->cantidadImgDiferentes(); k++){
				SDL_Rect rect = itImg->second->getSDLRect(j,k);
				SDL_RenderCopy(this->renderer,imagenEntidad->getTexture(),&rect,&pos);
				pos.x += pos.w;
			}
			pos.x = x;
			pos.y += pos.h;
		}

	}

	/* Dibujamos al player */
	SDL_Texture* texturePlayer = this->spritePlayer->getImagen()->getTexture();
	SDL_Rect frame = this->spritePlayer->getSDLRectActual();
	SDL_RenderCopy(this->renderer,texturePlayer,&frame,&posicionPlayer);
}

/********************************************************************************/
void VentanaJuego::mostrar(){
	bool run = true;
	SDL_Event event;

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

		while (run && event.type != SDL_QUIT){

			SDL_PollEvent(&event);

			if (event.type == SDL_QUIT) run = false;
			SDL_GetMouseState(&MouseX,&MouseY);

/////* OLD
			/* Analisis del evento de movimiento
			if (event.type == SDL_MOUSEBUTTONDOWN){
				if (event.button.button == SDL_BUTTON_LEFT){
                    Follow_Point_X = MouseX - posicionPlayer.w / 2;
                    Follow_Point_Y = MouseY - posicionPlayer.h / 2;
                    Follow = true;
                    Direccion direccion = this->calculador->calcularDireccion(Follow_Point_X, Follow_Point_Y, posicionPlayer.x, posicionPlayer.y);
                    this->spritePlayer->setDireccion(direccion);
				}
			}

            if (Follow) {
            	float distance = this->calculador->calcularDistancia(posX_player, posY_player, Follow_Point_X, Follow_Point_Y);

				if (distance != 0){
                    if (posX_player != Follow_Point_X) {
                    	float x_result = (posX_player - ((posX_player - Follow_Point_X) / distance) * 1.5f);
                    	posicionPlayer.x = int(x_result);
                    	posX_player = x_result;
                    }

                    if (posY_player != Follow_Point_Y) {
                        float y_result = (posY_player - ((posY_player - Follow_Point_Y) / distance) * 1.5f);
                        posicionPlayer.y = int(y_result);
                        posY_player = y_result;
                    }
                    this->spritePlayer->efectuarMovimiento();
                 }else  Follow = false;
            }
            /* Fin de analisis de evento de movimiento */
/////

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

			this->procesarClick(event,MouseX,MouseY,
									  posX_player,posY_player,
									  x_anterior,y_anterior,
									  Follow_Point_X,Follow_Point_Y,Follow);


            /* Actualiza el renderer */
            this->dibujar();

            SDL_RenderPresent(this->renderer);

            SDL_Delay(1000/this->spritePlayer->getFps());

		}

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
			if (MouseX < MARGEN_SCROLL / 2) cantidad = MARGEN_SCROLL;
			else cantidad = MARGEN_SCROLL / 2;

			if ((*this->cero_x) < LIMITE_DESPLAZAMIENTO_EN_X){
				*cero_x += cantidad;
				this->posicionPlayer.x += cantidad;
				posPlayerX += cantidad;
				x_anterior += cantidad;
				followX += cantidad;
			}
		}

		/* La camara se mueve hacia la derecha */
		if (MouseX > SCREEN_WIDTH - MARGEN_SCROLL && MouseX < SCREEN_WIDTH){
			if (MouseX > SCREEN_WIDTH - MARGEN_SCROLL / 2) cantidad = MARGEN_SCROLL;
			else cantidad = MARGEN_SCROLL / 2;

			if (*this->cero_x > - LIMITE_DESPLAZAMIENTO_EN_X){
				*cero_x -= cantidad;
				this->posicionPlayer.x -= cantidad;
				posPlayerX -= cantidad;
				x_anterior -= cantidad;
				followX -= cantidad;
			}
		}

		/* La camara se mueve hacia arriba */
		if (MouseY < MARGEN_SCROLL && MouseY > 0){
			if (MouseX < MARGEN_SCROLL / 2) cantidad = MARGEN_SCROLL;
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
			if (MouseY > SCREEN_HEIGHT - MARGEN_SCROLL / 2) cantidad = MARGEN_SCROLL;
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
												  int &Follow_Point_X, int &Follow_Point_Y, bool &Follow){

	//======Analisis del evento de movimiento======//
	if (event.type == SDL_MOUSEBUTTONDOWN){
		if (event.button.button == SDL_BUTTON_LEFT){
            Follow_Point_X = MouseX - posicionPlayer.w / 2;
            Follow_Point_Y = MouseY - posicionPlayer.h / 2;

            /* Validación de click dentro del escenario */
            if (this->calculador->puntoContenidoEnEscenario(Follow_Point_X,Follow_Point_Y,this->TILES_X, this->TILES_Y)){
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
            	float x_result = (posX_player - ((posX_player - Follow_Point_X) / distance) * 1.5f);
            	posicionPlayer.x = int(x_result);
            	posX_player = x_result;
            }

            if (posY_player != Follow_Point_Y) {
                float y_result = (posY_player - ((posY_player - Follow_Point_Y) / distance) * 1.5f);
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
VentanaJuego::~VentanaJuego() {
/*	std::map<std::string,Imagen*>::iterator p = this->mapImagenes->begin();
	while (p != this->mapImagenes->end()){
		Imagen *imagen = (*p).second;	*/
	map<TipoEntidad,Sprite*>::iterator p = this->mapSprites->begin();
	while (p != this->mapSprites->end()){
		Imagen *imagen = (*p).second->getImagen();
		p++;
		delete imagen;

	}
	delete this->spritePlayer;

	//delete this->mapImagenes;
	delete this->mapSprites;

	delete this->vectorPosiciones;

	delete this->calculador;

	delete this->cero_x;
	delete this->cero_y;

	this->close();
}
