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
}

/********************************************************************************/
void ContenedorDeRecursos::cargarImagenesEntidades(vector<InfoEntidad> infoEntidades){
	/* Primero cargamos la imagen del relieve por default */
	Imagen *pasto = Loader::cargarImagen(this->renderer,"images/relieve/pasto.png");
	this->mapImagenes->insert(PASTO,pasto);

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
void ContenedorDeRecursos::generarYGuardarSpritesEntidades(vector<Entidad*> *posEntidades, Coordenada coord_ceros, Escenario* escenario){
	for (unsigned i = 0; i < posEntidades->size(); i++) {
		generarYGuardarSpriteEntidad((*posEntidades)[i], coord_ceros, escenario);
	}
	std::cout<<mapSpritesEntidades->size()<<" sprites creados"<<std::endl;//
}

void ContenedorDeRecursos::generarYGuardarSpriteEntidad(Entidad* entidad, Coordenada coord_ceros, Escenario* escenario) {
	int tile_x = entidad->getPosicion().x;
	int tile_y = entidad->getPosicion().y;
	Coordenada coord_tile(tile_x, tile_y);
	entidad->setTam(mapInfoEntidades[entidad->getTipo()].ancho, mapInfoEntidades[entidad->getTipo()].alto);	// Esto ya lo hace la clase FactoryEntidades
	Coordenada coordenada = Calculador::calcularPosicionRelativa(coord_tile,coord_ceros);

	SDL_Rect posicion;
	/* Cargamos por default los siguientes valores para TIERRA ó AGUA */
	posicion.x = coordenada.x;
	posicion.y = coordenada.y;
	posicion.w = ANCHO_PIXEL_PASTO;
	posicion.h = ALTO_PIXEL_PASTO;

	Sprite *sprite;

	switch (entidad->getTipo()){
		case ARBOL 	  : {
						posicion.y -= (1.5 * DISTANCIA_ENTRE_Y);
						posicion.w = ANCHO_PIXEL_PASTO;
						posicion.h = 2 * ALTO_PIXEL_PASTO;
						sprite = new Sprite(1,1,this->getImagenTipo(ARBOL),posicion,escenario,coord_ceros,entidad);

						int x_ini = posicion.x;
						int y_ini = posicion.y;
						Rectangulo rect1(x_ini + 0.25*ANCHO_PIXEL_PASTO, 0.5* ANCHO_PIXEL_PASTO, y_ini, 0.75*posicion.h);
						sprite->agregarRectangulo(rect1);
						}
						break;
			/* Modificamos los tamanios de la imagen castillo para que ocupe
			 * los tiles que le corresponden */
		case CUARTEL:
		case CENTRO_CIVICO : {
						posicion.x -= DISTANCIA_ENTRE_X * (this->mapInfoEntidades[entidad->getTipo()].ancho - 1);
						posicion.y = posicion.y - ALTO_PIXEL_PASTO +  DISTANCIA_ENTRE_Y / 4;
						//posicion.w = ANCHO_PIXEL_PASTO;
						//posicion.h = (ALTO_PIXEL_PASTO * this->mapInfoEntidades[CASTILLO].ancho + ALTO_PIXEL_PASTO) / this->mapInfoEntidades[CASTILLO].ancho;
						//sprite = new Sprite(mapInfoEntidades[CASTILLO].ancho,mapInfoEntidades[CASTILLO].ancho,this->getImagenTipo(CASTILLO),posicion);
						posicion.w = ANCHO_PIXEL_PASTO * this->mapInfoEntidades[entidad->getTipo()].ancho;
						posicion.h = ALTO_PIXEL_PASTO * this->mapInfoEntidades[entidad->getTipo()].alto + (ALTO_PIXEL_PASTO -  DISTANCIA_ENTRE_Y / 2);
						sprite = new Sprite(1,1,this->getImagenTipo(entidad->getTipo()),posicion,escenario,coord_ceros,entidad);

						int x_ini = posicion.x;
						int y_ini = posicion.y;
						Rectangulo rect1(x_ini, mapInfoEntidades[entidad->getTipo()].ancho*ANCHO_PIXEL_PASTO, y_ini, mapInfoEntidades[entidad->getTipo()].ancho*ALTO_PIXEL_PASTO);
						sprite->agregarRectangulo(rect1);
						}
						break;

		case CASTILLO : {
						posicion.x -= DISTANCIA_ENTRE_X * (this->mapInfoEntidades[entidad->getTipo()].ancho - 1);
						posicion.y = posicion.y - ALTO_PIXEL_PASTO +  DISTANCIA_ENTRE_Y / 4;
						//posicion.w = ANCHO_PIXEL_PASTO;
						//posicion.h = (ALTO_PIXEL_PASTO * this->mapInfoEntidades[CASTILLO].ancho + ALTO_PIXEL_PASTO) / this->mapInfoEntidades[CASTILLO].ancho;
						//sprite = new Sprite(mapInfoEntidades[CASTILLO].ancho,mapInfoEntidades[CASTILLO].ancho,this->getImagenTipo(CASTILLO),posicion);
						posicion.w = ANCHO_PIXEL_PASTO * this->mapInfoEntidades[entidad->getTipo()].ancho;
						posicion.h = ALTO_PIXEL_PASTO * this->mapInfoEntidades[entidad->getTipo()].alto + (ALTO_PIXEL_PASTO -  DISTANCIA_ENTRE_Y / 2);
						sprite = new Sprite(1,1,this->getImagenTipo(entidad->getTipo()),posicion,escenario,coord_ceros,entidad);

						int x_ini = posicion.x;
						int y_ini = posicion.y;
						Rectangulo rect1(x_ini, mapInfoEntidades[entidad->getTipo()].ancho*ANCHO_PIXEL_PASTO, y_ini, mapInfoEntidades[entidad->getTipo()].ancho*ALTO_PIXEL_PASTO);
						sprite->agregarRectangulo(rect1);
						}
						break;
		case SOLDADO  : {
						/* FRAMES:
						 * ATAQUE : 15 frames
						 * CAMINAR : 10 frames
						 * MUERE : (NO ENCONTRE SPRITES)
						 * */
						posicion.x += 0.25 * ANCHO_PIXEL_PASTO;
						posicion.y -= DISTANCIA_ENTRE_Y;
						posicion.w = 0.5*ANCHO_PIXEL_PASTO;
						posicion.h = 1.25*ALTO_PIXEL_PASTO;
						sprite = new Sprite(DIRECCIONES,10,this->getImagenTipo(entidad->getTipo()),posicion,escenario,coord_ceros,entidad);

						int x_ini = posicion.x;
						int y_ini = posicion.y;
						Rectangulo rect1(x_ini + 0.3*posicion.w, 0.3*posicion.w, y_ini + 0.1 * posicion.h, 0.8*posicion.h);
						sprite->agregarRectangulo(rect1);
						}
						break;

		case ALDEANO :{
						/* FRAMES:
						 * ATAQUE : 15 frames
						 * CAMINAR : 15 frames
						 * MUERE : 10 frames
						 * */
						posicion.x += ANCHO_PIXEL_PASTO * 0.25;
						posicion.y -= DISTANCIA_ENTRE_Y;
						posicion.w = ANCHO_PIXEL_PASTO*0.8;// / 2;
						posicion.h = 1.25*ALTO_PIXEL_PASTO;// * 3 / 4;
						sprite = new Sprite(DIRECCIONES,15,this->getImagenTipo(entidad->getTipo()),posicion,escenario,coord_ceros,entidad);
						//SDL_SetTextureColorMod(imagenPetrificadaSoldado->getTexture(),150,150,150);
						//sprite->agregarImagenPetrificada(imagenPetrificadaSoldado);

						int x_ini = posicion.x;
						int y_ini = posicion.y;
						Rectangulo rect1(x_ini + 0.3*posicion.w, 0.3*posicion.w, y_ini + 0.1 * posicion.h, 0.8*posicion.h);
						sprite->agregarRectangulo(rect1);
						}
						break;
		case ARQUERO :{
						/* FRAMES:
						 * ATAQUE : 10 frames
						 * CAMINAR : 10 frames
						 * MUERE : 10 frames
						 * */
						//posicion.x += ANCHO_PIXEL_PASTO * 0.25;
						posicion.y -= 25;
						posicion.w = 1.25*ANCHO_PIXEL_PASTO;
						posicion.h = 1.75*ALTO_PIXEL_PASTO;
						sprite = new Sprite(DIRECCIONES,10,this->getImagenTipo(entidad->getTipo()),posicion,escenario,coord_ceros,entidad);
						//SDL_SetTextureColorMod(imagenPetrificadaSoldado->getTexture(),150,150,150);
						//sprite->agregarImagenPetrificada(imagenPetrificadaSoldado);

						int x_ini = posicion.x;
						int y_ini = posicion.y;
						Rectangulo rect1(x_ini + 0.3*posicion.w, 0.3*posicion.w, y_ini + 0.1 * posicion.h, 0.8*posicion.h);
						sprite->agregarRectangulo(rect1);
						}
						break;
		case ANIMAL :	{
						/* FRAMES:
						 * COMER : 10 frames
						 * */
						posicion.x += ANCHO_PIXEL_PASTO / 8;
						//posicion.y -= (DISTANCIA_ENTRE_Y);
						posicion.w = ANCHO_PIXEL_PASTO * 0.8;
						posicion.h = ALTO_PIXEL_PASTO;
						sprite = new Sprite(DIRECCIONES,10,this->getImagenTipo(ANIMAL),posicion,escenario,coord_ceros,entidad);
						sprite->activarMovimiento(true);
						sprite->setDireccion(NORTE);

						int x_ini = posicion.x;
						int y_ini = posicion.y;
						Rectangulo rect1(x_ini + 0.3*ANCHO_PIXEL_PASTO, 0.6* ANCHO_PIXEL_PASTO, y_ini + 0.2*posicion.h, 0.6*posicion.h);
						sprite->agregarRectangulo(rect1);
						}
						break;
		default       :
						/* AGUA ó TIERRA */
						{sprite = new Sprite(1,1,this->getImagenTipo(entidad->getTipo()),posicion,escenario,coord_ceros,entidad);

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
	std::cout<<"entidad : "<<entidad->getInfo()<<"   Posición : "<< entidad->getPosicion().x<<" "<<entidad->getPosicion().y<<std::endl;//
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
	return it->second;
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
	Imagen *imagen = Loader::cargarImagen(this->renderer,"images/utils_barra_estado/barra_fondo.png");
	this->mapImagenesUtil->insert(BARRA_FONDO,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/utils_barra_estado/barra_descripcion.png");
	this->mapImagenesUtil->insert(BARRA_DESCRIPCION,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/utils/barra_negra.png");
	this->mapImagenesUtil->insert(BARRA_NEGRA,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/utils/selector_tile_0.png");
	this->mapImagenesUtil->insert(SELECT_TILE,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/utils/selector_tile_01.png");
	this->mapImagenesUtil->insert(SELECT_TILE_01,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/utils/selector_tile_02.png");
	this->mapImagenesUtil->insert(SELECT_TILE_02,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/utils/selector_tile_03.png");
	this->mapImagenesUtil->insert(SELECT_TILE_03,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/utils/selector_tile_04.png");
	this->mapImagenesUtil->insert(SELECT_TILE_04,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/utils/ubicacion_minimapa.png");
	this->mapImagenesUtil->insert(CUADRO_UBICACION,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/utils/icono_rojo_1.png");
	this->mapImagenesUtil->insert(ICONO_ROJO,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/utils/icono_azul_1.png");
	this->mapImagenesUtil->insert(ICONO_AZUL,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/utils/icono_amarillo_1.png");
	this->mapImagenesUtil->insert(ICONO_AMARILLO,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/utils/icono_naranja_1.png");
	this->mapImagenesUtil->insert(ICONO_NARANJA,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/utils/icono_verde_2.png");
	this->mapImagenesUtil->insert(ICONO_VERDE,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/utils/icono_rosa_1.png");
	this->mapImagenesUtil->insert(ICONO_ROSA,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/utils/icono_blanco_1.png");
	this->mapImagenesUtil->insert(ICONO_BLANCO,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/utils/icono_gris_1.png");
	this->mapImagenesUtil->insert(ICONO_GRIS,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/utils/icono_violeta_1.png");
	this->mapImagenesUtil->insert(ICONO_VIOLETA,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/utils/capa_gris.png");
	this->mapImagenesUtil->insert(CAPA_GRIS,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/utils/capa_negra.png");
	this->mapImagenesUtil->insert(CAPA_NEGRA,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/utils/cuadrado_magenta_1.png");
	this->mapImagenesUtil->insert(CUADRADO_MAGENTA,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/utils/cuadrado_verde_1.png");
	this->mapImagenesUtil->insert(CUADRADO_VERDE,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/utils/cuadrado_azul_1.png");
	this->mapImagenesUtil->insert(CUADRADO_AZUL,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/utils/cuadrado_rojo_1.png");
	this->mapImagenesUtil->insert(CUADRADO_ROJO,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/utils/iconos_edificios.png");
	this->mapImagenesUtil->insert(HERRAMIENTAS_ALDEANO,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/utils_barra_estado/arco_arquero.png");
	this->mapImagenesUtil->insert(ARCO_ARQUERO,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/utils_barra_estado/espada_soldado.png");
	this->mapImagenesUtil->insert(ESPADA_SOLDADO,imagen);
}


/********************************************************************************/
void ContenedorDeRecursos::cargarImagenesRecursos(){
	Imagen *imagen = Loader::cargarImagen(this->renderer,"images/utils/moneda_1.png");
	this->mapImagenes->insert(ORO,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/utils/madera_1.png");
	this->mapImagenes->insert(MADERA,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/utils/comida_1.png");
	this->mapImagenes->insert(COMIDA,imagen);

	imagen = Loader::cargarImagen(this->renderer,"images/utils/piedra.png");
	this->mapImagenes->insert(PIEDRA,imagen);
}

/********************************************************************************/
Imagen* ContenedorDeRecursos::getImagenUtilTipo(TipoImagenUtil tipo){
	map<TipoImagenUtil,Imagen* >::iterator p = this->mapImagenesUtil->find(tipo);
	Imagen* imagen = (*p).second;
	return imagen;
}

/********************************************************************************/

InfoEntidad ContenedorDeRecursos::getInfoTipo(TipoEntidad tipo){
	InfoEntidad info = mapInfoEntidades[tipo];
	return info;
}

/********************************************************************************/
/*void ContenedorDeRecursos::setearCanalAlphaParaEntidad(Entidad* entidad, int canalAlpha){
	Sprite* spriteEntidad = this->getSpriteDeEntidad(entidad);
	SDL_SetTextureAlphaMod(spriteEntidad->getImagen()->getTexture(),canalAlpha);
}
*/
/********************************************************************************/
void ContenedorDeRecursos::borrarSpriteDeEntidad(Entidad* entidad) {
	map<Entidad*, Sprite*>::iterator it = this->mapSpritesEntidades->find(entidad);
	if (it != this->mapSpritesEntidades->end()) {
		delete (it->second);
		this->mapSpritesEntidades->erase(it->first);
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

	map<TipoImagenUtil, Imagen* >::iterator itt = this->mapImagenesUtil->begin();
	while (itt != this->mapImagenesUtil->end()){
		Imagen *imagen = itt->second;
		itt++;
		delete imagen;
	}
	delete this->mapImagenesUtil;

	map<Entidad*, Sprite* >::iterator itttt = this->mapSpritesEntidades->begin();
	while (itttt != this->mapSpritesEntidades->end()){
		Sprite *sprite = itttt->second;
		itttt++;
		delete sprite;
	}
	delete this->mapSpritesEntidades;
}

