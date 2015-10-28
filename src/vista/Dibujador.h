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
#include "../vista/CapaFog.h"
#include <SDL2/SDL_ttf.h>

class Dibujador {
private:
	SDL_Renderer *renderer;
	ContenedorDeRecursos *contenedor;
	map<TipoEntidad,InfoEntidad> mapInfoEntidades;
	int *cero_x;
	int *cero_y;
	SDL_Rect rectRelieve;
	vector<Imagen*> imagenesBasura;

	void dibujarMiniMapa(Escenario* esc, SDL_Rect rect);
	void dibujarRecuadroCamara(SDL_Rect rect, int ancho_tile, int alto_tile);

public:
	Dibujador(SDL_Renderer *renderer);
	void setOrigen(int *cero_x, int *cero_y);

	void setContenedorDeRecursos(ContenedorDeRecursos* container);

	void dibujarRelieve(Escenario* esc);
	//void dibujarEntidades();
	//void dibujarProtagonista(Sprite* sprite);
	//void dibujarCapaNegra(CapaFog* capa);
	void dibujarEscenario(Escenario* esc);
	void dibujarBarraEstado(Escenario* esc, BarraEstado* barraEstado, TTF_Font *fuenteTexto);
	bool dibujarContorno(Escenario* esc, TTF_Font* fuenteTexto);

	///
	void repintarOcupado(Escenario* esc);

	virtual ~Dibujador();
};

#endif /* VISTA_DIBUJADOR_H_ */
