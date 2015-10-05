/*
 * Procesador.h
 *
 *  Created on: 1 de oct. de 2015
 *      Author: emanuel
 */

#ifndef PROCESADOR_H_
#define PROCESADOR_H_
#include <SDL2/SDL.h>
#include "Sprite.h"
#include "Calculador.h"

class Procesador {
private:
	int velocidad_personaje;
	Calculador *calculador;

public:
	Procesador(Calculador *calculado, int vel);

	void procesarClick(SDL_Event event, int MouseX, int MouseY, Sprite *sprite,
			  	  	  float &posX_player, float &posY_player,
					  int &x_anterior, int &y_anterior,float dt);

	virtual ~Procesador();
};

#endif /* PROCESADOR_H_ */
