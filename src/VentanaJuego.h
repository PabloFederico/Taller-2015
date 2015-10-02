/*
 * VentanaJuego.h
 *
 *  Created on: 2 de sept. de 2015
 *      Author: emanuel
 */

#ifndef VENTANAJUEGO_H_
#define VENTANAJUEGO_H_
#include <SDL2/SDL.h>
#include "Juego.h"
#include "Calculador.h"
#include "Sprite.h"
#include "Map.h"
#include "Dibujador.h"
#include "Structs.h"
#include "Ventana.h"
#include "Scroll.h"
#include "Procesador.h"
#include "CapaNegra.h"
#include "ContenedorDeRecursos.h"
using namespace std;


class VentanaJuego : public Ventana {
private:
	Juego *juego;

	int MARGEN_SCROLL;
	int TILES_X;
	int TILES_Y;

	float velocidad_personaje;

	ContenedorDeRecursos *contenedor;

	Sprite *spritePlayer;
	Entidad *protagonista;

	Scroll *scroll;
	CapaNegra *capa;
	Calculador *calculador;
	Dibujador *dibujador;
	Procesador *procesador;

	int *cero_x;
	int *cero_y;

	/* Carga las imagenes que se van a utilizar en el juego */
	void cargarImagenes(vector<InfoEntidad> infoEntidades);

	/* Crea SDL_Rect para cada entidad que se encuentre en el escenario y las guarda en un
	 * vector asociada a un SDL_Texture (imagen) */
	void cargarPosicionesEntidades(vector<PosEntidad>* posEntidades);

	/* Dibuja el escenario*/
	void dibujar();

	/* Actualiza las posiciones de las entidades según el movimiento de la cámara*/
	void actualizarPosicionesEntidades(int corrimiento_x, int corrimiento_y);

	void reiniciar();

	void cargarJuego(Juego *juego);

	void liberarRecursos();

public:
	VentanaJuego(Juego *juego);

	/* Muestra la ventana principal */
	void mostrar();

	virtual ~VentanaJuego();

};

#endif /* VENTANAJUEGO_H_ */
