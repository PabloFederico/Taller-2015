/*
 * Sprite.h
 *
 *  Created on: 16 de sept. de 2015
 *      Author: emanuel
 */

#ifndef VISTA_SPRITE_H_
#define VISTA_SPRITE_H_
#include <SDL2/SDL.h>
#include <vector>
#include "../utils/Calculador.h"
#include "../utils/Structs.h"
#include "Imagen.h"

class Sprite {
private:
	int cant_Direcciones;
	int cant_Img_Distintas;
	Imagen* imagen;
	int fps;
	int delay;

	int miliseg_inicial;

	/* Guarda el tiempo que transcurre al finalizar un ciclo del sprite */
	int tiempoTranscurridoAlFinDeCiclo;

	SDL_Rect** frames;
	SDL_Rect frameActual;

	int direccion;
	int indexSpriteActual;

	bool enMovimiento;

	SDL_Rect posicion;

	vector<Rectangulo> rectangulos;

	vector<Coordenada> caminoARecorrer;

	void cargarFrames();

	void resetTime();
public:
	RegistroPosicion regPos;

public:
	Sprite(int cant_Direcciones, Uint32 cant_Img_Distintas, Imagen* imagen, SDL_Rect posicion);

	void setDireccion(int direccion);

	void efectuarMovimiento();

	bool estaEnMovimiento();

	void activarMovimiento(bool valor);

	Imagen* getImagen();

	SDL_Rect getPosicion();

	void setPosX(int x);
	void setPosY(int y);

	void mover(int cant_x, int cant_y);

	SDL_Rect getFrameActual();

	int getFps();

	void setFps(int fps);

	void setDelay(int delay);

	//SDL_Rect getSDLRect(int i, int j);

	int currentTime();

	bool checkColision(Sprite* otro);

	void agregarRectangulo(Rectangulo rectangulo);

	Coordenada coordPixelSprite();

	void setearNuevoCamino(Camino nuevoCamino, Coordenada coord_ceros);

	vector<Coordenada> getCaminoARecorrer();

	void quitarPrimeraCoordenada();

	bool quedaCaminoPorRecorrer();

	void acomodar();

	void update(int vel_personaje);

	virtual ~Sprite();
};

#endif /* VISTA_SPRITE_H_ */
