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

	static Direccion calcularDireccionEntrePuntosAdyacentes(Coordenada c_origen, Coordenada c_final);

	//static bool puntoContenidoEnEscenario(Coordenada coord_pixel, Coordenada coord_ceros_pixel, Escenario *escenario);
	static bool puntoContenidoEnEscenario(Coordenada coord_tile, Escenario *escenario);

	static Coordenada tileParaCualquierPixel(Coordenada coord_pixel, Coordenada coord_ceros);

	static Coordenada tileParaPixel(Coordenada coord_pixel, Coordenada coord_ceros);

	static Coordenada pixelCentralDeTile(Coordenada coord_tile, Coordenada coord_ceros);

	static Coordenada pixelCentralDeCualquierTile(Coordenada coord_tile, Coordenada coord_ceros);

	static Coordenada generarCoordenadaRandom(int size_x_final, int size_x_inicial, int size_y_final, int size_y_inicial);
	static Coordenada generarPosRandomDentroDeEscenarioConLimites(int size_x, int x_max, int x_min, int size_y, int y_max, int y_min);


	//static float distEuclidiana(Coordenada a, Coordenada z);
	static Camino obtenerCaminoMinParaAcercarse(Escenario *esc, Coordenada coord_tile_orig, Coordenada coord_tile_dest, Coordenada coord_ceros, int rango_accion);
	static Camino obtenerCaminoMin(Escenario *esc, Coordenada coord_orig, Coordenada coord_dest, Coordenada coord_ceros_pixel, bool coord_en_pixeles = true);

	static Coordenada generarPosRandom(int size_x_final, int size_x_inicial, int size_y_final, int size_y_inicial);

	static Coordenada obtenerCoordenadaLibreCercaDeEdificio(Edificio* edificio,Escenario* escenario);

	static int ChequeoDeBorde(int max, int input);
};

#endif /* UTILS_CALCULADOR_H_ */
