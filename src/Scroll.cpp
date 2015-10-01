/*
 * Scroll.cpp
 *
 *  Created on: 1 de oct. de 2015
 *      Author: emanuel
 */

#include "Scroll.h"
#include "Constantes.h"

Scroll::Scroll(int *cero_x, int *cero_y, int width, int height, int margen){
	this->cero_x = cero_x;
	this->cero_y = cero_y;
	this->width = width;
	this->height = height;
	this->margen_scroll = margen;
	this->LIMITE_DESPLAZAMIENTO_EN_X = 50;
	this->LIMITE_DESPLAZAMIENTO_EN_Y = 50;
}

void Scroll::setearLimites(int limite_X, int limite_Y){
	this->LIMITE_DESPLAZAMIENTO_EN_X = limite_X;
	this->LIMITE_DESPLAZAMIENTO_EN_Y = limite_Y;
}

void Scroll::procesarScroll(int mouseX, int mouseY, Sprite* sprite,
					  float &posPlayerX, float &posPlayerY,
		              int &x_anterior, int &y_anterior,
		              int &followX, int &followY){

	if (mouseX < margen_scroll || mouseX > (width-margen_scroll) || mouseY < margen_scroll || mouseY > height-margen_scroll){
		int cantidad = 0;
		/* La camara se mueve hacia la izquierda */
		if (mouseX < margen_scroll && mouseX > 0){
			if (mouseX < margen_scroll / 2) cantidad = 2 * margen_scroll;
			else cantidad = margen_scroll / 2;

			if ((*this->cero_x) < LIMITE_DESPLAZAMIENTO_EN_X ){
				*cero_x += cantidad;
				//this->posicionPlayer.x += cantidad;
				sprite->mover(cantidad,0);
				posPlayerX += cantidad;
				x_anterior += cantidad;
				followX += cantidad;
			}
		}

		/* La camara se mueve hacia la derecha */
		if (mouseX > width - margen_scroll && mouseX < width){
			if (mouseX > width - margen_scroll / 2)
				cantidad = 2 * margen_scroll;
			else cantidad = margen_scroll / 2;

			if (width - *this->cero_x < LIMITE_DESPLAZAMIENTO_EN_X + ANCHO_PIXEL_PASTO){
				*cero_x -= cantidad;
				//this->posicionPlayer.x -= cantidad;
				sprite->mover(-cantidad,0);
				posPlayerX -= cantidad;
				x_anterior -= cantidad;
				followX -= cantidad;
			}
		}

		/* La camara se mueve hacia arriba */
		if (mouseY < margen_scroll && mouseY > 0){
			if (mouseY < margen_scroll / 2) cantidad = 2 * margen_scroll;
			else cantidad = margen_scroll / 2;

			if (*this->cero_y < 0){
				*cero_y += cantidad;
				//this->posicionPlayer.y += cantidad;
				sprite->mover(0,cantidad);
				posPlayerY += cantidad;
				y_anterior += cantidad;
				followY += cantidad;
			}
		}

		/* La camara se mueve hacia abajo */
		if (mouseY > height - margen_scroll && mouseY < height){
			if (mouseY > height - margen_scroll / 2) cantidad = 2 * margen_scroll;
			else cantidad = margen_scroll / 2;

			if (abs(*this->cero_y) < 2*LIMITE_DESPLAZAMIENTO_EN_Y - height){
				*cero_y -= cantidad;
				//this->posicionPlayer.y -= cantidad;
				sprite->mover(0,-cantidad);
				posPlayerY -= cantidad;
				y_anterior -= cantidad;
				followY -= cantidad;
			}
		}
	}
}

Scroll::~Scroll() {
	// TODO Auto-generated destructor stub
}

