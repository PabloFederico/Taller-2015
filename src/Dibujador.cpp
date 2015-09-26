/*
 * Dibujador.cpp
 *
 *  Created on: 26 de sept. de 2015
 *      Author: emanuel
 */

#include "Dibujador.h"
#include "Constantes.h"

Dibujador::Dibujador(SDL_Renderer *renderer, Map<TipoEntidad,Sprite*> *mapSprites, int *cero_x, int *cero_y) {
	this->renderer = renderer;
	this->mapSprites = mapSprites;
	this->cero_x = cero_x;
	this->cero_y = cero_y;
}

void Dibujador::setMapInfoEntidad(map<TipoEntidad,InfoEntidad> map){
	this->mapInfoEntidades = map;
}

void Dibujador::dibujarRelieve(int tiles_x, int tiles_y){
	Imagen *imagenRelieve = this->mapSprites->find(DEFAULT)->second->getImagen();
	SDL_Rect posicion;

	int cero_relativo_x = *this->cero_x;
	int cero_relativo_y = *this->cero_y;

	posicion.w = ANCHO_PIXEL_PASTO;
	posicion.h = ALTO_PIXEL_PASTO;

	/* Dibujamos el relieve por Default */
	for(int j = 0; j < tiles_y; j++){

		posicion.x = cero_relativo_x;
		posicion.y = cero_relativo_y;

		for(int i = 0; i < tiles_x; i++){
			SDL_RenderCopy(this->renderer,imagenRelieve->getTexture(),NULL,&posicion);

			posicion.x += DISTANCIA_ENTRE_X;
			posicion.y += DISTANCIA_ENTRE_Y;
		}
		cero_relativo_x -= DISTANCIA_ENTRE_X;
		cero_relativo_y += DISTANCIA_ENTRE_Y;
	}
}

void Dibujador::dibujarEntidadesNoMovibles(vector<DataPos>* vectorPosiciones){
	/* Dibujamos las entidades que no se mueven */
	for (unsigned i = 0; i < vectorPosiciones->size(); i++){
		DataPos data = (*vectorPosiciones)[i];
		SDL_Rect pos = data.posicion;

		TipoEntidad tipo = data.tipo;

		/* Buscamos la imagen de la entidad a traves del tipo */
		map<TipoEntidad,Sprite*>::iterator itImg = this->mapSprites->find(tipo);
		Imagen *imagenEntidad = (*itImg).second->getImagen();
		int tiles_ocupados = this->mapInfoEntidades[tipo].ancho;

		int x = pos.x;

		/* Estos solo se van a ejecutar una vez, salvo el caso de dibujar un castillo,
		 * eso dependera de cuantos tiles ocupe */
		for (int j = 0; j < tiles_ocupados; j++){
			for (int k = 0; k < tiles_ocupados; k++){
				SDL_Rect rect = itImg->second->getSDLRect(j,k);
				SDL_RenderCopy(this->renderer,imagenEntidad->getTexture(),&rect,&pos);
				pos.x += pos.w;
			}
			pos.x = x;
			pos.y += pos.h;
		}

	}
}

void Dibujador::dibujarProtagonista(SDL_Rect rect, Sprite* sprite){
	/* Dibujamos al player */
	SDL_Texture* texturePlayer = sprite->getImagen()->getTexture();
	SDL_Rect frame = sprite->getSDLRectActual();
	SDL_RenderCopy(this->renderer,texturePlayer,&frame,&rect);
}

/*
void Dibujador::dibujarEscenario(Escenario *escenario){
	SDL_RenderClear(this->renderer);

	pair<int,int> dimension = escenario->getDimension();
	dibujarRelieve(dimension.first, dimension.second);
}
*/

Dibujador::~Dibujador() {
	this->mapInfoEntidades.clear();
}

