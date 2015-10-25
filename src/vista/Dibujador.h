/*
 * Dibujador.h
 *
 *  Created on: 26 de sept. de 2015
 *      Author: emanuel
 */

#ifndef VISTA_DIBUJADOR_H_
#define VISTA_DIBUJADOR_H_
#include "../modelo/BarraEstado.h"
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
	Dibujador(SDL_Renderer *renderer);
	void setOrigen(int *cero_x, int *cero_y);

	void setContenedorDeRecursos(ContenedorDeRecursos* container);

	void dibujarRelieve(int tiles_x, int tiles_y);
	void dibujarEntidades();
	void dibujarProtagonista(Sprite* sprite);
	void dibujarCapaNegra(CapaNegra* capa);
	void dibujarBarraEstado(BarraEstado* barraEstado);

	///
	void repintarOcupado(Escenario* esc);

	virtual ~Dibujador();
};

#endif /* VISTA_DIBUJADOR_H_ */
