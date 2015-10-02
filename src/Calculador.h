/*
 * Calculador.h
 *
 *  Created on: 16 de sept. de 2015
 *      Author: emanuel
 */

#ifndef CALCULADOR_H_
#define CALCULADOR_H_
#include <map>
#include "Direccion.h"

class Calculador {
private:
	int *cero_x;
	int *cero_y;
	int tiles_x;
	int tiles_y;

public:
	Calculador(int *cero_x, int *cero_y, std::pair<int,int> dim_escenario);

	double calcularDistancia(int X1, int Y1, int X2, int Y2);

	std::pair<int,int> calcularPosicionRelativa(int x, int y);

	std::pair<int,int> calcularPosicionInversa(int x, int y);

	Direccion calcularDireccion(int x_dest, int y_dest, int x_orig, int y_orig);

	bool puntoContenidoEnEscenario(int x, int y);

	virtual ~Calculador();
};

#endif /* CALCULADOR_H_ */
