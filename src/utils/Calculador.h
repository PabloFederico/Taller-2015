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

#include "Direccion.h"
#include "../utils/Constantes.h"
#include "../modelo/Escenario.h"
#include "Exceptions.h"


class Calculador {

public:
	static double calcularDistanciaEntrePixeles(Coordenada coord_pixel1, Coordenada coord_pixel2);

	static Coordenada calcularPosicionRelativa(Coordenada coord_tile, Coordenada coord_ceros_pixel);

	static Coordenada calcularPosicionInversa(Coordenada coord_pixel, Coordenada coord_ceros_pixel, Escenario *escenario);

	static Direccion calcularDireccion(Coordenada coord_pixel_dest, Coordenada coord_pixel_orig);

	//static bool puntoContenidoEnEscenario(Coordenada coord_pixel, Coordenada coord_ceros_pixel, Escenario *escenario);
	static bool puntoContenidoEnEscenario(Coordenada coord_tile, Escenario *escenario);

	static Coordenada tileParaPixel(Coordenada coord_pixel, Coordenada coord_ceros);

	static Coordenada pixelCentralDeTile(Coordenada coord_tile, Coordenada coord_ceros);

	static Camino obtenerCaminoMin(Escenario *esc, Coordenada coord_pixel_orig, Coordenada coord_pixel_dest, Coordenada coord_ceros_pixel);

	static Coordenada generarPosRandom(int size_x_final, int size_x_inicial, int size_y_final, int size_y_inicial, int seed);
};

#endif /* UTILS_CALCULADOR_H_ */
