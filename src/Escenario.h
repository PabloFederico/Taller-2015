/*
 * Escenario.h
 *
 *  Created on: 2 de sept. de 2015
 *      Author: pgfederi
 */

#ifndef ESCENARIO_H_
#define ESCENARIO_H_
#include "Entidad.h"
#include <vector>
using namespace std;

struct PosTipoEntidad{
	int x;
	int y;
	TipoEntidad tipo;
	PosTipoEntidad(int x, int y, TipoEntidad tipo){
		this->x = x;
		this->y = y;
		this->tipo = tipo;
	};
};

struct InfoEscenario{
	int size_x;
	int size_y;
	vector<PosTipoEntidad> posTipoEntidades;
	TipoEntidad protagonista;
	int posX_protagonista;
	int posY_protagonista;

	InfoEscenario(){
		size_x = 1;
		size_y = 1;
		protagonista = SOLDADO;
		posX_protagonista = 0;
		posY_protagonista = 0;
	};

	void agregarEntidad(pair<int,int> pos, TipoEntidad tipo){
		PosTipoEntidad posTipoEnte(pos.first,pos.second,tipo);
		this->posTipoEntidades.push_back(posTipoEnte);
	};

	vector<PosTipoEntidad> getPosicionesEntidades(){
		return this->posTipoEntidades;
	}

	bool operator!() {
		return !((size_x > 0) && (size_y > 0) && (posX_protagonista >= 0) && (posY_protagonista >= 0));
	}
};

struct PosEntidad{
	int x;
	int y;
	Entidad* entidad;
	PosEntidad(int x, int y, Entidad* ente){
		this->x = x;
		this->y = y;
		this->entidad = ente;
	};
};

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
