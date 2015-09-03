/*
 * VentanaJuego.h
 *
 *  Created on: 2 de sept. de 2015
 *      Author: emanuel
 */

#ifndef VENTANAJUEGO_H_
#define VENTANAJUEGO_H_
#include <SDL2/SDL.h>
using namespace std;

class VentanaJuego {
private:
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	int TILES_X;
	int TILES_Y;
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *imagen;
	int cero_x;
	int cero_y;
	int escala_pixel_tile;

	bool init();
	SDL_Texture* loadTexture(const char* path);
	void close();

	// Transforma una posicion (x,y) a una posicion de la ventana
	SDL_Rect posicionRelativa(int x, int y);


public:
	VentanaJuego(int screen_width, int screen_height, int tiles_x, int tiles_y);
	void dibujarSuperficie();
	virtual ~VentanaJuego();

};

#endif /* VENTANAJUEGO_H_ */
