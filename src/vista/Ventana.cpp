/*
 * Ventana.cpp
 *
 *  Created on: 29 de sept. de 2015
 *      Author: emanuel
 */

#include "../vista/Ventana.h"

#include <stdio.h>
#include "../utils/Constantes.h"

Ventana::Ventana(Controller *controlador) {
	pair<int,int> dimensionVentana = controlador->getJuego()->getDimensionVentana();
	this->SCREEN_WIDTH = dimensionVentana.first;
	this->SCREEN_HEIGHT = dimensionVentana.second;
	this->window = NULL;
	this->renderer = NULL;
	this->fuenteTexto = NULL;
	this->controlador = controlador;
}

/********************************************************************************/
bool Ventana::init(){
	 this->window = SDL_CreateWindow("Taller de Programación I: AoF",100,100,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
	 if (this->window == NULL) return false;

	 this->renderer = SDL_CreateRenderer(this->window,-1,SDL_RENDERER_ACCELERATED);
	 if (this->renderer == NULL) return false;
	 else {
		  SDL_RenderSetLogicalSize(this->renderer,SCREEN_WIDTH,SCREEN_HEIGHT);
		  SDL_SetRenderDrawColor(this->renderer,0,0,0,0);
		 }

	return true;
}

/********************************************************************************/
void Ventana::close(){
	SDL_DestroyRenderer(this->renderer);
	this->renderer = NULL;

	SDL_DestroyWindow(this->window);
	this->window = NULL;

	TTF_CloseFont(fuenteTexto);
	fuenteTexto = NULL;
}

/********************************************************************************/
Ventana::~Ventana() {
	//close();
}

