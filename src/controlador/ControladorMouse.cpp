/*
 * ControladorMouse.cpp
 *
 *  Created on: 8 de oct. de 2015
 *      Author: emanuel
 */

#include "ControladorMouse.h"
#include "../utils/Calculador.h"

ControladorMouse::ControladorMouse(Juego *juego) {
	this->juego = juego;
}

void ControladorMouse::procesarEvento(SDL_Event &event, int MouseX, int MouseY){
	Sprite *sprite = juego->getSpritePlayer();

	/* Actualiza la capa negra */
	int x = sprite->getPosicion().x + sprite->getPosicion().w;
	int y = sprite->getPosicion().y + sprite->getPosicion().h;
	pair<int,int> coord = Calculador::calcularPosicionInversa(x,y,juego->getCeros().first,juego->getCeros().second,juego->getEscenario());

    juego->getEscenario()->getCapa()->descubrirDesdePunto(coord.first,coord.second);


	int Follow_Point_X = sprite->regPos.x_anterior;
	int Follow_Point_Y = sprite->regPos.y_anterior;
	//======Analisis del evento de movimiento======//
	if (event.type == SDL_MOUSEBUTTONDOWN){
		if (event.button.button == SDL_BUTTON_LEFT){
			SDL_Rect posicionPlayer = sprite->getPosicion();
            Follow_Point_X = MouseX - posicionPlayer.w / 2;
            Follow_Point_Y = MouseY - posicionPlayer.h ;

            /* Validación de click dentro del escenario */
            pair<int*,int*> ceros = this->juego->getCeros();
            if (Calculador::puntoContenidoEnEscenario(Follow_Point_X+posicionPlayer.w,Follow_Point_Y+posicionPlayer.h, ceros.first, ceros.second, juego->getEscenario())){
                sprite->activarMovimiento(true);
                sprite->regPos.x_anterior = Follow_Point_X;
                sprite->regPos.y_anterior = Follow_Point_Y;
                Direccion direccion = Calculador::calcularDireccion(Follow_Point_X, Follow_Point_Y, posicionPlayer.x, posicionPlayer.y);
                sprite->setDireccion(direccion);
            }else {
            	/* Si el click esta fuera del escenario, su punto destino será
            	 * el anterior al click (si es que se encontraba en movimiento) */
            	Follow_Point_X = sprite->regPos.x_anterior;
            	Follow_Point_Y = sprite->regPos.y_anterior;
            }

		}
	}

    if (sprite->estaEnMovimiento()) {
    	float distance = Calculador::calcularDistanciaEntrePixeles(sprite->regPos.posX_player, sprite->regPos.posY_player, Follow_Point_X, Follow_Point_Y);

		if (distance > 1.0){
            if (sprite->regPos.posX_player != Follow_Point_X) {
            	float x_result = (sprite->regPos.posX_player - ((sprite->regPos.posX_player - Follow_Point_X) / distance) *(this->juego->getVelocidad())  * 0.05);
            	sprite->setPosX(int(x_result));
            	sprite->regPos.posX_player = x_result;
            }

            if (sprite->regPos.posY_player != Follow_Point_Y) {
                float y_result = (sprite->regPos.posY_player - ((sprite->regPos.posY_player - Follow_Point_Y) / distance) * (juego->getVelocidad()) * 0.05);
                sprite->setPosY(int(y_result));
                sprite->regPos.posY_player = y_result;
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

ControladorMouse::~ControladorMouse() {
	// TODO Auto-generated destructor stub
}

