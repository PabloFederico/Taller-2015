/*
 * Calculador.cpp
 *
 *  Created on: 16 de sept. de 2015
 *      Author: emanuel
 */

#include "Calculador.h"
#include <math.h>
#include <stdlib.h>
#include "Constantes.h"

Calculador::Calculador(int *cero_x, int *cero_y) {
	this->cero_x = cero_x;
	this->cero_y = cero_y;
}

std::pair<int,int> Calculador::calcularPosicionRelativa(int x, int y){
	int x_nuevo, y_nuevo;

	//Posicionamiento en el eje y relativo
	x_nuevo = *this->cero_x - y * DISTANCIA_ENTRE_X;
	y_nuevo = *this->cero_y + y * DISTANCIA_ENTRE_Y;

	//posicionamiento en el eje x relativo
	x_nuevo = x_nuevo + x * DISTANCIA_ENTRE_X;
	y_nuevo = y_nuevo + x * DISTANCIA_ENTRE_Y;

	return std::make_pair(x_nuevo,y_nuevo);
}

double Calculador::calcularDistancia(int X1, int Y1, int X2, int Y2){
	double DifferenceX = X1 - X2;
    double DifferenceY = Y1 - Y2;
    double distance = sqrt((DifferenceX * DifferenceX) + (DifferenceY * DifferenceY));
    return distance;
}

Direccion Calculador::calcularDireccion(int x_dest, int y_dest, int x_orig, int y_orig){
	Direccion direccion;
	int x_result = x_dest - x_orig;
	int y_result = y_dest - y_orig;

	if (abs(x_result) < 30){
		if (y_result < 0) direccion = NORTE;
		else direccion = SUR;
	}
	else if (abs(y_result) < 30){
			if (x_result < 0) direccion = OESTE;
				else direccion = ESTE;
	}
	else if (x_result > 0){
			if (y_result < 0) direccion = NORESTE;
				else direccion = SURESTE;
	}else{
		  if (y_result < 0) direccion = NOROESTE;
		  else direccion = SUROESTE;
	}

	return direccion;
}

bool Calculador::puntoContenidoEnEscenario(int x, int y, int tiles_x, int tiles_y){
	bool contenido = false;
	int x_relativo;
	int y_relativo;
	int cero_relativo_x = *this->cero_x;
	int cero_relativo_y = *this->cero_y;
	for (int i = 0; i < tiles_x; i++){
		x_relativo = cero_relativo_x;
		y_relativo = cero_relativo_y;
		for(int j = 0; j < tiles_y; j++){
			if (((x > x_relativo) && (x < x_relativo + ANCHO_PIXEL_PASTO)) && ((y > y_relativo) && (y < y_relativo + ALTO_PIXEL_PASTO))){
				return true;
			}
			x_relativo += DISTANCIA_ENTRE_X;
			y_relativo += DISTANCIA_ENTRE_Y;
		}
		cero_relativo_x -= DISTANCIA_ENTRE_X;
		cero_relativo_y += DISTANCIA_ENTRE_Y;

	}
	return contenido;
}


Calculador::~Calculador() {
	// TODO Auto-generated destructor stub
}

