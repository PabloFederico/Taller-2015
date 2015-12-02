/*
 * VentanaEspera.cpp
 *
 *  Created on: 18 de nov. de 2015
 *      Author: emanuel
 */

#include "VentanaEspera.h"
#include "../utils/Loader.h"

VentanaEspera::VentanaEspera(std::pair<int,int> dimensionesVentana):Ventana(NULL) {//Controller* controlador):Ventana(controlador) {
	if (!init()) { std::cout << "error VentanaEspera" << std::endl; return; }
	this->SCREEN_WIDTH = dimensionesVentana.first;
	this->SCREEN_HEIGHT = dimensionesVentana.second;
	imagenFondo = Loader::cargarImagen(renderer,"images/utils/fondo_ventana_espera.png");
	fuenteTexto = TTF_OpenFont("images/utils/censcbk.ttf",TAM_LETRA_CONEXION);
	imagenEsperando = Loader::cargarTexto(renderer,fuenteTexto,"Esperando Conexiones");
}

EstadoFinVentana VentanaEspera::run(){
	EstadoFinVentana estado = OK;

	SDL_Rect rect_wait;
	rect_wait.x = 500;
	rect_wait.y = 550;
	rect_wait.w = imagenEsperando->getPixelsX();
	rect_wait.h = imagenEsperando->getPixelsY();

	int contador = 0;
	std::string texto = "Esperando Conexiones";
	int time = SDL_GetTicks();
	bool esperando = true;
	SDL_Event e;
	while (esperando){
		SDL_RenderClear(renderer);
		SDL_PollEvent(&e);
		if (e.type == SDL_QUIT){
			esperando = false;
			estado = EXIT;
		}
		contador = 5;//
		if (SDL_GetTicks()-time > 1000) {
			texto = texto + ".";
			delete imagenEsperando;
			imagenEsperando = Loader::cargarTexto(renderer,fuenteTexto,texto);
			rect_wait.w = imagenEsperando->getPixelsX();
			rect_wait.h = imagenEsperando->getPixelsY();
			contador++;
			time = SDL_GetTicks();
		}
		if (contador % 4 == 0){
			texto = "Esperando Conexiones";
			delete imagenEsperando;
			imagenEsperando = Loader::cargarTexto(renderer,fuenteTexto,texto);
			rect_wait.w = imagenEsperando->getPixelsX();
			rect_wait.h = imagenEsperando->getPixelsY();
		}
		if (contador > 5){
			esperando = false;
		}

		SDL_RenderCopy(renderer,imagenFondo->getTexture(),NULL,NULL);
		SDL_RenderCopy(renderer,imagenEsperando->getTexture(),NULL,&rect_wait);

		SDL_RenderPresent(renderer);

	}
	std::cout << "fin run ventanaEspera"<<std::endl;//
	return estado;
}

VentanaEspera::~VentanaEspera() {
	delete imagenFondo;
	delete imagenEsperando;
	close();
}

