/*
 * Procesador.cpp
 *
 *  Created on: 1 de oct. de 2015
 *      Author: emanuel
 */

#include "../controlador/Procesador.h"

Procesador::Procesador(Calculador *calculador, int vel) {
	this->calculador = calculador;
	this->velocidad_personaje = vel;
}

void Procesador::procesarClick(SDL_Event event, int MouseX, int MouseY, Sprite *sprite,
							   float &posX_player, float &posY_player,
							   int &x_anterior, int &y_anterior,float dt){


	int Follow_Point_X = x_anterior;
	int Follow_Point_Y = y_anterior;
	//======Analisis del evento de movimiento======//
	if (event.type == SDL_MOUSEBUTTONDOWN){
		if (event.button.button == SDL_BUTTON_LEFT){
			SDL_Rect posicionPlayer = sprite->getPosicion();
            Follow_Point_X = MouseX - posicionPlayer.w / 2;
            Follow_Point_Y = MouseY - posicionPlayer.h ;

            /* Validación de click dentro del escenario */
            if (this->calculador->puntoContenidoEnEscenario(Follow_Point_X+posicionPlayer.w,Follow_Point_Y+posicionPlayer.h)){
                sprite->activarMovimiento(true);
                x_anterior = Follow_Point_X;
                y_anterior = Follow_Point_Y;
                Direccion direccion = this->calculador->calcularDireccion(Follow_Point_X, Follow_Point_Y, posicionPlayer.x, posicionPlayer.y);
                sprite->setDireccion(direccion);
            }else {
            	/* Si el click esta fuera del escenario, su punto destino será
            	 * el anterior al click (si es que se encontraba en movimiento) */
            	Follow_Point_X = x_anterior;
            	Follow_Point_Y = y_anterior;
            }

		}
	}

    if (sprite->estaEnMovimiento()) {
    	float distance = this->calculador->calcularDistancia(posX_player, posY_player, Follow_Point_X, Follow_Point_Y);

		if (distance > 1){
            if (posX_player != Follow_Point_X) {
            	float x_result = (posX_player - ((posX_player - Follow_Point_X) / distance) *(this->velocidad_personaje)  * dt);
            	sprite->setPosX(int(x_result));
            	posX_player = x_result;
            }

            if (posY_player != Follow_Point_Y) {
                float y_result = (posY_player - ((posY_player - Follow_Point_Y) / distance) * (this->velocidad_personaje) * dt);
                 sprite->setPosY(int(y_result));
                posY_player = y_result;
            }
            if (sprite->currentTime() > (1000/sprite->getFps())){
            	sprite->efectuarMovimiento();
            }

		}else  sprite->activarMovimiento(false);
    }else{
    	/* Cuando se deja de mover, se queda en una posición firme */
    	 sprite->acomodar();
    }
    //=======Fin de analisis de evento de movimiento=========//
}

Procesador::~Procesador() {
	// TODO Auto-generated destructor stub
}

