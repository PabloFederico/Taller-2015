/*
 * VentanaJuego.cpp
 *
 *  Created on: 2 de sept. de 2015
 *      Author: emanuel
 */

#include "VentanaJuego.h"
#include <SDL2/SDL_image.h>
#include <math.h>


VentanaJuego::VentanaJuego(int screen_width, int screen_height, int tiles_x, int tiles_y){
	this->SCREEN_WIDTH = screen_width;
	this->SCREEN_HEIGHT = screen_height;
	this->TILES_X = tiles_x;
	this->TILES_Y = tiles_y;
	if (init()){
		this->imagen = this->loadTexture("image.png");
	}
	// Dimensiones de la imagen a cargar
	this->escala_pixel_tile = 10;

	// El (0,0) relativo del mapa respecto a la ventana principal
	this->cero_x = floor(this->SCREEN_WIDTH / 2) - this->escala_pixel_tile;
	this->cero_y = (this->SCREEN_HEIGHT - this->TILES_Y) / (2 * this->escala_pixel_tile) - this->escala_pixel_tile;
}

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

// Cargar una imagen
SDL_Texture* VentanaJuego::loadTexture(const char* path){
	SDL_Texture* texture = NULL;
	SDL_Surface *surface = IMG_Load(path);
	if (surface != NULL){
		texture = SDL_CreateTextureFromSurface(this->renderer, surface);
		SDL_FreeSurface(surface);
	}
	return texture;
}

void VentanaJuego::close(){
	SDL_DestroyTexture(this->imagen);
	this->imagen = NULL;

	SDL_DestroyRenderer(this->renderer);
	this->renderer = NULL;

	SDL_DestroyWindow(this->window);
	this->window = NULL;

	IMG_Quit();
	SDL_Quit();
}

void VentanaJuego::dibujarSuperficie(){
	SDL_RenderClear(this->renderer);
	SDL_Rect posicion;
	int cero_relativo_x = this->cero_x;
	int cero_relativo_y = this->cero_y;
	posicion.w = this->escala_pixel_tile;
	posicion.h = this->escala_pixel_tile;

	// Se dibuja la superficie
	for(int j = 0; j < this->TILES_Y; j++){

		posicion.x = cero_relativo_x;
		posicion.y = cero_relativo_y;

		for(int i = 0; i < this->TILES_X; i++){
			SDL_RenderCopy(this->renderer,this->imagen,NULL,&posicion);
			SDL_RenderPresent(this->renderer);

			posicion.x = posicion.x + this->escala_pixel_tile;
			posicion.y = posicion.y + this->escala_pixel_tile;
		}
		cero_relativo_x = cero_relativo_x - this->escala_pixel_tile;
		cero_relativo_y = cero_relativo_y + this->escala_pixel_tile;
	}

	// Se dibuja al jugador
	SDL_Rect pos_player = this->posicionRelativa(5,1);
	SDL_Texture *player = this->loadTexture("player.png");
	SDL_RenderCopy(this->renderer,player,NULL,&pos_player);

	SDL_RenderPresent(this->renderer);
	SDL_Delay(5000);
}

SDL_Rect VentanaJuego::posicionRelativa(int x, int y){
	SDL_Rect posicion;
	posicion.w = this->escala_pixel_tile;
	posicion.h = this->escala_pixel_tile;

	//Posicionamiento en el eje y relativo
	posicion.x = this->cero_x - y * this->escala_pixel_tile;
	posicion.y = this->cero_y + y * this->escala_pixel_tile;

	//posicionamiento en el eje x relativo
	posicion.x = posicion.x + x * this->escala_pixel_tile;
	posicion.y = posicion.y + x * this->escala_pixel_tile;

	return posicion;
}

VentanaJuego::~VentanaJuego() {
	this->close();
}

