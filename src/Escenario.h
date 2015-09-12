/*
 * Escenario.h
 *
 *  Created on: 2 de sept. de 2015
 *      Author: pgfederi
 */

#ifndef ESCENARIO_H_
#define ESCENARIO_H_
#include <map>
#include "Entidad.h"
#include <vector>

class Escenario {
private:
	int size_x;
	int size_y;
	/* En una posicion puede haber muchas entidades*/
	std::map<std::pair<int,int>, std::vector<Entidad*>* > *mapPosicionesEntidades;

public:
	Escenario(int ancho, int largo);

	/* Devuelve las dimensiones del escenario en un par (x,y) */
	std::pair<int,int> getDimension();

	/* Devuelve un mapa donde las claves son pares x,y (coordenadas) y los valores
	 * son vectores que contienen Entidades, ya que en una posición puede haber
	 * más de una entidad*/
	std::map<std::pair<int,int>, std::vector<Entidad*>* >* getPosEntidades();

	/* Agrega una entidad en una posición indicada como parametro */
	void agregarEntidad(std::pair<int,int> pos, Entidad* entidad);

	virtual ~Escenario();
};

#endif /* ESCENARIO_H_ */
