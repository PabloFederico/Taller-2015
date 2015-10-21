/*
 * Dibujador.cpp
 *
 *  Created on: 26 de sept. de 2015
 *      Author: emanuel
 */

#include "../vista/Dibujador.h"
#include "../utils/Constantes.h"

/********************************************************************************/
Dibujador::Dibujador(SDL_Renderer *renderer) {
	this->renderer = renderer;
	this->contenedor = contenedor;
	this->cero_x = cero_x;
	this->cero_y = cero_y;
}

void Dibujador::setOrigen(int *cero_x, int *cero_y){
	this->cero_x = cero_x;
	this->cero_y = cero_y;
}

void Dibujador::setContenedorDeRecursos(ContenedorDeRecursos* container){
	this->contenedor = container;
	this->mapInfoEntidades = container->getInfoEntidades();
}

/********************************************************************************/
void Dibujador::dibujarRelieve(int tiles_x, int tiles_y){
	Imagen *imagenRelieve = this->contenedor->getImagenTipo(PASTO);

	int cero_relativo_x = *this->cero_x;
	int cero_relativo_y = *this->cero_y;

	rectRelieve.w = ANCHO_PIXEL_PASTO;
	rectRelieve.h = ALTO_PIXEL_PASTO;

	/* Dibujamos el relieve por Default */
	for(int j = 0; j < tiles_y; j++){

		rectRelieve.x = cero_relativo_x;
		rectRelieve.y = cero_relativo_y;

		for(int i = 0; i < tiles_x; i++){
			SDL_RenderCopy(this->renderer,imagenRelieve->getTexture(),NULL,&rectRelieve);

			rectRelieve.x += DISTANCIA_ENTRE_X;
			rectRelieve.y += DISTANCIA_ENTRE_Y;
		}
		cero_relativo_x -= DISTANCIA_ENTRE_X;
		cero_relativo_y += DISTANCIA_ENTRE_Y;
	}
}

/********************************************************************************/
void Dibujador::dibujarEntidades(){
	map<Entidad*,Sprite* >::iterator it = this->contenedor->getMapaSpritesEntidades()->begin();
	while (it != contenedor->getMapaSpritesEntidades()->end()){
		Entidad *entidad = it->first;
		//TipoEntidad tipo = entidad->getTipo();
		Sprite *sprite = it->second;
		SDL_Rect pos = sprite->getPosicion();

		Imagen *imagenEntidad = sprite->getImagen();
		//int tiles_ocupados = this->mapInfoEntidades[tipo].ancho;

		//int x = pos.x;

		// Estos solo se van a ejecutar una vez, salvo el caso de dibujar un castillo,
		//  eso dependera de cuantos tiles ocupe /
		//for (int j = 0; j < tiles_ocupados; j++){
			//for (int k = 0; k < tiles_ocupados; k++){

				//Entidades con movimiento:
				//TODO: Esto hay que hacerlo mas generico.
				if (entidad->esMovible() && sprite->estaEnMovimiento()){
					if (sprite->currentTime() > (1000/sprite->getFps())){
						sprite->efectuarMovimiento();
					}
					SDL_Rect frame = sprite->getFrameActual();
					SDL_RenderCopy(this->renderer,imagenEntidad->getTexture(),&frame,&pos);
				}
				//Entidades sin movimiento:
				else{
					SDL_Rect rect = sprite->getFrameActual();
					//SDL_Rect rect = sprite->getSDLRect(j,k);
					SDL_RenderCopy(this->renderer,imagenEntidad->getTexture(),&rect,&pos);
					//pos.x += pos.w;
				}
			//}
			//pos.x = x;
			//pos.y += pos.h;
		//}

		it++;
	} /* Fin While */
}

/********************************************************************************/
void Dibujador::dibujarProtagonista(Sprite* sprite){
	/* Dibujamos al player */
	SDL_Rect rect = sprite->getPosicion();

	SDL_Texture* texturePlayer = sprite->getImagen()->getTexture();
	SDL_Rect frame = sprite->getFrameActual();
	SDL_RenderCopy(this->renderer,texturePlayer,&frame,&rect);
}

/********************************************************************************/
void Dibujador::dibujarCapaNegra(CapaNegra* capa){
	if (!capa->totalmenteDescubierta()){
		int tiles_x = capa->getDimension().first;
		int tiles_y = capa->getDimension().second;
		Imagen* imagen = this->contenedor->getImagenTipo(DEFAULT);

		int cero_relativo_x = *this->cero_x;
		int cero_relativo_y = *this->cero_y;

		rectRelieve.w = ANCHO_PIXEL_PASTO;
		rectRelieve.h = ALTO_PIXEL_PASTO;

		/* Dibujamos la capa negra */
		for(int j = 0; j < tiles_y; j++){

			rectRelieve.x = cero_relativo_x;
			rectRelieve.y = cero_relativo_y;

			for(int i = 0; i < tiles_x; i++){

				if (capa->tileOculto(i,j)){
					SDL_RenderCopy(this->renderer,imagen->getTexture(),NULL,&rectRelieve);
				}

				rectRelieve.x += DISTANCIA_ENTRE_X;
				rectRelieve.y += DISTANCIA_ENTRE_Y;
			}
			cero_relativo_x -= DISTANCIA_ENTRE_X;
			cero_relativo_y += DISTANCIA_ENTRE_Y;
		}
	}
}

/********************************************************************************/
/*
void Dibujador::dibujarEscenario(Escenario *escenario){
	SDL_RenderClear(this->renderer);

	pair<int,int> dimension = escenario->getDimension();
	dibujarRelieve(dimension.first, dimension.second);
}
*/

/********************************************************************************/
void Dibujador::dibujarBarraEstado(BarraEstado* barraEstado){
	int width_window;
	int height_window;
	SDL_GetRendererOutputSize(renderer,&width_window,&height_window);

	std::pair<int,int> dim = barraEstado->getDimension();

	Imagen *imagen;
	SDL_Rect rect_barra;
	/* Dibujamos el fondo */
	rect_barra.x = 0;
	rect_barra.y = height_window - dim.second;
	rect_barra.w = dim.first * 0.3;
	rect_barra.h = dim.second;
	imagen = this->contenedor->getImagenUtilTipo(BARRA_FONDO);
	SDL_RenderCopy(renderer,imagen->getTexture(),NULL,&rect_barra);

	/* Dibujamos el descriptor */
	rect_barra.x += rect_barra.w;
	rect_barra.w = 0.4 * dim.first;
	imagen = this->contenedor->getImagenUtilTipo(BARRA_DESCRIPCION);
	SDL_RenderCopy(renderer,imagen->getTexture(),NULL,&rect_barra);

	/* Dibujamos el minimapa */
	rect_barra.x += rect_barra.w;
	rect_barra.w = dim.first * 0.3;
	imagen = this->contenedor->getImagenUtilTipo(BARRA_FONDO);
	SDL_RenderCopy(renderer,imagen->getTexture(),NULL,&rect_barra);

}

/********************************************************************************/
Dibujador::~Dibujador() {
	this->mapInfoEntidades.clear();
}

