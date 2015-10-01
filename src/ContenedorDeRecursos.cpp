/*
 * ContenedorDeRecursos.cpp
 *
 *  Created on: 29 de sept. de 2015
 *      Author: emanuel
 */

#include "ContenedorDeRecursos.h"
#include "Loader.h"
#include "Constantes.h"

/********************************************************************************/
ContenedorDeRecursos::ContenedorDeRecursos(SDL_Renderer *renderer, Calculador *calculador) {
	this->renderer = renderer;
	this->calculador = calculador;
	this->mapSpritesEntidades = new Map<Entidad*, Sprite*>();
	this->mapImagenes = new Map<TipoEntidad, Imagen*>();
}

/********************************************************************************/
void ContenedorDeRecursos::cargarImagenesEntidades(vector<InfoEntidad> infoEntidades){
	Loader loader;
	/* Primero cargamos la imagen del relieve por default */
	Imagen *pasto = loader.cargarImagen(this->renderer,"images/pasto.png");
	this->mapImagenes->insert(DEFAULT,pasto);

	for (unsigned i = 0; i < infoEntidades.size(); i++){
		TipoEntidad tipo = infoEntidades[i].tipo;
		string path = infoEntidades[i].path;

		/* Nos guardamos la información de las entidades */
		this->mapInfoEntidades[tipo] = infoEntidades[i];

		Imagen *imagen = loader.cargarImagen(this->renderer,path);

		this->mapImagenes->insert(tipo,imagen);
	}
}

/********************************************************************************/
void ContenedorDeRecursos::generarYGuardarSpritesEntidades(vector<PosEntidad> *posEntidades){
	for (unsigned i = 0; i < posEntidades->size(); i++){
		int tile_x = (*posEntidades)[i].x;
		int tile_y = (*posEntidades)[i].y;
		Entidad* entidad = (*posEntidades)[i].entidad;
		pair<int,int> coordenada = this->calculador->calcularPosicionRelativa(tile_x,tile_y);

		SDL_Rect posicion;
    	/* Cargamos por default los siguientes valores para TIERRA ó AGUA */
		posicion.x = coordenada.first;
		posicion.y = coordenada.second;
		posicion.w = ANCHO_PIXEL_PASTO;
		posicion.h = ALTO_PIXEL_PASTO;

		Sprite *sprite;

		switch (entidad->getTipo()){
			case ARBOL 	  : posicion.y -= (1.5 * DISTANCIA_ENTRE_Y);
							posicion.w = ANCHO_PIXEL_PASTO;
							posicion.h = 2 * ALTO_PIXEL_PASTO;
							sprite = new Sprite(1,1,this->getImagenTipo(ARBOL),posicion);
							break;
	    		/* Modificamos los tamanios de la imagen castillo para que ocupe
	    		 * los tiles que le corresponden */
			case CASTILLO : posicion.x -= DISTANCIA_ENTRE_X * (this->mapInfoEntidades[CASTILLO].ancho - 1);
						    posicion.y = posicion.y - ALTO_PIXEL_PASTO +  DISTANCIA_ENTRE_Y / 4;
						    posicion.w = ANCHO_PIXEL_PASTO;
						    posicion.h = (ALTO_PIXEL_PASTO * this->mapInfoEntidades[CASTILLO].ancho + ALTO_PIXEL_PASTO) / this->mapInfoEntidades[CASTILLO].ancho;
						    sprite = new Sprite(mapInfoEntidades[CASTILLO].ancho,mapInfoEntidades[CASTILLO].ancho,this->getImagenTipo(CASTILLO),posicion);
						    break;
			case SOLDADO  :
			case JUANA_DE_ARCO :
							posicion.x += ANCHO_PIXEL_PASTO / 4;
							posicion.w = ANCHO_PIXEL_PASTO / 3;
							posicion.h = ALTO_PIXEL_PASTO * 3 / 4;
							sprite = new Sprite(DIRECCIONES,IMAGENES_DIFERENTES,this->getImagenTipo(entidad->getTipo()),posicion);
							break;
			case ANIMAL :
							posicion.x += ANCHO_PIXEL_PASTO / 8;
							posicion.y -= (DISTANCIA_ENTRE_Y);
							posicion.w = ANCHO_PIXEL_PASTO;
							posicion.h = 2 * ALTO_PIXEL_PASTO;
							sprite = new Sprite(DIRECCIONES,IMAGENES_DIFERENTES,this->getImagenTipo(ANIMAL),posicion);
							break;
	    	default       :
	    					/* AGUA ó TIERRA */
	    					sprite = new Sprite(1,1,this->getImagenTipo(entidad->getTipo()),posicion);
	    					break;
	    }

		sprite->setDelay(this->mapInfoEntidades[entidad->getTipo()].delay);
		sprite->setFps(this->mapInfoEntidades[entidad->getTipo()].fps);
		this->mapSpritesEntidades->insert(entidad,sprite);
	}
}

/********************************************************************************/
Imagen* ContenedorDeRecursos::getImagenTipo(TipoEntidad tipo){
	map<TipoEntidad,Imagen* >::iterator p = this->mapImagenes->find(tipo);
	Imagen* imagen = (*p).second;
	return imagen;
}

/********************************************************************************/
map<TipoEntidad, InfoEntidad> ContenedorDeRecursos::getInfoEntidades(){
	return this->mapInfoEntidades;
}

/********************************************************************************/
Map<Entidad*, Sprite*>* ContenedorDeRecursos::getMapaSpritesEntidades(){
	return this->mapSpritesEntidades;
}

/********************************************************************************/
Sprite* ContenedorDeRecursos::getSpriteDeEntidad(Entidad *entidad){
	map<Entidad*, Sprite* >::iterator it = this->mapSpritesEntidades->find(entidad);
	Sprite *sprite = it->second;
	this->mapSpritesEntidades->erase(entidad);
	return sprite;
}
/********************************************************************************/
void ContenedorDeRecursos::actualizarPosicionesEntidades(int corrimiento_x, int corrimiento_y){
	map<Entidad*, Sprite* >::iterator itt = this->mapSpritesEntidades->begin();
	while (itt != this->mapSpritesEntidades->end()){
		Sprite *sprite = itt->second;
		sprite->mover(corrimiento_x, corrimiento_y);
		itt++;
	}
}

/********************************************************************************/
ContenedorDeRecursos::~ContenedorDeRecursos() {
	this->mapInfoEntidades.clear();

	map<TipoEntidad, Imagen* >::iterator it = this->mapImagenes->begin();
	while (it != this->mapImagenes->end()){
		Imagen *imagen = it->second;
		it++;
		delete imagen;
	}
	delete this->mapImagenes;

	map<Entidad*, Sprite* >::iterator ittt = this->mapSpritesEntidades->begin();
	while (ittt != this->mapSpritesEntidades->end()){
		Sprite *sprite = ittt->second;
		ittt++;
		delete sprite;
	}
	delete this->mapSpritesEntidades;
}

