/*
 * Dibujador.h
 *
 *  Created on: 26 de sept. de 2015
 *      Author: emanuel
 */

#ifndef VISTA_DIBUJADOR_H_
#define VISTA_DIBUJADOR_H_
#include "../utils/ContenedorDeRecursos.h"
#include "../modelo/Escenario.h"
#include "../vista/CapaNegra.h"

class Dibujador {
private:
	SDL_Renderer *renderer;
	ContenedorDeRecursos *contenedor;
	map<TipoEntidad,InfoEntidad> mapInfoEntidades;
	int *cero_x;
	int *cero_y;
	SDL_Rect rectRelieve;

public:
	Dibujador(SDL_Renderer *renderer, ContenedorDeRecursos *contenedor, int *cero_x, int *cero_y);

	//void dibujarEscenario(Escenario *escenario);

	void dibujarRelieve(int tiles_x, int tiles_y);
	void dibujarEntidades();
	void dibujarProtagonista(Sprite* sprite);
	void dibujarCapaNegra(CapaNegra* capa);

	virtual ~Dibujador();
};

#endif /* VISTA_DIBUJADOR_H_ */
