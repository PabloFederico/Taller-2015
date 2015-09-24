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
using namespace std;

struct DataPos{
	SDL_Rect posicion;
	TipoEntidad tipo;
	DataPos(SDL_Rect pos, TipoEntidad tipo){
		this->posicion = pos;
		this->tipo = tipo;
	};
};

class VentanaJuego {
private:
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	int MARGEN_SCROLL;
	int TILES_X;
	int TILES_Y;
	float velocidad_personaje;
	SDL_Window *window;
	SDL_Renderer *renderer;

	map<TipoEntidad,InfoEntidad> mapInfoEntidades;

	/* Es muy general*/
	Map<TipoEntidad,Sprite*> * mapSprites;

	Sprite *spritePlayer;
	SDL_Rect posicionPlayer;
	TipoEntidad tipoProtagonista;

	vector<DataPos> *vectorPosiciones;

	Calculador *calculador;

	int *cero_x;
	int *cero_y;

	/* Dimensiones de la imagen a cargar
	const static int escala_pixel_tile_x = 20;
	const static int escala_pixel_tile_y = 10;*/
	int LIMITE_DESPLAZAMIENTO_EN_X;
	int LIMITE_DESPLAZAMIENTO_EN_Y;

	/* Inicializa los recursos SDL */
	bool init();

	/* Carga las imagenes que se van a utilizar en el juego */
	void cargarImagenes(vector<InfoEntidad> infoEntidades);

	/* Crea SDL_Rect para cada entidad que se encuentre en el escenario y las guarda en un
	 * vector asociada a un SDL_Texture (imagen) */
	void cargarPosicionesEntidades(vector<PosEntidad>* posEntidades);

	/* Libera recursos utilizados por SDL*/
	void close();

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


public:
	VentanaJuego(Juego *juego);

	/* Muestra la ventana principal */
	void mostrar();

	virtual ~VentanaJuego();

};

#endif /* VENTANAJUEGO_H_ */
