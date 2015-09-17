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
#include "Loader.h"
#include "Sprite.h"
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
	int TILES_X;
	int TILES_Y;

	SDL_Window *window;
	SDL_Renderer *renderer;

	Imagen *imagenRelieve;
	std::map<TipoEntidad, Imagen*> *mapImagenes;

	Sprite *spritePlayer;
	SDL_Rect posicionPlayer;

	vector<DataPos> *vectorPosiciones;

	Calculador *calculador;
	Loader loader;

	int cero_x;
	int cero_y;

	/* Dimensiones de la imagen a cargar */
	const static int escala_pixel_tile_x = 20;
	const static int escala_pixel_tile_y = 10;

	Escenario *escenario;

	/* Inicializa los recursos SDL */
	bool init();

	/* Carga las imagenes que se van a utilizar en el juego */
	void cargarImagenes(std::map<TipoEntidad, Entidad*> *entidades);

	/* Crea SDL_Rect para cada entidad que se encuentre en el escenario y las guarda en un
	 * vector asociada a un SDL_Texture (imagen) */
	void cargarPosicionesEntidades(std::map<std::pair<int,int>, std::vector<Entidad*>* > *posEntidades);

	/* Libera recursos utilizados por SDL*/
	void close();

	/* Dibuja el escenario*/
	void render();


public:
	VentanaJuego(Juego *juego);

	/* Muestra la ventana principal */
	void mostrar();

	virtual ~VentanaJuego();

};

#endif /* VENTANAJUEGO_H_ */
