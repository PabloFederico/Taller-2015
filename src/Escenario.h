/*
 * Escenario.h
 *
 *  Created on: 2 de sept. de 2015
 *      Author: pgfederi
 */

#ifndef ESCENARIO_H_
#define ESCENARIO_H_
#include "Entidad.h"
#include "Structs.h"
#include <vector>
using namespace std;

class Escenario {
private:
	int size_x;
	int size_y;

	vector<PosEntidad>* posicionesEntidades;

	Entidad* protagonista;

	/* Agrega una entidad en una posición indicada como parámetro */
	void agregarEntidad(pair<int,int> pos, Entidad* entidad);

public:
	//Escenario(int ancho, int largo);
	Escenario(InfoEscenario info);

	/* Devuelve las dimensiones del escenario en un par (x,y) */
	pair<int,int> getDimension();

	/* Devuelve un vector que contiene structs de PosEntidad, posición en X,
	 * posición en Y y la entidad */
	vector<PosEntidad>* getVectorEntidades();

	Entidad* getProtagonista();

	virtual ~Escenario();
};

#endif /* ESCENARIO_H_ */
