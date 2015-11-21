/*
 * ContenedorDeSonidos.h
 *
 *  Created on: 20 de nov. de 2015
 *      Author: guidonegri
 */

#ifndef UTILS_CONTENEDORDESONIDOS_H_
#define UTILS_CONTENEDORDESONIDOS_H_
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "../utils/Enumerados.h"
#include "../utils/Map.h"
using namespace std;

class ContenedorDeSonidos {
private:
	Map<TipoSonido, Mix_Chunk*> *mapSonidos;

public:
	ContenedorDeSonidos();

	Mix_Chunk* getSonidoTipo(TipoSonido tipo);

	void cargarSonidos();

	virtual ~ContenedorDeSonidos();
};

#endif /* UTILS_CONTENEDORDESONIDOS_H_ */
