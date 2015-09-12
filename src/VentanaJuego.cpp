/*
 * VentanaJuego.cpp
 *
 *  Created on: 2 de sept. de 2015
 *      Author: emanuel
 */

#include "VentanaJuego.h"
#include <SDL2/SDL_image.h>

VentanaJuego::VentanaJuego(Juego *juego){
	std::pair<int,int> dimensionVentana = juego->dimensionVentana();
	this->SCREEN_WIDTH = dimensionVentana.first;
	this->SCREEN_HEIGHT = dimensionVentana.second;

	this->escenario = juego->getEscenario();
	this->TILES_X = this->escenario->getDimension().first;
	this->TILES_Y = this->escenario->getDimension().second;

	if (init()){
		/* Dimensiones de la imagen a cargar */
		this->escala_pixel_tile_x = 20;
		this->escala_pixel_tile_y = 10;

		/* El (0,0) relativo del mapa respecto a la ventana principal */
		/* la función "floor" es para que quede un numero entero */
		this->cero_x = (this->SCREEN_WIDTH / 2) - this->escala_pixel_tile_x;
		this->cero_y = (this->SCREEN_HEIGHT - this->TILES_Y) / (2 * this->escala_pixel_tile_y) - this->escala_pixel_tile_y;

		this->relieveDefault = this->cargarImagen("pasto.png");

		/* map donde se almacenan las imagenes que se van a usar */
		this->mapImagenes = new std::map<TipoEntidad,SDL_Texture*>();

		this->cargarImagenes(juego->getEntidades());

		this->vectorPosiciones = new vector<DataPos>();

		this->cargarPosicionesEntidades(this->escenario->getPosEntidades());

		/* Configurar de manera que se cargue el protagonista
		 * con su respectiva posicion*/
		std::map<TipoEntidad,SDL_Texture*>::iterator p = this->mapImagenes->find(juego->getProtagonista()->getTipo());
		this->imagenPlayer = (*p).second;
		this->posicionPlayer.y -= 5;
		this->posicionPlayer.w = 30;
		this->posicionPlayer.h = 20;

		/* Borramos la referencia de la imagen del protagonistia del map de imagenes
		 * y guardamos una referencia en imagenPlayer (en forma directa) */
		this->mapImagenes->erase(juego->getProtagonista()->getTipo());
	}
}

/********************************************************************************/
bool VentanaJuego::init(){
	bool success = true;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) success = false;
	else{
		 this->window = SDL_CreateWindow("Juego",60,60,this->SCREEN_WIDTH,this->SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
		 if (this->window == NULL) success = false;

		 this->renderer = SDL_CreateRenderer(this->window,-1,SDL_RENDERER_ACCELERATED);
		 if (this->renderer == NULL) success = false;
		 else {
			 SDL_RenderSetLogicalSize(this->renderer,this->SCREEN_WIDTH,this->SCREEN_HEIGHT);
			 SDL_SetRenderDrawColor(this->renderer,0,0,0,0);
		 }
	}
	return success;
}

/********************************************************************************/
SDL_Texture* VentanaJuego::cargarImagen(string path){
	SDL_Texture* texture = NULL;
	SDL_Surface *surface = IMG_Load(path.c_str());
	if (surface != NULL){
		texture = SDL_CreateTextureFromSurface(this->renderer, surface);
		SDL_FreeSurface(surface);
	}
	return texture;
}

/********************************************************************************/
void VentanaJuego::cargarImagenes(std::map<TipoEntidad,Entidad*> *entidades){
	std::map<TipoEntidad,Entidad*>::iterator p = entidades->begin();
	while (p != entidades->end()){
		TipoEntidad tipo = (*p).first;
		Entidad *ente = (*p).second;
		p++;
		SDL_Texture *imagen = this->cargarImagen(ente->getPath());
		this->mapImagenes->insert(std::make_pair(tipo,imagen));
	}
}

/********************************************************************************/
void VentanaJuego::cargarPosicionesEntidades(std::map<std::pair<int,int>, std::vector<Entidad*>* > *posEntidades){
	std::map<std::pair<int,int>, std::vector<Entidad*>* >::iterator p = posEntidades->begin();
	while (p != posEntidades->end()){

		vector<Entidad*> *vectorEntidades = (*p).second;
		int tile_x = (*p).first.first;
		int tile_y = (*p).first.second;

	    /* En una posición puede haber más de una entidad */
	    for (unsigned i = 0; i < vectorEntidades->size(); i++){
	    	Entidad *entidad = (*vectorEntidades)[i];

	    	std::pair<int,int> coordenada = this->posicionRelativa(tile_x,tile_y);
	    	SDL_Rect posicion;
	    	posicion.x = coordenada.first;
	    	posicion.y = coordenada.second;
	    	posicion.w = 2 * this->escala_pixel_tile_x;
	    	posicion.h = 2 * this->escala_pixel_tile_y;

	    	DataPos data(posicion,entidad->getTipo());

	    	if (entidad->getTipo() == SOLDADO) {
	    		this->posicionPlayer = data.posicion;
	    	}
	    	else{
	    		this->vectorPosiciones->push_back(data);
	    	}
	    }
	    p++;
	}
}

