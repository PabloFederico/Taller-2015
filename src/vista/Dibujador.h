/*
 * Dibujador.h
 *
 *  Created on: 26 de sept. de 2015
 *      Author: emanuel
 */

#ifndef VISTA_DIBUJADOR_H_
#define VISTA_DIBUJADOR_H_
#include "../modelo/Juego.h"
#include "../modelo/BarraEstado.h"
#include "../utils/ContenedorDeRecursos.h"
#include "../modelo/Escenario.h"
#include "../vista/CapaFog.h"
#include "../controlador/Mouse.h"
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
	void dibujarRecuadroSeleccion(Escenario* esc);

public:
	Dibujador(SDL_Renderer *renderer);
	void setOrigen(int *cero_x, int *cero_y);

	void setContenedorDeRecursos(ContenedorDeRecursos* container);

	void dibujarRelieve(Escenario* esc, pair<int,int> tamVentana);

	void dibujarEscenario(Escenario* esc, TTF_Font* fuenteTexto, pair<int,int> tamVentana);
	void dibujarBarraEstado(Escenario* esc, BarraEstado* barraEstado, TTF_Font *fuenteTexto, TTF_Font *fuenteTextoRecursos);

	void dibujarEfectosTraslucidos(Coordenada c, Escenario* escenario);

	void dibujarAtaquesDeLargaDistancia(Juego* juego);

	///
	void repintarOcupado(Escenario* esc);

	virtual ~Dibujador();
};

#endif /* VISTA_DIBUJADOR_H_ */
