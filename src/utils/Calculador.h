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
#include "../modelo/Escenario.h"

class Calculador {

public:
	static double calcularDistanciaEntrePixeles(int X1, int Y1, int X2, int Y2);

	static std::pair<int,int> calcularPosicionRelativa(int x, int y, int *cero_x, int *cero_y, Escenario *escenario);

	static std::pair<int,int> calcularPosicionInversa(int x, int y, int *cero_x, int *cero_y, Escenario *escenario);

	static Direccion calcularDireccion(int x_dest, int y_dest, int x_orig, int y_orig);

	static bool puntoContenidoEnEscenario(int x, int y, int *cero_x, int *cero_y, Escenario *escenario);

	static std::pair<int,int> tileParaPixel(int pix_x, int pix_y, int cero_x, int cero_y);

	static std::pair<int,int> pixelCentralDeTile(int tile_x, int tile_y, int cero_x, int cero_y);

	static std::vector< std::pair<int,int> > obtenerCaminoMin(Escenario *esc, int inic_x, int inic_y, int dest_x, int dest_y, int cero_x, int cero_y);
};

#endif /* UTILS_CALCULADOR_H_ */
