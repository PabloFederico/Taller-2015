/*
 * Dibujador.cpp
 *
 *  Created on: 26 de sept. de 2015
 *      Author: emanuel
 */

#include "../vista/Dibujador.h"
#include "../utils/Constantes.h"
#include "../utils/Loader.h"

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
/*
void Dibujador::dibujarRelieve(int tiles_x, int tiles_y){
	Imagen *imagenRelieve = this->contenedor->getImagenTipo(PASTO);

	int cero_relativo_x = *this->cero_x;
	int cero_relativo_y = *this->cero_y;

	rectRelieve.w = ANCHO_PIXEL_PASTO;
	rectRelieve.h = ALTO_PIXEL_PASTO;

	// Dibujamos el relieve por Default
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
*/
/********************************************************************************/
/*
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
				if (entidad->esMovible() && sprite->estaEnMovimiento()){
					if (sprite->currentTime() > (1000/sprite->getFps())){
						sprite->efectuarMovimiento();
					}
<<<<<<< HEAD
					//Coordenada c = Calculador::tileParaPixel(Coordenada(pos.x+pos.w/2,pos.y+pos.h),Coordenada(*cero_x,*cero_y));
					//if (sprite->estaEnZonaDespejada(c.x, c.y)){
=======
					// NO ATRAPA FUERADEESCENARIO
					Coordenada c = Calculador::tileParaPixel(Coordenada(pos.x+pos.w/2,pos.y+pos.h),Coordenada(*cero_x,*cero_y));
					//
					if (sprite->estaEnZonaDespejada(c.x, c.y)){
>>>>>>> branch 'redes' of https://github.com/PabloFederico/Taller-2015.git
						SDL_Rect frame = sprite->getFrameActual();
						SDL_RenderCopy(this->renderer,imagenEntidad->getTexture(),&frame,&pos);
					//}
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
	} // Fin While
}
*/
/********************************************************************************/
/*
void Dibujador::dibujarProtagonista(Sprite* sprite){
	// Dibujamos al player
	SDL_Rect rect = sprite->getPosicion();

	SDL_Texture* texturePlayer = sprite->getImagen()->getTexture();
	SDL_Rect frame = sprite->getFrameActual();
	SDL_RenderCopy(this->renderer,texturePlayer,&frame,&rect);
}
*/
/********************************************************************************/
/*
void Dibujador::dibujarCapaNegra(CapaFog* capa){
		int tiles_x = capa->getDimension().first;
		int tiles_y = capa->getDimension().second;
		Imagen* imagenNegra = this->contenedor->getImagenUtilTipo(CAPA_NEGRA);
		Imagen* imagenGris = this->contenedor->getImagenUtilTipo(CAPA_GRIS);

		int cero_relativo_x = *this->cero_x;
		int cero_relativo_y = *this->cero_y;

		rectRelieve.w = ANCHO_PIXEL_PASTO;
		rectRelieve.h = ALTO_PIXEL_PASTO;

		// Dibujamos la capa negra
		for(int j = 0; j < tiles_y; j++){

			rectRelieve.x = cero_relativo_x;
			rectRelieve.y = cero_relativo_y;

			for(int i = 0; i < tiles_x; i++){

				switch (capa->getEstadoTile(i,j)){
				case ESTADO_GRIS:
							SDL_RenderCopy(this->renderer,imagenGris->getTexture(),NULL,&rectRelieve);
							break;
					case ESTADO_NEGRO:
							SDL_RenderCopy(this->renderer,imagenNegra->getTexture(),NULL,&rectRelieve);
							break;
					default : break;
				}

				rectRelieve.x += DISTANCIA_ENTRE_X;
				rectRelieve.y += DISTANCIA_ENTRE_Y;
			}
			cero_relativo_x -= DISTANCIA_ENTRE_X;
			cero_relativo_y += DISTANCIA_ENTRE_Y;
		}
}
*/
/********************************************************************************/
void Dibujador::dibujarEscenario(Escenario* esc){
	pair<int,int> dimension = esc->getDimension();
	CapaFog* capaFog = esc->getCapa();
	Imagen *imagenRelieve = this->contenedor->getImagenTipo(PASTO);
	Imagen *imagenGris = this->contenedor->getImagenUtilTipo(CAPA_GRIS);

	int cero_relativo_x = *this->cero_x;
	int cero_relativo_y = *this->cero_y;

	rectRelieve.w = ANCHO_PIXEL_PASTO;
	rectRelieve.h = ALTO_PIXEL_PASTO;

	/* Recorremos tile por tile */
	for (int j = 0; j < dimension.second; j++){
		rectRelieve.x = cero_relativo_x;
		rectRelieve.y = cero_relativo_y;
		for (int i = 0; i < dimension.first; i++){

			/* Solo dibujamos para las zonas visibles (GRISES ó COLOR) */
			if (capaFog->getEstadoTile(i,j) != ESTADO_NEGRO){
				//this->dibujarContorno(esc, fuenteTexto);
				SDL_RenderCopy(renderer,imagenRelieve->getTexture(),NULL,&rectRelieve);
				Tile* tile = esc->getTile(i,j);
				vector<Entidad*> entidades = tile->getEntidades();
				for (unsigned k = 0; k < entidades.size(); k++){
					Entidad* entidad = entidades[k];
					Sprite* sprite = this->contenedor->getSpriteDeEntidad(entidad);
					SDL_Rect pos = sprite->getPosicion();
					//Entidades con movimiento:
					if (entidad->esMovible() && sprite->estaEnMovimiento()){
						if (sprite->currentTime() > (1000/sprite->getFps())){
							sprite->efectuarMovimiento();
						}
						/* Si la entidad esta dentro del campo de visón, lo dibujamos */
						if (capaFog->getEstadoTile(i,j) == ESTADO_COLOR){
							SDL_Rect frame = sprite->getFrameActual();
							SDL_RenderCopy(this->renderer,sprite->getImagen()->getTexture(),&frame,&pos);
						}
					}
					//Entidades sin movimiento:
					else{
						SDL_Rect rect = sprite->getFrameActual();
						SDL_RenderCopy(this->renderer,sprite->getImagen()->getTexture(),&rect,&pos);
					}
				}
			}
			if (capaFog->getEstadoTile(i,j) == ESTADO_GRIS){
				SDL_RenderCopy(renderer,imagenGris->getTexture(),NULL,&rectRelieve);
			}
			rectRelieve.x += DISTANCIA_ENTRE_X;
			rectRelieve.y += DISTANCIA_ENTRE_Y;
		}
		cero_relativo_x -= DISTANCIA_ENTRE_X;
		cero_relativo_y += DISTANCIA_ENTRE_Y;
	}
}


