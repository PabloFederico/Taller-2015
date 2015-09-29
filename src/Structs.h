/*
 * Structs.h
 *
 *  Created on: 26 de sept. de 2015
 *      Author: emanuel
 */

#ifndef STRUCTS_H_
#define STRUCTS_H_
#include <SDL2/SDL.h>
#include <vector>
#include "Entidad.h"
#include "Sprite.h"

/* Estructura para guardar el Tipo de Entidad que se encuentra en las
 *  coordenadas x,y */
struct PosTipoEntidad{
	int x;
	int y;
	TipoEntidad tipo;
	PosTipoEntidad(int x, int y, TipoEntidad tipo){
		this->x = x;
		this->y = y;
		this->tipo = tipo;
	};
};

/* Estructura para guardar información para Escenario */
struct InfoEscenario{
	int size_x;
	int size_y;
	vector<PosTipoEntidad> posTipoEntidades;
	TipoEntidad protagonista;
	int posX_protagonista;
	int posY_protagonista;

	InfoEscenario(){
		size_x = 1;
		size_y = 1;
		protagonista = SOLDADO;
		posX_protagonista = 0;
		posY_protagonista = 0;
	};

	void agregarEntidad(pair<int,int> pos, TipoEntidad tipo){
		PosTipoEntidad posTipoEnte(pos.first,pos.second,tipo);
		this->posTipoEntidades.push_back(posTipoEnte);
	};

	vector<PosTipoEntidad> getPosicionesEntidades(){
		return this->posTipoEntidades;
	}

	bool operator!() {
		return !((size_x > 0) && (size_y > 0) && (posX_protagonista >= 0) && (posY_protagonista >= 0));
	}
};

/* Estructura para guardar una instancia Entidad que se encuentra en las
 *  coordenadas x,y */
struct PosEntidad{
	int x;
	int y;
	Entidad* entidad;
	PosEntidad(int x, int y, Entidad* ente){
		this->x = x;
		this->y = y;
		this->entidad = ente;
	};
};

/* Estructura para guardar el Tipo de Entidad asociado a un SDL_Rect para
 * poder dibujar sobre el renderer */
struct DataPos{
	SDL_Rect posicion;
	Entidad *entidad;
	DataPos(SDL_Rect pos, Entidad *entidad){
		this->posicion = pos;
		this->entidad = entidad;
	};
};


/* Estructura para guardar información sobre una Entidad*/
struct InfoEntidad{
	TipoEntidad tipo;
	string path;
	int ancho;
	int alto;
	int pixel_ref_x;
	int pixel_ref_y;
	int fps;
	int delay;
	InfoEntidad(){
		tipo = DEFAULT;
		path = "images/mann.png";	// Acá poner la imagen de ImagenNoEncontrada.
		ancho = 1;
		alto = 1;
		pixel_ref_x = 0;
		pixel_ref_y = 0;
		fps = 10;
		delay = 0;
	};
};


#endif /* STRUCTS_H_ */
