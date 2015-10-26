/*
 * Escenario.h
 *
 *  Created on: 2 de sept. de 2015
 *      Author: pgfederi
 */

#ifndef MODELO_ESCENARIO_H_
#define MODELO_ESCENARIO_H_
#include <vector>
#include <map>
#include <algorithm>

#include "../modelo/Exceptions.h"
#include "../utils/Structs.h"
#include "../modelo/EntidadFactory.h"
#include "../modelo/Entidad.h"
#include "../vista/CapaFog.h"
#include "../modelo/Tile.h"
using namespace std;

class Escenario {
private:
	int size_x;
	int size_y;

	Tile*** matriz_tiles;

	CapaFog *capa;

	vector<PosEntidad>* posicionesEntidades;

	EntidadFactory* fabricaDeEntidades;

	Entidad* protagonista;

	Coordenada c_protagonista;

	Tile* tile_clic;

	void quitarEntidad(Coordenada pos, Entidad* entidad);

	/* Agrega una entidad en una posición indicada como parámetro */
	void agregarEntidad(Coordenada pos, Entidad* entidad);

public:
	Escenario(InfoEscenario info, EntidadFactory* fabrica);

	/* Devuelve las dimensiones del escenario en un par (x,y) */
	pair<int,int> getDimension();

	/* Devuelve un vector que contiene structs de PosEntidad, posición en X,
	 * posición en Y y la entidad */
	vector<PosEntidad>* getVectorEntidades();

	Entidad* getProtagonista();

	void setearTileClic(Tile* tile);

	Tile* getTileClic();

	bool tileEsOcupable(Coordenada c);

	void ocuparTile(Coordenada c);

	void desocuparTile(Coordenada c);

	Tile* getTile(int x, int y);

	void actualizarPosicionProtagonista(Coordenada c);

	CapaFog* getCapa();

	virtual ~Escenario();
};

#endif /* MODELO_ESCENARIO_H_ */
