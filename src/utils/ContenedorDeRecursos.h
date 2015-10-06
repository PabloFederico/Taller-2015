/*
 * ContenedorDeRecursos.h
 *
 *  Created on: 29 de sept. de 2015
 *      Author: emanuel
 */

#ifndef UTILS_CONTENEDORDERECURSOS_H_
#define UTILS_CONTENEDORDERECURSOS_H_
#include <vector>
#include <SDL2/SDL.h>

#include "../utils/Calculador.h"
#include "../utils/Enumerados.h"
#include "../utils/Map.h"
#include "../utils/Structs.h"
using namespace std;

class ContenedorDeRecursos {
private:
	SDL_Renderer *renderer;
	Calculador *calculador;
	Map<TipoEntidad, Imagen*> *mapImagenes;
	Map<Entidad*, Sprite*> *mapSpritesEntidades;
	map<TipoEntidad, InfoEntidad> mapInfoEntidades;

public:
	ContenedorDeRecursos(SDL_Renderer *renderer, Calculador *calculador);

	void cargarImagenesEntidades(vector<InfoEntidad> infoEntidades);

	Imagen* getImagenTipo(TipoEntidad tipo);

	map<TipoEntidad, InfoEntidad> getInfoEntidades();

	Map<Entidad*, Sprite*>* getMapaSpritesEntidades();

	Sprite* getSpriteDeEntidad(Entidad *entidad);

	void generarYGuardarSpritesEntidades(vector<PosEntidad> *posEntidades);

	void actualizarPosicionesEntidades(int corrimiento_x, int corrimiento_y);

	virtual ~ContenedorDeRecursos();
};

#endif /* UTILS_CONTENEDORDERECURSOS_H_ */
