/*
 * Sprite.h
 *
 *  Created on: 16 de sept. de 2015
 *      Author: emanuel
 */

#ifndef VISTA_SPRITE_H_
#define VISTA_SPRITE_H_
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include "../utils/Calculador.h"
#include "../utils/Structs.h"
#include "Imagen.h"

class Sprite {
protected:
	Entidad* entidad;

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

	Escenario *escenario;
	Coordenada coord_ceros;

	Imagen* imagenPetrificada;

	void cargarFrames(int dir, int dif, Imagen* image, SDL_Rect** frames);

	void resetTime();
public:
	RegistroPosicion regPos;

public:
	Sprite(int cant_Direcciones, Uint32 cant_Img_Distintas, Imagen* imagen, SDL_Rect posicion, Escenario* escen, Coordenada c_ceros, Entidad* ent = NULL);

	void setDireccion(int direccion);

	Direccion getDireccion();

	void efectuarMovimiento();

	void agregarImagenPetrificada(Imagen* image);

	void cambiarTexture();

	bool estaEnMovimiento();

	void activarMovimiento(bool valor);

	Imagen* getImagen();

	SDL_Rect getPosicion();

	Entidad* getEntidad();

	virtual Coordenada getPosPies();

	virtual void setPosX(int x);
	virtual void setPosY(int y);

	virtual void mover(int cant_x, int cant_y);

	SDL_Rect getFrameActual();

	int getFps();

	void setFps(int fps);

	void setDelay(int delay);

	int currentTime();

	bool checkColision(Sprite* otro);

	void agregarRectangulo(Rectangulo rectangulo);

	Coordenada coordPixelSprite();

	void setearNuevoCamino(Camino nuevoCamino, Coordenada coord_ceros);

	vector<Coordenada> getCaminoARecorrer();

	void quitarPrimeraCoordenada();

	bool quedaCaminoPorRecorrer();

	bool revisarCamino(Coordenada c_punto_actual);

	virtual void acomodar();

	virtual void update(int vel_personaje, Mix_Chunk* sonido_caminar);

	virtual ~Sprite();
};

#endif /* VISTA_SPRITE_H_ */
