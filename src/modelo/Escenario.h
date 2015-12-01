/*
 * Escenario.h
 *
 *  Created on: 2 de sept. de 2015
 *      Author: guidonegri
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
#include "../modelo/Unidad.h"
#include "../modelo/Edificio.h"
using namespace std;

class Escenario {
private:
	int size_x;
	int size_y;

	Tile*** matriz_tiles;

	CapaFog *capa;

	vector<Entidad*>* posicionesEntidades;

	int idJug;
	//EntidadFactory* fabricaDeEntidades;

	//vector<Unidad*>* unidadesEnemigos;
	//vector<Edificio*>* edificiosEnemigos;

	Entidad* entidadSeleccionada;

	Tile* tile_clic;
	Coordenada c_tile_clic;

	Coordenada c_tile_ini_recuadro;
	Coordenada c_tile_fin_recuadro;

	void inicializarMatrizTiles();

	Entidad* entidadTemporal;

public:
	Escenario(InfoEscenario infoEsc, int id_jug);

	/* Devuelve las dimensiones del escenario en un par (x,y) */
	pair<int,int> getDimension();

	int getIDJug();

	/* Devuelve un vector que contiene structs de PosEntidad, posición en X,
	 * posición en Y y la entidad */
	vector<Entidad*>* getVectorEntidades();

	/* Agrega una entidad en una posición indicada como parámetro */
	bool agregarEntidad(Coordenada pos, Entidad* entidad);
	void quitarEntidad(Entidad* entidad);

	Entidad* getEntidadSeleccionada();

	Entidad* obtenerEntidadOcupadoraEnTile(Tile* tile);
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

	Entidad* getRecurso(/*TipoEntidad tipo, */int identificador);

	static Coordenada generarCoordenadaRandom(int size_x_final, int size_x_inicial, int size_y_final, int size_y_inicial, int seed);
	void agregarRecurso(TipoEntidad recurso,Coordenada coord_random);
	//void quitarRecurso(Coordenada c,Entidad* entidad); Se usa la genérica quitarEntidad();

	void actualizarPosicionEnemigo(Entidad* ent, Coordenada c);

	void actualizarPosicionParaEntidad(Coordenada c, Entidad* entidad);

	CapaFog* getCapa();

	bool lugarHabilitadoParaConstruir(Coordenada c, Entidad* entidad);

	bool tieneRecuadroSeleccion();
	void agregarCoordenadasRecuadroSeleccion(Coordenada c_inicial, Coordenada c_final);
	pair<Coordenada,Coordenada> getCoordenadasRecuadro();
	void quitarRecuadroSeleccion();

	void iniciarEntidadTemporal(Entidad* entidad);
	void resetEntidadTemporal();
	Entidad* getEntidadTemporal();

	vector<Entidad*> revisarMuertosDeNadie();
	virtual ~Escenario();
};

#endif /* MODELO_ESCENARIO_H_ */
