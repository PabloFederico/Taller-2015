/*
 * ControladorCamara.cpp
 *
 *  Created on: 7 de oct. de 2015
 *      Author: emanuel
 */

#include "ControladorCamara.h"
#include "../utils/Constantes.h"
#include <stdio.h>
#include <math.h>

ControladorCamara::ControladorCamara(Juego *juego) {
	this->juego = juego;
	this->camara = NULL;
}

void ControladorCamara::setCamara(Camara *cam){
	this->camara = cam;
}

void ControladorCamara::procesarPosicionMouse(int mouseX, int mouseY){
	int width = camara->getDimension().first;
	int height = camara->getDimension().second;
	int margen_scroll = camara->getMargenScrolling();
	int LIMITE_DESPLAZAMIENTO_EN_X = camara->getLimites().first;
	int LIMITE_DESPLAZAMIENTO_EN_Y = camara->getLimites().second;
	int *cero_x = juego->getCeros().first;
	int *cero_y = juego->getCeros().second;

	if (mouseX < margen_scroll || mouseX > (width-margen_scroll) || mouseY < margen_scroll || mouseY > height-margen_scroll){
		int cantidad = 0;
		/* La camara se mueve hacia la izquierda */
		if (mouseX < margen_scroll && mouseX > 0){
			if (mouseX < margen_scroll / 2) cantidad = margen_scroll;
			else cantidad = margen_scroll / 4;

			if ((*cero_x) < LIMITE_DESPLAZAMIENTO_EN_X ){
				camara->mover(cantidad,0);
				juego->actualizarPosicionesEntidades(cantidad,0);
			}
		}

		/* La camara se mueve hacia la derecha */
		if (mouseX > width - margen_scroll && mouseX < width){
			if (mouseX > width - margen_scroll / 2)
				cantidad = margen_scroll;
			else cantidad = margen_scroll / 4;

			if (width - *cero_x < LIMITE_DESPLAZAMIENTO_EN_X + ANCHO_PIXEL_PASTO){
				camara->mover(-cantidad,0);
				juego->actualizarPosicionesEntidades(-cantidad,0);
			}
		}

		/* La camara se mueve hacia arriba */
		if (mouseY < margen_scroll && mouseY > 0){
			if (mouseY < margen_scroll / 2) cantidad = margen_scroll;
			else cantidad = margen_scroll / 4;

			if (*cero_y < 0){
				camara->mover(0,cantidad);
				juego->actualizarPosicionesEntidades(0,cantidad);
			}
		}

		/* La camara se mueve hacia abajo */
		if (mouseY > height - margen_scroll && mouseY < height){
			if (mouseY > height - margen_scroll / 2) cantidad = margen_scroll;
			else cantidad = margen_scroll / 4;

			if (abs(*cero_y) < 2*LIMITE_DESPLAZAMIENTO_EN_Y - height){
				camara->mover(0,-cantidad);
				juego->actualizarPosicionesEntidades(0,-cantidad);

			}
		}
	}
}

void ControladorCamara::moverCamara(int cant_x, int cant_y){
	camara->mover(cant_x,cant_y);
	juego->actualizarPosicionesEntidades(cant_x,cant_y);
}

ControladorCamara::~ControladorCamara() {
	delete this->camara;
}