/********************************************************************************/
bool Dibujador::dibujarContorno(Escenario* esc, TTF_Font* fuenteTexto){
	for (unsigned i = 0; i < imagenesBasura.size(); i++){
		delete imagenesBasura[i];
	}
	imagenesBasura.clear();

	Imagen* contorno = contenedor->getImagenTipo(CONTORNO);
	Imagen* contornoxl = contenedor->getImagenTipo(CONTORNOXL);

	Tile* tile = esc->getTileClic();
	if (tile == NULL) return false;

	CapaFog* capaFog = esc->getCapa();
	Coordenada c_tile = esc->getCoordTileClic();
	if (capaFog->getEstadoTile(c_tile.x, c_tile.y) == ESTADO_NEGRO){
		esc->setearTileClic(NULL);
		return false;
	}

	vector<Entidad*> entidades = tile->getEntidades();
	if (entidades.size() == 0) esc->setearTileClic(NULL);

	for (unsigned k = 0; k < entidades.size(); k++){
		Entidad* entidad = entidades[k];
		InfoEntidad info = contenedor->getInfoTipo(entidad->getTipo());
		string descripcion = info.descripcion;
		Sprite* sprite = contenedor->getSpriteDeEntidad(entidad);
		SDL_Rect pos = sprite->getPosicion();
		pos.w = ANCHO_PIXEL_PASTO;
		pos.h = ALTO_PIXEL_PASTO;

		SDL_Rect rect_desc;
		int width_window;
		int height_window;
		SDL_GetRendererOutputSize(renderer,&width_window,&height_window);
		rect_desc.x = 250;
		rect_desc.y = height_window - 110; // Barra arranca en 150
		rect_desc.h = 15;
		rect_desc.w = 10 * descripcion.size();

		Imagen* image_desc = Loader::cargarTextoConFondo(renderer,fuenteTexto,descripcion,SDL_Color{255,255,255});
		imagenesBasura.push_back(image_desc);

		bool realizar;
		switch (entidad->getTipo()){
			case SOLDADO:
				pos.x -= 14;
				pos.y += 10;
				realizar = true;
				break;
			case ARBOL:
				pos.y += 23;
				realizar = true;
				break;
			case ANIMAL:
				pos.x -= 5;
				pos.y += 15;
				realizar = true;
				break;
			case CASTILLO:
				pos.w = entidad->getTam().first + 250;
				pos.h = entidad->getTam().second + 120;
				pos.x -= 5;
				pos.y += 5;
				contorno = contornoxl;
				realizar = true;
				break;
			case TIERRA:
				if (entidades.size() == 1) esc->setearTileClic(NULL);
				realizar = false;
				break;
			default :
				realizar = false;
				break;
		}
		// Dibujo contorno de tile y descripcion en barra:
		if (realizar == true){
			SDL_RenderCopy(this->renderer,contorno->getTexture(),NULL,&pos);
			SDL_RenderCopy(renderer, image_desc->getTexture(), NULL, &rect_desc);
		}

	}
	return true; //agregado por MC
}
/********************************************************************************/
void Dibujador::dibujarBarraEstado(Escenario* esc, BarraEstado* barraEstado, TTF_Font* fuenteTexto){
	for (unsigned i = 0; i < imagenesBasura.size(); i++){
		delete imagenesBasura[i];
	}
	imagenesBasura.clear();

	int width_window;
	int height_window;
	SDL_GetRendererOutputSize(renderer,&width_window,&height_window);

	std::pair<int,int> dim = barraEstado->getDimension();

	Imagen *imagen;
	SDL_Rect rect_barra;

	/* Dibujamos el fondo (Para la info de los recursos económicos) */
	rect_barra.x = 0;
	rect_barra.y = height_window - dim.second;
	rect_barra.w = dim.first * 0.3;
	rect_barra.h = dim.second;
	imagen = this->contenedor->getImagenUtilTipo(BARRA_FONDO);
	SDL_RenderCopy(renderer,imagen->getTexture(),NULL,&rect_barra);

	/* Dibujamos el descriptor (Para la descripción de los elementos seleccionados) */
	rect_barra.x += rect_barra.w;
	rect_barra.w = 0.4 * dim.first;
	imagen = this->contenedor->getImagenUtilTipo(BARRA_DESCRIPCION);
	SDL_RenderCopy(renderer,imagen->getTexture(),NULL,&rect_barra);

	/* Dibujamos el fondo del minimapa */
	rect_barra.x += rect_barra.w;
	rect_barra.w = dim.first * 0.3;
	imagen = this->contenedor->getImagenUtilTipo(BARRA_FONDO);
	SDL_RenderCopy(renderer,imagen->getTexture(),NULL,&rect_barra);

	/********************************************************************************/
	/* Dibujamos los recursos economicos con su respectivos valores */
	Imagen* imagen_barra_negra = contenedor->getImagenUtilTipo(BARRA_NEGRA);
	SDL_Rect rect_barra_negra;
	rect_barra_negra.x = 0;
	rect_barra_negra.y = rect_barra.y + 3;
	rect_barra_negra.w = 60;
	rect_barra_negra.h = 15;

	SDL_Rect rect_recurso;
	rect_recurso.x = 5;
	rect_recurso.y = rect_barra.y + 4;
	rect_recurso.w = 10;
	rect_recurso.h = 10;

	SDL_Rect rect_num;
	rect_num.x = rect_barra_negra.w - 10;
	rect_num.y = rect_barra.y + 6;
	rect_num.w = 10;
	rect_num.h = 10;

	map<TipoEntidad, int> mapRecursosEconomicos = barraEstado->getRecursosEconomicos();

	for (int i = MADERA; i <= ORO; i++){
		int cantidad = mapRecursosEconomicos[(TipoEntidad)i];
		ostringstream convert;
		convert << cantidad;
		std::string cant_string = convert.str();
		Imagen* image_recurso = contenedor->getImagenTipo((TipoEntidad)i);
		Imagen* image_num = Loader::cargarTextoConFondo(renderer,fuenteTexto,cant_string,SDL_Color{255,255,255});
		imagenesBasura.push_back(image_num);
		SDL_RenderCopy(renderer, imagen_barra_negra->getTexture(), NULL, &rect_barra_negra);
		SDL_RenderCopy(renderer, image_recurso->getTexture(), NULL, &rect_recurso);
		SDL_RenderCopy(renderer, image_num->getTexture(), NULL, &rect_num);
		rect_barra_negra.x += rect_barra_negra.w + 15;
		rect_recurso.x = rect_barra_negra.x + 5;
		rect_num.x = rect_barra_negra.x + rect_barra_negra.w - 10;
	}

	/********************************************************************************/
	/* Dibujamos el minimapa */
	rect_barra.x += 20;
	rect_barra.y += 25;
	rect_barra.w = dim.first * 0.25;
	rect_barra.h = dim.second * 0.8;
	this->dibujarMiniMapa(esc,rect_barra);
}

