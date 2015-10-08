/*
 * Calculador.h
 *
 *  Created on: 16 de sept. de 2015
 *      Author: emanuel
 */

#ifndef UTILS_CALCULADOR_H_
#define UTILS_CALCULADOR_H_
#include <map>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>

#include "Direccion.h"
#include "../utils/Constantes.h"
#include "../modelo/Exceptions.h"

class Calculador {
private:
	int *cero_x;
	int *cero_y;
	int tiles_x;
	int tiles_y;

	float seno,coseno,long_diagonal;

public:
	Calculador(int *cero_x, int *cero_y, std::pair<int,int> dim_escenario);

	double calcularDistancia(int X1, int Y1, int X2, int Y2);

	std::pair<int,int> calcularPosicionRelativa(int x, int y);

	std::pair<int,int> calcularPosicionInversa(int x, int y);

	Direccion calcularDireccion(int x_dest, int y_dest, int x_orig, int y_orig);

	bool puntoContenidoEnEscenario(int x, int y);

	std::pair<int,int> tileParaPixel(int pix_x, int pix_y);

	std::pair<int,int> pixelCentralDeTile(int tile_x, int tile_y);

	std::vector< std::pair<int,int> > obtenerCaminoMin(int inic_x, int inic_y, int dest_x, int dest_y);

	virtual ~Calculador();
};

#endif /* UTILS_CALCULADOR_H_ */
