/*
 * Escenario.h
 *
 *  Created on: 2 de sept. de 2015
 *      Author: pgfederi
 */

#ifndef ESCENARIO_H_
#define ESCENARIO_H_
#include "Map.h"
#include "Entidad.h"
#include <vector>
using namespace std;

struct InfoEscenario{
	int size_x;
	int size_y;
	map<pair<int,int>,vector<TipoEntidad> > mapPosTipoEntidades;
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
		map<pair<int,int>, vector<TipoEntidad> >::iterator p = this->mapPosTipoEntidades.find(pos);
		if (p == this->mapPosTipoEntidades.end()){
			vector<TipoEntidad> entidades;
			entidades.push_back(tipo);
			this->mapPosTipoEntidades[pos] = entidades;
		}else{
			(*p).second.push_back(tipo);
		}
	};

	map<pair<int,int>, vector<TipoEntidad> > getPosicionesEntidades(){
		return this->mapPosTipoEntidades;
	}

	bool operator!() {
		return !((size_x > 0) && (size_y > 0) && (posX_protagonista >= 0) && (posY_protagonista >= 0));
	}
};

class Escenario {
private:
	int size_x;
	int size_y;
	/* En una posicion puede haber muchas entidades*/
	//std::map<std::pair<int,int>, std::vector<Entidad*>* > *mapPosicionesEntidades;
	Map<pair<int,int>, vector<Entidad*>* > *mapPosicionesEntidades;

	Entidad* protagonista;

	/* Agrega una entidad en una posición indicada como parámetro */
	void agregarEntidad(pair<int,int> pos, Entidad* entidad);

public:
	//Escenario(int ancho, int largo);
	Escenario(InfoEscenario info);

	/* Devuelve las dimensiones del escenario en un par (x,y) */
	pair<int,int> getDimension();

	/* Devuelve un mapa donde las claves son pares x,y (coordenadas) y los valores
	 * son vectores que contienen Entidades, ya que en una posición puede haber
	 * más de una entidad*/
	//map<pair<int,int>, vector<Entidad*>* >* getPosEntidades();
	Map<pair<int,int>, vector<Entidad*>* >* getPosEntidades();

	Entidad* getProtagonista();

	virtual ~Escenario();
};

#endif /* ESCENARIO_H_ */
