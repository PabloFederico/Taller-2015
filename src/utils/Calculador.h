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
	static double calcularDistanciaEntrePixeles(Coordenada coord_pixel1, Coordenada coord_pixel2);

	static Coordenada calcularPosicionRelativa(Coordenada coord_tile, Coordenada coord_ceros_pixel, Escenario *escenario);

	static Coordenada calcularPosicionInversa(Coordenada coord_pixel, Coordenada coord_ceros_pixel, Escenario *escenario);

	static Direccion calcularDireccion(Coordenada coord_pixel_dest, Coordenada coord_pixel_orig);

	static bool puntoContenidoEnEscenario(Coordenada coord_pixel, Coordenada coord_ceros_pixel, Escenario *escenario);

	static Coordenada tileParaPixel(Coordenada coord_pixel, Coordenada coord_ceros);

	static Coordenada pixelCentralDeTile(Coordenada coord_tile, Coordenada coord_ceros);

	static std::vector<Coordenada> obtenerCaminoMin(Escenario *esc, Coordenada coord_tile_orig, Coordenada coord_tile_dest, Coordenada coord_ceros);
};

#endif /* UTILS_CALCULADOR_H_ */
