/*
 * Procesador.h
 *
 *  Created on: 1 de oct. de 2015
 *      Author: emanuel
 */

#ifndef CONTROLADOR_PROCESADOR_H_
#define CONTROLADOR_PROCESADOR_H_
#include <SDL2/SDL.h>

#include "../utils/Calculador.h"
#include "../vista/Sprite.h"

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

#endif /* CONTROLADOR_PROCESADOR_H_ */
