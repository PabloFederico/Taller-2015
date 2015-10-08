/*
 * Escenario.h
 *
 *  Created on: 2 de sept. de 2015
 *      Author: pgfederi
 */

#ifndef MODELO_ESCENARIO_H_
#define MODELO_ESCENARIO_H_
#include <vector>

#include "../utils/Structs.h"
#include "../modelo/Entidad.h"
#include "../vista/CapaNegra.h"
using namespace std;

class Escenario {
private:
	int size_x;
	int size_y;

	CapaNegra *capa;

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

	bool tileEsOcupable(int x, int y);

	CapaNegra* getCapa();

	virtual ~Escenario();
};

#endif /* MODELO_ESCENARIO_H_ */
