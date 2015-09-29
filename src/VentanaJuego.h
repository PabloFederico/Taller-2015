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

	Calculador *calculador;
	Dibujador *dibujador;

	int *cero_x;
	int *cero_y;

	int LIMITE_DESPLAZAMIENTO_EN_X;
	int LIMITE_DESPLAZAMIENTO_EN_Y;

	/* Carga las imagenes que se van a utilizar en el juego */
	void cargarImagenes(vector<InfoEntidad> infoEntidades);

	/* Crea SDL_Rect para cada entidad que se encuentre en el escenario y las guarda en un
	 * vector asociada a un SDL_Texture (imagen) */
	void cargarPosicionesEntidades(vector<PosEntidad>* posEntidades);

	/* Dibuja el escenario*/
	void dibujar();

	/* */
	void procesarScroll(int MouseX, int MouseY,
						float &posPlayerX, float &posPlayerY,
						int &x_anterior, int &y_anterior,
						int &followX, int &followY);

	void procesarClick(SDL_Event event, int MouseX, int MouseY,
										float &posPlayerX, float &posPlayerY,
										int &x_anterior, int &y_anterior,
										int &followX, int &followY, bool &Follow, float dt);

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