/********************************************************************************/
void Dibujador::dibujarMiniMapa(Escenario* esc, SDL_Rect rect){
	Imagen *imagenRelieve = contenedor->getImagenTipo(PASTO);
	Imagen* imagenNegra = contenedor->getImagenUtilTipo(CAPA_NEGRA);

	SDL_RenderCopy(renderer,imagenNegra->getTexture(),NULL,&rect);

	pair<int,int> dimension = esc->getDimension();
	int ANCHO = rect.w / dimension.first;
	int ALTO = rect.h / dimension.second;

	CapaFog* capaFog = esc->getCapa();


	int cero_relativo_x = rect.x + rect.w / 2 - ANCHO / 2;
	int cero_relativo_y = rect.y;

	rectRelieve.w = ANCHO;
	rectRelieve.h = ALTO;

	/* Recorremos tile por tile */
	for (int j = 0; j < dimension.second; j++){
		rectRelieve.x = cero_relativo_x;
		rectRelieve.y = cero_relativo_y;
		for (int i = 0; i < dimension.first; i++){

			/* Solo dibujamos para las zonas visibles (GRISES ó COLOR) */

			if (capaFog->getEstadoTile(i,j) != ESTADO_NEGRO){
				SDL_RenderCopy(renderer,imagenRelieve->getTexture(),NULL,&rectRelieve);
				Tile* tile = esc->getTile(i,j);
				vector<Entidad*> entidades = tile->getEntidades();
				SDL_Rect rect_aux = rectRelieve;
				rect_aux.w = 3;
				rect_aux.h = 3;
				Imagen* image = NULL;
				for (unsigned k = 0; k < entidades.size(); k++){
					Entidad* entidad = entidades[k];
					switch (entidad->getTipo()){
						case SOLDADO:
						case JUANA_DE_ARCO:
								image = contenedor->getImagenUtilTipo(ICONO_ROJO);
								break;
						case ANIMAL:
								image = contenedor->getImagenUtilTipo(ICONO_BLANCO);
								break;
						case CASTILLO:
								image = contenedor->getImagenUtilTipo(ICONO_VIOLETA);
								rect_aux = rectRelieve;
								break;
						case ARBOL:
								image = contenedor->getImagenUtilTipo(ICONO_VERDE);
								break;
						case AGUA:
								image = contenedor->getImagenTipo(AGUA);
								rect_aux = rectRelieve;
								break;
						case TIERRA:
								image = contenedor->getImagenTipo(TIERRA);
								rect_aux = rectRelieve;
								break;
						case MADERA:
								image = contenedor->getImagenUtilTipo(ICONO_NARANJA);
								break;
						case COMIDA:
								image = contenedor->getImagenUtilTipo(ICONO_ROSA);
								break;
						case ORO:
								image = contenedor->getImagenUtilTipo(ICONO_AMARILLO);
								break;
						default : break;
 					}
					SDL_RenderCopy(this->renderer,image->getTexture(),NULL,&rect_aux);
				}
			}
			rectRelieve.x += 0.5 * ANCHO;
			rectRelieve.y += 0.5 * ALTO;
		}
		cero_relativo_x -= 0.5 * ANCHO;
		cero_relativo_y += 0.5 * ALTO;
	}
}

/********************************************************************************/
Dibujador::~Dibujador() {
	this->mapInfoEntidades.clear();
	for (unsigned i = 0; i < imagenesBasura.size(); i++){
		delete imagenesBasura[i];
	}
	imagenesBasura.clear();
}

