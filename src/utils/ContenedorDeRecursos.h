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
#include "../modelo/Escenario.h"
#include "../utils/Enumerados.h"
#include "../utils/Map.h"
#include "../vista/Sprite.h"
#include "../utils/Structs.h"
using namespace std;

class ContenedorDeRecursos {
private:
	SDL_Renderer *renderer;
	Map<TipoEntidad, Imagen*> *mapImagenes;
	Map<Entidad*, Sprite*> *mapSpritesEntidades;
	map<TipoEntidad, InfoEntidad> mapInfoEntidades;
	Map<TipoImagenUtil, Imagen*> *mapImagenesUtil; /* para las imagenes de barra de estado */
	map<TipoImagenUnidadEstado, Imagen*> mapImagenUnidades;

public:
	ContenedorDeRecursos(SDL_Renderer *renderer);

	void cargarImagenesEntidades(vector<InfoEntidad> infoEntidades);

	Imagen* getImagenTipo(TipoEntidad tipo);

	map<TipoEntidad, InfoEntidad> getInfoEntidades();

	InfoEntidad getInfoTipo(TipoEntidad tipo);

	Map<Entidad*, Sprite*>* getMapaSpritesEntidades();

	Sprite* getSpriteDeEntidad(Entidad *entidad);

	void generarYGuardarSpritesEntidades(vector<Entidad*> *posEntidades,Coordenada coord_ceros, Escenario* escenario);
	void generarYGuardarSpriteEntidad(Entidad* entidad, Coordenada coord_ceros, Escenario* escenario);

	void actualizarPosicionesEntidades(int corrimiento_x, int corrimiento_y);

	void cargarImagenesUtil();

	Imagen* getImagenUtilTipo(TipoImagenUtil tipo);

	void cargarImagenesRecursos();

	void cargarFramesFaltantesDeUnidades();

	void borrarSpriteDeEntidad(Entidad* entidad);

	virtual ~ContenedorDeRecursos();
};

#endif /* UTILS_CONTENEDORDERECURSOS_H_ */
