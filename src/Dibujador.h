/*
 * Dibujador.h
 *
 *  Created on: 26 de sept. de 2015
 *      Author: emanuel
 */

#ifndef DIBUJADOR_H_
#define DIBUJADOR_H_
#include "Sprite.h"
#include "Map.h"
#include "Escenario.h"
#include "Structs.h"

class Dibujador {
private:
	SDL_Renderer *renderer;
	Map<TipoEntidad,Sprite*> *mapSprites;
	map<TipoEntidad,InfoEntidad> mapInfoEntidades;
	int *cero_x;
	int *cero_y;

public:
	Dibujador(SDL_Renderer *renderer, Map<TipoEntidad,Sprite*> *mapSprites, int *cero_x, int *cero_y);

	void setMapInfoEntidad(map<TipoEntidad,InfoEntidad> map);

	//void dibujarEscenario(Escenario *escenario);

	void dibujarRelieve(int tiles_x, int tiles_y);
	void dibujarEntidadesNoMovibles(vector<DataPos>* vectorPosiciones);
	void dibujarProtagonista(SDL_Rect rect, Sprite* sprite);

	virtual ~Dibujador();
};

#endif /* DIBUJADOR_H_ */
