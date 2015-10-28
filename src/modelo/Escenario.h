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

#include "../utils/Structs.h"
#include "../modelo/EntidadFactory.h"
#include "../modelo/Entidad.h"
#include "../vista/CapaFog.h"
#include "../modelo/Tile.h"
#include "../utils/Exceptions.h"
#include "../utils/Log.h"
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
	vector<PosEntidad>* enemigos;
	Entidad* entidadSeleccionada;

	Coordenada c_protagonista;

	Tile* tile_clic;
	Coordenada c_tile_clic;

	void inicializarMatrizTiles();

public:
	Escenario(InfoEscenario info, EntidadFactory* fabrica, vector<PosEntidad>* enemigos = NULL);

	/* Devuelve las dimensiones del escenario en un par (x,y) */
	pair<int,int> getDimension();

	/* Devuelve un vector que contiene structs de PosEntidad, posición en X,
	 * posición en Y y la entidad */
	vector<PosEntidad>* getVectorEntidades();

	/* Agrega una entidad en una posición indicada como parámetro */
	bool agregarEntidad(Coordenada pos, Entidad* entidad);
	void quitarEntidad(Coordenada pos, Entidad* entidad);

	Entidad* getProtagonista();
	Coordenada getPosProtagonista();

	Entidad* getEntidadSeleccionada();

	void setearTileClic(Tile* tile, Coordenada c_tile);
	void setearCoordTileClic(Coordenada c);
	bool coordEnEscenario(Coordenada c);

	Tile* getTileClic();
	Coordenada getCoordTileClic();

	bool tileEsOcupable(Coordenada c);

	void ocuparTile(Coordenada c);

	void desocuparTile(Coordenada c);

	Tile* getTile(int x, int y);
	Tile* getTile(Coordenada c);

	static Coordenada generarCoordenadaRandom(int size_x_final, int size_x_inicial, int size_y_final, int size_y_inicial);
	void agregarRecurso();
	void quitarRecurso(Coordenada c,Entidad* entidad);
	void actualizarPosicionProtagonista(Coordenada c);
	void actualizarPosicionEnemigo(Entidad* ent, Coordenada c);

	CapaFog* getCapa();

	virtual ~Escenario();
};

#endif /* MODELO_ESCENARIO_H_ */
