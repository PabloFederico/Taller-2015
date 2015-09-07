/*
 * VentanaJuego.cpp
 *
 *  Created on: 2 de sept. de 2015
 *      Author: emanuel
 */

#include "VentanaJuego.h"
#include <SDL2/SDL_image.h>
#include <math.h>

VentanaJuego::VentanaJuego(Juego *juego){
	std::pair<int,int> dimensionVentana = juego->dimensionVentana();
	this->SCREEN_WIDTH = dimensionVentana.first;
	this->SCREEN_HEIGHT = dimensionVentana.second;

	this->escenario = juego->getEscenario();
	this->TILES_X = this->escenario->getDimension().first;
	this->TILES_Y = this->escenario->getDimension().second;

	if (init()){
		this->relieveDefault = this->cargarImagen("pasto.png");

		/* map donde se almacenan las imagenes que se van a usar */
		this->mapImagenes = new std::map<TipoEntidad,SDL_Texture*>();

		this->cargarImagenes(juego->getEntidades());

		//this->cargarPosicionesEntidades(juego->mapa->getPosEntidades());

		/* Configurar de manera que se cargue el protagonista
		 * con su respectiva posicion*/
		std::map<TipoEntidad,SDL_Texture*>::iterator p = this->mapImagenes->find(juego->getProtagonista()->getTipo());
		this->imagenPlayer = (*p).second;

		/* Borramos la referencia de la imagen del protagonistia del map de imagenes
		 * y guardamos una referencia en imagenPlayer (en forma directa) */
		this->mapImagenes->erase(juego->getProtagonista()->getTipo());
	}
	/* Dimensiones de la imagen a cargar */
	this->escala_pixel_tile = 20;

	/* El (0,0) relativo del mapa respecto a la ventana principal */
	/* la función "floor" es para que quede un numero entero */
	this->cero_x = floor(this->SCREEN_WIDTH / 2) - this->escala_pixel_tile;
	this->cero_y = (this->SCREEN_HEIGHT - this->TILES_Y) / (2 * this->escala_pixel_tile) - this->escala_pixel_tile;
}

/********************************************************************************/
bool VentanaJuego::init(){
	bool success = true;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) success = false;
	else{
		 this->window = SDL_CreateWindow("Juego",60,60,this->SCREEN_WIDTH,this->SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
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
SDL_Texture* VentanaJuego::cargarImagen(string path){
	SDL_Texture* texture = NULL;
	SDL_Surface *surface = IMG_Load(path.c_str());
	if (surface != NULL){
		texture = SDL_CreateTextureFromSurface(this->renderer, surface);
		SDL_FreeSurface(surface);
	}
	return texture;
}

/********************************************************************************/
void VentanaJuego::cargarImagenes(std::map<TipoEntidad,Entidad*>* entidades){
	std::map<TipoEntidad,Entidad*>::iterator p = entidades->begin();
	while (p != entidades->end()){
		TipoEntidad tipo = (*p).first;
		Entidad *ente = (*p).second;
		p++;
		SDL_Texture *imagen = this->cargarImagen(ente->getPath());
		this->mapImagenes->insert(std::make_pair(tipo,imagen));
	}
}

/********************************************************************************/
void VentanaJuego::close(){
	SDL_DestroyTexture(this->relieveDefault);
	this->relieveDefault = NULL;

	SDL_DestroyTexture(this->imagenPlayer);
	this->imagenPlayer = NULL;

	SDL_DestroyRenderer(this->renderer);
	this->renderer = NULL;

	SDL_DestroyWindow(this->window);
	this->window = NULL;

	IMG_Quit();
	SDL_Quit();
}

/********************************************************************************/
void VentanaJuego::dibujar(){
	SDL_RenderClear(this->renderer);
	SDL_Rect posicion;
	int cero_relativo_x = this->cero_x;
	int cero_relativo_y = this->cero_y;
	posicion.w = 40;//this->escala_pixel_tile;
	posicion.h = 40; //this->escala_pixel_tile;

	// Se dibuja la superficie
	for(int j = 0; j < this->TILES_Y; j++){

		posicion.x = cero_relativo_x;
		posicion.y = cero_relativo_y;

		for(int i = 0; i < this->TILES_X; i++){
			SDL_RenderCopy(this->renderer,this->relieveDefault,NULL,&posicion);

			posicion.x = posicion.x + this->escala_pixel_tile;
			posicion.y = posicion.y + this->escala_pixel_tile;
		}
		cero_relativo_x = cero_relativo_x - this->escala_pixel_tile;
		cero_relativo_y = cero_relativo_y + this->escala_pixel_tile;
	}

	// Dibujamos las entidades
	std::map<std::pair<int,int>,std::vector<Entidad*>*>::iterator p = this->escenario->getPosEntidades()->begin();
	SDL_Rect posEntidad;

	/* Recorremos el map de posiciones de entidades del escenario*/
	while (p != this->escenario->getPosEntidades()->end()){
		posEntidad = this->posicionRelativa((*p).first.first, (*p).first.second);
		posEntidad.w = 40;
	    posEntidad.h = 40;

	    vector<Entidad*> *vectorEntidades = (*p).second;

	    /* En una posición puede haber más de una entidad */
	    for (int i = 0; i < vectorEntidades->size(); i++){
	    	Entidad *entidad = (*vectorEntidades)[i];
			std::map<TipoEntidad,SDL_Texture*>::iterator itImg = this->mapImagenes->find(entidad->getTipo());
			SDL_Texture *imagenEntidad = itImg->second;
			SDL_RenderCopy(this->renderer,imagenEntidad,NULL,&posEntidad);
	    }
		p++;
	}

	// Se dibuja al jugador
	this->posicionPlayer = this->posicionRelativa(5,0);
	this->posicionPlayer.w = 30;
	this->posicionPlayer.h = 30;
	SDL_RenderCopy(this->renderer,this->imagenPlayer,NULL,&(this->posicionPlayer));

	SDL_RenderPresent(this->renderer);
	SDL_Delay(5000);
}

/********************************************************************************/
SDL_Rect VentanaJuego::posicionRelativa(int x, int y){
	SDL_Rect posicion;
	//Posicionamiento en el eje y relativo
	posicion.x = this->cero_x - y * this->escala_pixel_tile;
	posicion.y = this->cero_y + y * this->escala_pixel_tile;

	//posicionamiento en el eje x relativo
	posicion.x = posicion.x + x * this->escala_pixel_tile;
	posicion.y = posicion.y + x * this->escala_pixel_tile;

	return posicion;
}

/********************************************************************************/
void VentanaJuego::mostrar(){
	this->dibujar();
}

/********************************************************************************/
VentanaJuego::~VentanaJuego() {
	std::map<TipoEntidad,SDL_Texture*>::iterator p = this->mapImagenes->begin();
	while (p != this->mapImagenes->end()){
		SDL_Texture *imagen = (*p).second;
		p++;
		SDL_DestroyTexture(imagen);
		imagen = NULL;

	}
	delete this->mapImagenes;
	this->close();
}

