/*
 * ContenedorDeRecursos.cpp
 *
 *  Created on: 29 de sept. de 2015
 *      Author: emanuel
 */

#include "../utils/ContenedorDeRecursos.h"
#include "../utils/Constantes.h"
#include "../utils/Calculador.h"
#include "../utils/Loader.h"

/********************************************************************************/
ContenedorDeRecursos::ContenedorDeRecursos(SDL_Renderer *renderer) {
	this->renderer = renderer;
	this->mapSpritesEntidades = new Map<Entidad*, Sprite*>();
	this->mapImagenes = new Map<TipoEntidad, Imagen*>();
	mapImagenesUtil = new Map<TipoImagenUtil, Imagen*>();
	mapImagenesRecursos = new Map<TipoRecurso, Imagen*>();
}

/********************************************************************************/
void ContenedorDeRecursos::cargarImagenesEntidades(vector<InfoEntidad> infoEntidades){
	/* Primero cargamos la imagen del relieve por default */
	Imagen *pasto = Loader::cargarImagen(this->renderer,"images/pasto.png");
	this->mapImagenes->insert(PASTO,pasto);

	Imagen *capaNegra = Loader::cargarImagen(this->renderer,"images/black1.png");
	this->mapImagenes->insert(DEFAULT,capaNegra);

	for (unsigned i = 0; i < infoEntidades.size(); i++){
		TipoEntidad tipo = infoEntidades[i].tipo;
		string path = infoEntidades[i].path;

		/* Nos guardamos la información de las entidades */
		this->mapInfoEntidades[tipo] = infoEntidades[i];

		Imagen *imagen = Loader::cargarImagen(this->renderer,path);

		this->mapImagenes->insert(tipo,imagen);
	}
}

