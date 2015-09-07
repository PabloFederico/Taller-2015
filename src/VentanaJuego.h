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
#include <map>
using namespace std;

class VentanaJuego {
private:
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	int TILES_X;
	int TILES_Y;

	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *relieveDefault;
	std::map<TipoEntidad, SDL_Texture*> *mapImagenes;

	SDL_Texture *imagenPlayer;
	SDL_Rect posicionPlayer;

	int cero_x;
	int cero_y;
	int escala_pixel_tile;
	Escenario *escenario;

	/* Inicializa los recursos SDL */
	bool init();

	/* Devuelve una referencia a una variable SDL_Texture que contiene la imagen */
	SDL_Texture* cargarImagen(string path);

	/* Carga las imagenes que se van a utilizar en el juego */
	void cargarImagenes(std::map<TipoEntidad, Entidad*>* entidades);

	/* Libera recursos utilizados por SDL*/
	void close();

	/* Convierte una posición del escenario a una posición de la ventana principal*/
	SDL_Rect posicionRelativa(int x, int y);

	/* Dibuja el escenario*/
	void dibujar();


public:
	VentanaJuego(Juego *juego);

	/* Muestra la ventana principal */
	void mostrar();

	virtual ~VentanaJuego();

};

#endif /* VENTANAJUEGO_H_ */
