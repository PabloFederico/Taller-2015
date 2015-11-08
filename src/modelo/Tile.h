/*
 * Tile.h
 *
 *  Created on: 25 de oct. de 2015
 *      Author: emanuel
 */

#ifndef MODELO_TILE_H_
#define MODELO_TILE_H_
#include "../modelo/Entidad.h"
#include "../utils/Exceptions.h"
#include <vector>

class Tile {
private:
	std::vector<Entidad*> entidades;
	bool esCentral = false;

public:
	Tile();

	vector<Entidad*> getEntidades();
	bool estaLibre();
	bool tieneRecurso();
	bool esTileCentral();

	void agregarEntidad(Entidad* entidad, bool central = false);
	void eliminarEntidad(Entidad* entidad);

	Entidad* quitarEntidad(int id_jug);
	Entidad* devolverRecurso();

	virtual ~Tile();
};

#endif /* MODELO_TILE_H_ */