/********************************************************************************/
void ContenedorDeRecursos::generarYGuardarSpritesEntidades(vector<PosEntidad> *posEntidades, Coordenada coord_ceros, Escenario* escenario){

	for (unsigned i = 0; i < posEntidades->size(); i++){
		int tile_x = (*posEntidades)[i].x;
		int tile_y = (*posEntidades)[i].y;
		Coordenada coord_tile(tile_x, tile_y);
		Entidad* entidad = (*posEntidades)[i].entidad;
		Coordenada coordenada = Calculador::calcularPosicionRelativa(coord_tile,coord_ceros,escenario);

		SDL_Rect posicion;
    	/* Cargamos por default los siguientes valores para TIERRA ó AGUA */
		posicion.x = coordenada.x;
		posicion.y = coordenada.y;
		posicion.w = ANCHO_PIXEL_PASTO;
		posicion.h = ALTO_PIXEL_PASTO;

		Sprite *sprite;

		switch (entidad->getTipo()){
			case ARBOL 	  : {posicion.y -= (1.5 * DISTANCIA_ENTRE_Y);
							posicion.w = ANCHO_PIXEL_PASTO;
							posicion.h = 2 * ALTO_PIXEL_PASTO;
							sprite = new Sprite(1,1,this->getImagenTipo(ARBOL),posicion,escenario,coord_ceros);

							int x_ini = posicion.x;
							int y_ini = posicion.y;
							Rectangulo rect1(x_ini + 0.25*ANCHO_PIXEL_PASTO, 0.5* ANCHO_PIXEL_PASTO, y_ini, 0.75*posicion.h);
							sprite->agregarRectangulo(rect1);
							}
							break;
	    		/* Modificamos los tamanios de la imagen castillo para que ocupe
	    		 * los tiles que le corresponden */
			case CASTILLO : {posicion.x -= DISTANCIA_ENTRE_X * (this->mapInfoEntidades[CASTILLO].ancho - 1);
						    posicion.y = posicion.y - ALTO_PIXEL_PASTO +  DISTANCIA_ENTRE_Y / 4;
						    //posicion.w = ANCHO_PIXEL_PASTO;
						    //posicion.h = (ALTO_PIXEL_PASTO * this->mapInfoEntidades[CASTILLO].ancho + ALTO_PIXEL_PASTO) / this->mapInfoEntidades[CASTILLO].ancho;
						    //sprite = new Sprite(mapInfoEntidades[CASTILLO].ancho,mapInfoEntidades[CASTILLO].ancho,this->getImagenTipo(CASTILLO),posicion);
						    posicion.w = ANCHO_PIXEL_PASTO * this->mapInfoEntidades[CASTILLO].ancho;
						    posicion.h = ALTO_PIXEL_PASTO * this->mapInfoEntidades[CASTILLO].ancho + (ALTO_PIXEL_PASTO -  DISTANCIA_ENTRE_Y / 4);
						    sprite = new Sprite(1,1,this->getImagenTipo(CASTILLO),posicion,escenario,coord_ceros);

						    int x_ini = posicion.x;
						    int y_ini = posicion.y;
						    Rectangulo rect1(x_ini, mapInfoEntidades[CASTILLO].ancho*ANCHO_PIXEL_PASTO, y_ini, mapInfoEntidades[CASTILLO].ancho*ALTO_PIXEL_PASTO);
							sprite->agregarRectangulo(rect1);
							}
						    break;
			case SOLDADO  :
			case JUANA_DE_ARCO :{
							posicion.x += ANCHO_PIXEL_PASTO / 4;
							posicion.w = ANCHO_PIXEL_PASTO / 2;
							posicion.h = ALTO_PIXEL_PASTO * 3 / 4;
							sprite = new Sprite(DIRECCIONES,14,this->getImagenTipo(entidad->getTipo()),posicion,escenario,coord_ceros);

							int x_ini = posicion.x;
							int y_ini = posicion.y;
							Rectangulo rect1(x_ini + 0.3*posicion.w, 0.3*posicion.w, y_ini + 0.1 * posicion.h, 0.8*posicion.h);
							sprite->agregarRectangulo(rect1);
							}
							break;
			case ANIMAL :	{
							posicion.x += ANCHO_PIXEL_PASTO / 8;
							posicion.y -= (DISTANCIA_ENTRE_Y);
							posicion.w = ANCHO_PIXEL_PASTO ;
							posicion.h = 2*ALTO_PIXEL_PASTO;
							sprite = new Sprite(DIRECCIONES,IMAGENES_DIFERENTES,this->getImagenTipo(ANIMAL),posicion,escenario,coord_ceros);
							sprite->activarMovimiento(true);

							int x_ini = posicion.x;
							int y_ini = posicion.y;
							Rectangulo rect1(x_ini + 0.3*ANCHO_PIXEL_PASTO, 0.6* ANCHO_PIXEL_PASTO, y_ini + 0.2*posicion.h, 0.6*posicion.h);
							sprite->agregarRectangulo(rect1);
							}
							break;
	    	default       :
	    					/* AGUA ó TIERRA */
	    					{sprite = new Sprite(1,1,this->getImagenTipo(entidad->getTipo()),posicion,escenario,coord_ceros);
	    					int x_ini = posicion.x;
	    					int y_ini = posicion.y;
	    					Rectangulo rect1(x_ini, ANCHO_PIXEL_PASTO, y_ini, ALTO_PIXEL_PASTO);
	    					 sprite->agregarRectangulo(rect1);
	    					}
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
	//this->mapSpritesEntidades->erase(entidad);
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
void ContenedorDeRecursos::cargarImagenesUtil(){
	Imagen *imagen = Loader::cargarImagen(this->renderer,"images/barra_fondo.png");
	this->mapImagenesUtil->insert(BARRA_FONDO,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/barra_descripcion.png");
	this->mapImagenesUtil->insert(BARRA_DESCRIPCION,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/barra_negra.png");
	this->mapImagenesUtil->insert(BARRA_NEGRA,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/selector_tile.png");
	this->mapImagenesUtil->insert(SELECT_TILE,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/icono_rojo_1.png");
	this->mapImagenesUtil->insert(ICONO_ROJO,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/icono_azul_1.png");
	this->mapImagenesUtil->insert(ICONO_AZUL,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/icono_amarillo_1.png");
	this->mapImagenesUtil->insert(ICONO_AMARILLO,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/icono_naranja_1.png");
	this->mapImagenesUtil->insert(ICONO_NARANJA,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/icono_verde_1.png");
	this->mapImagenesUtil->insert(ICONO_VERDE,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/icono_rosa_1.png");
	this->mapImagenesUtil->insert(ICONO_ROSA,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/icono_blanco_1.png");
	this->mapImagenesUtil->insert(ICONO_BLANCO,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/icono_gris_1.png");
	this->mapImagenesUtil->insert(ICONO_GRIS,imagen);
}


/********************************************************************************/
void ContenedorDeRecursos::cargarImagenesRecursos(){
	Imagen *imagen = Loader::cargarImagen(this->renderer,"images/moneda_1.png");
	this->mapImagenesRecursos->insert(ORO,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/madera_1.png");
	this->mapImagenesRecursos->insert(MADERA,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/comida_1.png");
	this->mapImagenesRecursos->insert(COMIDA,imagen);
}

/********************************************************************************/
Imagen* ContenedorDeRecursos::getImagenUtilTipo(TipoImagenUtil tipo){
	map<TipoImagenUtil,Imagen* >::iterator p = this->mapImagenesUtil->find(tipo);
	Imagen* imagen = (*p).second;
	return imagen;
}

/********************************************************************************/
Imagen* ContenedorDeRecursos::getImagenRecursoTipo(TipoRecurso tipo){
	map<TipoRecurso,Imagen* >::iterator p = this->mapImagenesRecursos->find(tipo);
	Imagen* imagen = (*p).second;
	return imagen;
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

	map<TipoImagenUtil, Imagen* >::iterator itt = this->mapImagenesUtil->begin();
	while (itt != this->mapImagenesUtil->end()){
		Imagen *imagen = itt->second;
		itt++;
		delete imagen;
	}
	delete this->mapImagenesUtil;

	map<TipoRecurso, Imagen* >::iterator ittt = this->mapImagenesRecursos->begin();
	while (ittt != this->mapImagenesRecursos->end()){
		Imagen *imagen = ittt->second;
		ittt++;
		delete imagen;
	}
	delete this->mapImagenesRecursos;

	map<Entidad*, Sprite* >::iterator itttt = this->mapSpritesEntidades->begin();
	while (itttt != this->mapSpritesEntidades->end()){
		Sprite *sprite = itttt->second;
		itttt++;
		delete sprite;
	}
	delete this->mapSpritesEntidades;
}