/********************************************************************************/
void VentanaJuego::close(){
	SDL_DestroyTexture(this->relieveDefault);
	this->relieveDefault = NULL;

	SDL_DestroyTexture(this->imagenPlayer);
	this->imagenPlayer = NULL;

	SDL_DestroyRenderer(this->renderer);
	this->renderer = NULL;

	SDL_DestroyWindow(this->window);
	this->window = NULL;

	IMG_Quit();
	SDL_Quit();
}

/********************************************************************************/
void VentanaJuego::dibujar(){
	SDL_RenderClear(this->renderer);
	SDL_Rect posicion;

	int cero_relativo_x = this->cero_x;
	int cero_relativo_y = this->cero_y;
	posicion.w = 2 * this->escala_pixel_tile_x;
	posicion.h = 2 * this->escala_pixel_tile_y;

	// Se dibuja la superficie
	for(int j = 0; j < this->TILES_Y; j++){

		posicion.x = cero_relativo_x;
		posicion.y = cero_relativo_y;

		for(int i = 0; i < this->TILES_X; i++){
			SDL_RenderCopy(this->renderer,this->relieveDefault,NULL,&posicion);

			posicion.x = posicion.x + this->escala_pixel_tile_x;
			posicion.y = posicion.y + this->escala_pixel_tile_y;
		}
		cero_relativo_x = cero_relativo_x - this->escala_pixel_tile_x;
		cero_relativo_y = cero_relativo_y + this->escala_pixel_tile_y;
	}

	/* Dibujamos las entidades que no se mueven */
	for (unsigned i = 0; i < this->vectorPosiciones->size(); i++){
		DataPos data = (*vectorPosiciones)[i];
		SDL_Rect pos = data.posicion;
		std::map<TipoEntidad,SDL_Texture*>::iterator itImg = this->mapImagenes->find(data.tipo);
		SDL_Texture *imagenEntidad = itImg->second;
		SDL_RenderCopy(this->renderer,imagenEntidad,NULL,&pos);
	}

	SDL_RenderCopy(this->renderer,this->imagenPlayer,NULL,&posicionPlayer);


	// Esto debería dar el efecto de scrolling /
	SDL_Rect clip_rec;
	clip_rec.x = this->posicionPlayer.x - 40;
	clip_rec.y = this->posicionPlayer.y - 40;
	clip_rec.w = 100;
	clip_rec.h = 100;

	//SDL_RenderSetClipRect(this->renderer,&clip_rec);

	SDL_Texture *texture = this->cargarImagen("e.png");
	//SDL_RenderCopy(this->renderer,texture,NULL,&clip_rec);
	SDL_Rect po;
	po.x = 100;
	po.y = 100;
	po.w = 200;
	po.h = 200;
	SDL_RenderCopyEx(this->renderer,texture,&po,&clip_rec,0.0,NULL,SDL_FLIP_NONE);
	//void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

	SDL_RenderPresent(this->renderer);


	SDL_Event event;
	bool run = true;
	while (run){
		while (SDL_PollEvent(&event)){
			if (event.type == SDL_QUIT) run = false;
		}
	}
}

/********************************************************************************/
std::pair<int,int> VentanaJuego::posicionRelativa(int x, int y){
	int x_nuevo, y_nuevo;

	//Posicionamiento en el eje y relativo
	x_nuevo = this->cero_x - y * this->escala_pixel_tile_x;
	y_nuevo = this->cero_y + y * this->escala_pixel_tile_y;

	//posicionamiento en el eje x relativo
	x_nuevo = x_nuevo + x * this->escala_pixel_tile_x;
	y_nuevo = y_nuevo + x * this->escala_pixel_tile_y;

	return std::make_pair(x_nuevo,y_nuevo);
}

/********************************************************************************/
void VentanaJuego::mostrar(){
	this->dibujar();
}

/********************************************************************************/
VentanaJuego::~VentanaJuego() {
	std::map<TipoEntidad,SDL_Texture*>::iterator p = this->mapImagenes->begin();
	while (p != this->mapImagenes->end()){
		SDL_Texture *imagen = (*p).second;
		p++;
		SDL_DestroyTexture(imagen);
		imagen = NULL;

	}
	delete this->mapImagenes;

	delete this->vectorPosiciones;

	this->close();
}

