/*
 * Calculador.cpp
 *
 *  Created on: 16 de sept. de 2015
 *      Author: emanuel
 */

#include "Calculador.h"
#include <math.h>

Calculador::Calculador(int cero_x, int cero_y, int escala_x, int escala_y) {
	this->cero_x = cero_x;
	this->cero_y = cero_y;
	this->escala_x = escala_x;
	this->escala_y = escala_y;
}

std::pair<int,int> Calculador::calcularPosicionRelativa(int x, int y){
	int x_nuevo, y_nuevo;

	//Posicionamiento en el eje y relativo
	x_nuevo = this->cero_x - y * this->escala_x;
	y_nuevo = this->cero_y + y * this->escala_y;

	//posicionamiento en el eje x relativo
	x_nuevo = x_nuevo + x * this->escala_x;
	y_nuevo = y_nuevo + x * this->escala_y;

	return std::make_pair(x_nuevo,y_nuevo);
}

double Calculador::calcularDistancia(int X1, int Y1, int X2, int Y2){
	double DifferenceX = X1 - X2;
    double DifferenceY = Y1 - Y2;
    double distance = sqrt((DifferenceX * DifferenceX) + (DifferenceY * DifferenceY));
    return distance;
}

Calculador::~Calculador() {
	// TODO Auto-generated destructor stub
}

