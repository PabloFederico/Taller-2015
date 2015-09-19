/*
 * VentanaJuego.cpp
 *
 *  Created on: 2 de sept. de 2015
 *      Author: emanuel
 */

#include "VentanaJuego.h"
#include <SDL2/SDL_image.h>

VentanaJuego::VentanaJuego(Juego *juego){
	std::pair<int,int> dimensionVentana = juego->dimensionVentana();
	this->SCREEN_WIDTH = dimensionVentana.first;
	this->SCREEN_HEIGHT = dimensionVentana.second;

	this->escenario = juego->getEscenario();
	this->TILES_X = this->escenario->getDimension().first;
	this->TILES_Y = this->escenario->getDimension().second;

	if (init()){

		/* El (0,0) relativo del mapa respecto a la ventana principal */
		/* la función "floor" es para que quede un numero entero */
		this->cero_x = (this->SCREEN_WIDTH / 2) - this->escala_pixel_tile_x;
		this->cero_y = (this->SCREEN_HEIGHT - this->TILES_Y) / (2 * this->escala_pixel_tile_y) - this->escala_pixel_tile_y;

		this->calculador = new Calculador(this->cero_x, this->cero_y, this->escala_pixel_tile_x, this->escala_pixel_tile_y);

		this->imagenRelieve = this->loader.cargarImagen(this->renderer,"images/pasto.png");

		/* map donde se almacenan las imagenes que se van a usar */
		this->mapImagenes = new std::map<std::string,Imagen*>();
		this->cargarImagenes(juego->getMapEntidades());
		
		this->vectorPosiciones = new vector<DataPos>();
		this->cargarPosicionesEntidades(this->escenario->getPosEntidades());

		/* Configurar de manera que se cargue el protagonista
		 * con su respectiva posicion*/
		std::map<std::string,Imagen*>::iterator p = this->mapImagenes->find(juego->getProtagonista()->getTipo());
		Imagen* imagenPlayer = (*p).second;
		this->spritePlayer = new Sprite(8,6,imagenPlayer);

		this->posicionPlayer.y -= 5;		// Estos tres deberían ser también en proporción a escala_pixel_tile_
		this->posicionPlayer.w = 30;		// Por ejemplo el primero: "this->posicionPlayer.y -= this.escala_pixel_tile_y / 2;"
		this->posicionPlayer.h = 20;

		/* Borramos la referencia de la imagen del protagonista del map de imagenes
		 * y guardamos una referencia en imagenPlayer (en forma directa) */
		this->mapImagenes->erase(juego->getProtagonista()->getTipo());			// Esta implementación deberá revisarse cuando haya más de un personaje seleccionable
	}
	// else ??
}

/********************************************************************************/
bool VentanaJuego::init(){
	bool success = true;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) success = false;
	else{
		 this->window = SDL_CreateWindow("AoF",60,60,this->SCREEN_WIDTH,this->SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
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

/********************************************************************************/
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
void VentanaJuego::cargarPosicionesEntidades(std::map<std::pair<int,int>, std::vector<Entidad*>* > *posEntidades){
	std::map<std::pair<int,int>, std::vector<Entidad*>* >::iterator p = posEntidades->begin();
	while (p != posEntidades->end()){
		vector<Entidad*> *vectorEntidades = (*p).second;
		int tile_x = (*p).first.first;
		int tile_y = (*p).first.second;
		std::pair<int,int> coordenada = this->calculador->calcularPosicionRelativa(tile_x,tile_y);//this->posicionRelativa(tile_x,tile_y);

	    /* En una posición puede haber más de una entidad */
	    for (unsigned i = 0; i < vectorEntidades->size(); i++){
	    	Entidad *entidad = (*vectorEntidades)[i];

	    	SDL_Rect posicion;
	    	posicion.x = coordenada.first;
	    	posicion.y = coordenada.second;
	    	posicion.w = 2 * this->escala_pixel_tile_x;
	    	posicion.h = 2 * this->escala_pixel_tile_y;

	    	DataPos data(posicion,entidad->getTipo());

	    	if (entidad->getTipo() == "soldado") {
	    		this->posicionPlayer = data.posicion;
	    	}
	    	else{
	    		this->vectorPosiciones->push_back(data);
	    	}
	    }
	    p++;
	}
}

/********************************************************************************/
void VentanaJuego::close(){
	delete this->imagenRelieve;

	SDL_DestroyRenderer(this->renderer);
	this->renderer = NULL;

	SDL_DestroyWindow(this->window);
	this->window = NULL;

	IMG_Quit();
	SDL_Quit();
}

/********************************************************************************/
void VentanaJuego::render(){
	SDL_RenderClear(this->renderer);
	SDL_Rect posicion;

	int cero_relativo_x = this->cero_x;
	int cero_relativo_y = this->cero_y;

	posicion.w = 2 * this->escala_pixel_tile_x;
	posicion.h = 2 * this->escala_pixel_tile_y;

	/* Dibujamos el relieve por Default */
	for(int j = 0; j < this->TILES_Y; j++){

		posicion.x = cero_relativo_x;
		posicion.y = cero_relativo_y;

		for(int i = 0; i < this->TILES_X; i++){
			SDL_RenderCopy(this->renderer,this->imagenRelieve->getTexture(),NULL,&posicion);

			posicion.x += this->escala_pixel_tile_x;
			posicion.y += this->escala_pixel_tile_y;
		}
		cero_relativo_x -= this->escala_pixel_tile_x;
		cero_relativo_y += this->escala_pixel_tile_y;
	}

	/* Dibujamos las entidades que no se mueven */
	for (unsigned i = 0; i < this->vectorPosiciones->size(); i++){
		DataPos data = (*vectorPosiciones)[i];
		SDL_Rect pos = data.posicion;
		std::map<std::string,Imagen*>::iterator itImg = this->mapImagenes->find(data.tipo);
		Imagen *imagenEntidad = itImg->second;
		SDL_RenderCopy(this->renderer,imagenEntidad->getTexture(),NULL,&pos);
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
    bool Follow = false;
    int Follow_Point_X;
    int Follow_Point_Y;

		while (run && event.type != SDL_QUIT){

			SDL_PollEvent(&event);

			if (event.type == SDL_QUIT) run = false;
			SDL_GetMouseState(&MouseX,&MouseY);

			/* Analisis del evento de movimiento */
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

            /* Actualiza el renderer */
            this->render();

            SDL_RenderPresent(this->renderer);
            SDL_Delay(25);

		}

}

/********************************************************************************/
VentanaJuego::~VentanaJuego() {
	std::map<std::string,Imagen*>::iterator p = this->mapImagenes->begin();
	while (p != this->mapImagenes->end()){
		Imagen *imagen = (*p).second;
		p++;
		delete imagen;

	}
	delete this->spritePlayer;

	delete this->mapImagenes;

	delete this->vectorPosiciones;

	delete this->calculador;

	this->close();
}

