/*
 * SpriteUnidad.h
 *
 *  Created on: 25 de nov. de 2015
 *      Author: emanuel
 */

#ifndef VISTA_SPRITEUNIDAD_H_
#define VISTA_SPRITEUNIDAD_H_
#include "Sprite.h"

class SpriteUnidad: public Sprite {
protected:
	EstadoEntidad estadoSprite;
	SDL_Rect **framesCaminata;
	SDL_Rect **framesAtaque;
	SDL_Rect **framesQuieto;
	SDL_Rect **framesMuerte;
	SDL_Rect **framesConstruccion; // Para el ALDEANO (no me gusta)
	Imagen* imagenCaminando;
	Imagen* imagenConstruyendo; // Para el ALDEANO (no me gusta)
	Imagen* imagenQuieto;
	Imagen* imagenAtaque;
	Imagen* imagenMuerte;
	int FRAMES_CAMINATA;
	int FRAMES_QUIETO;
	int FRAMES_ATAQUE;
	int FRAMES_CONSTRUYENDO;
	int FRAMES_MUERTE;
	std::map<EstadoEntidad,SDL_Rect> mapRectEstado;



public:
	SpriteUnidad(int cant_dir, int frames_dif, Imagen* image, SDL_Rect pos, Escenario* esc, Coordenada c_ceros, Entidad* entidad);
	void cargarFramesAtaque(Imagen* imagenAtaque);
	void cargarFramesConstruccion(Imagen* imagenConstruccion);
	void cargarFramesQuieto(Imagen*imagenQuieto);
	void cargarFramesMuerte(Imagen* imagenMuerte);
	void verificarEstadoEntidadConImagen();
	Coordenada getPosPies();
	void setPosX(int x);
	void setPosY(int y);
	void mover(int cant_x, int cant_y);
	void update(int vel, Mix_Chunk* sonido_caminar);
	virtual ~SpriteUnidad();
};

#endif /* VISTA_SPRITEUNIDAD_H_ */
