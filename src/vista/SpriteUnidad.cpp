/*
 * SpriteUnidad.cpp
 *
 *  Created on: 25 de nov. de 2015
 *      Author: emanuel
 */

#include "SpriteUnidad.h"

SpriteUnidad::SpriteUnidad(int cant_dir, int frames_dif, Imagen* image, SDL_Rect pos, Escenario* esc, Coordenada c_ceros, Entidad* entidad): Sprite(cant_dir, frames_dif,image, pos, esc, c_ceros, entidad) {
	estadoSprite = QUIETO;
	mapRectEstado[CAMINANDO] = pos;
	framesCaminata = frames;
	framesConstruccion = NULL;
	framesAtaque = NULL;
	framesMuerte = NULL;
	framesQuieto = NULL;
	imagenCaminando = image;
	imagenConstruyendo = NULL;
	imagenQuieto = NULL;
	imagenAtaque = NULL;
	imagenMuerte = NULL;
	FRAMES_CAMINATA = frames_dif;
	FRAMES_QUIETO = 1;
	FRAMES_CONSTRUYENDO = 15; // // Para el ALDEANO

	FPS_CAMINATA = 30;
	FPS_ATAQUE = 20;
	FPS_CONSTRUYENDO = 20;
	FPS_MUERTE = 10;

	SDL_Rect rectAtaque = pos;
	SDL_Rect rectQuieto = pos;
	SDL_Rect rectMuere = pos;
	SDL_Rect rectConstruccion = pos;

	switch (entidad->getTipo()){
		case ALDEANO:
					 rectAtaque.x -= 20;
					 rectAtaque.y += 5;
					 rectAtaque.w *= 2.2;
					 rectAtaque.h *= 1.3;

					 rectMuere.x -= 40;
					 rectMuere.y += 5;
					 rectMuere.w *= 2;
					 rectMuere.h *= 1;

					 rectQuieto.h *= 1;
					 rectQuieto.w *= 0.9;
					 rectQuieto.x -= 10;
					 //rectQuieto.y -= 10;

					 rectConstruccion.w *= 1.3;
					 rectConstruccion.h *= 1;

					 FRAMES_ATAQUE = 15;
					 FRAMES_MUERTE = 15;

					 break;
		case ARQUERO:

					 rectAtaque.x -= 15;
					 rectAtaque.y -= 2;
					 rectAtaque.w *= 1.30;

					 rectMuere.x -= 35;
					 rectMuere.w *= 2;

					 FPS_CAMINATA = 20;
					 FPS_ATAQUE = 8;

					 rectQuieto.x -= 5;
					 rectQuieto.y += 3;
					 rectQuieto.h *= 1;

					 FRAMES_ATAQUE = 10;
					 FRAMES_MUERTE = 10;
					 break;
		case SOLDADO:
					 rectAtaque.x -= 17;
					 rectAtaque.y -= 7;
					 rectAtaque.w *= 2.5;
					 rectAtaque.h *= 1.3;

					 // rectMuere NO PROBADO (porque no tenemos sprite de esto)
					 rectMuere.x -= 35;
					 rectMuere.y -= 14;
					 rectMuere.w *= 2.4;
					 rectMuere.h *= 2;

					 rectQuieto.w *= 1.3;
					 rectQuieto.h *= 1;

					 //rectMuere = rectQuieto; // TODO solo para pasar pruebas

					 FRAMES_ATAQUE = 15;
					 FRAMES_MUERTE = 15;
					 break;
		default : break;
	}

	mapRectEstado[ATACANDO] = rectAtaque;
	mapRectEstado[MURIENDO] = rectMuere;
	mapRectEstado[CONSTRUYENDO] = rectConstruccion;
	// TODO RECOLECTANDO, TALANDO ???
	mapRectEstado[QUIETO] = rectQuieto;
}

void SpriteUnidad::cargarFramesAtaque(Imagen* image){
	imagenAtaque = image;
	framesAtaque = new SDL_Rect*[cant_Direcciones];
	for (int i = 0; i < cant_Direcciones; i++){
		this->framesAtaque[i] = new SDL_Rect[FRAMES_ATAQUE];
	}
	cargarFrames(cant_Direcciones,FRAMES_ATAQUE,image,framesAtaque);
}

void SpriteUnidad::cargarFramesConstruccion(Imagen* image){
	imagenConstruyendo = image;
	framesConstruccion = new SDL_Rect*[cant_Direcciones];
	for (int i = 0; i < cant_Direcciones; i++){
		this->framesConstruccion[i] = new SDL_Rect[FRAMES_CONSTRUYENDO];
	}
	cargarFrames(cant_Direcciones,FRAMES_CONSTRUYENDO,image,framesConstruccion);
}

void SpriteUnidad::cargarFramesQuieto(Imagen* image){
	imagenQuieto = image;
	framesQuieto = new SDL_Rect*[cant_Direcciones];
	for (int i = 0; i < cant_Direcciones; i++){
		this->framesQuieto[i] = new SDL_Rect[FRAMES_QUIETO];
	}
	cargarFrames(cant_Direcciones,FRAMES_QUIETO,image,framesQuieto);
}

void SpriteUnidad::cargarFramesMuerte(Imagen* image){
	imagenMuerte = image;
	framesMuerte = new SDL_Rect*[cant_Direcciones];
	for (int i = 0; i < cant_Direcciones; i++){
		this->framesMuerte[i] = new SDL_Rect[FRAMES_MUERTE];
	}
	cargarFrames(cant_Direcciones,FRAMES_MUERTE,image,framesMuerte);
}

void SpriteUnidad::setPosX(int x){
	int dif = x - posicion.x;
	int diferencia = rectangulos[0].x_ini - posicion.x;
	this->posicion.x = x;
	for (unsigned i = 0; i < rectangulos.size(); i++){
		rectangulos[i].x_ini = x + diferencia;
	}
	map<EstadoEntidad, SDL_Rect>::iterator p = mapRectEstado.begin();
	while (p != mapRectEstado.end()){
		SDL_Rect rect = mapRectEstado[p->first];
		rect.x += dif;
		mapRectEstado[p->first] = rect;
		p++;
	}
}

void SpriteUnidad::setPosY(int y){
	int dif = y - posicion.y;
	int diferencia = rectangulos[0].y_ini - posicion.y;
	this->posicion.y = y;
	for (unsigned i = 0; i < rectangulos.size(); i++){
		rectangulos[i].y_ini = y + diferencia;
	}
	map<EstadoEntidad, SDL_Rect>::iterator p = mapRectEstado.begin();
	while (p != mapRectEstado.end()){
		SDL_Rect rect = mapRectEstado[p->first];
		rect.y += dif;
		mapRectEstado[p->first] = rect;
		p++;
	}
}

void SpriteUnidad::mover(int cant_x, int cant_y){
	this->posicion.x += cant_x;
	this->posicion.y += cant_y;
	regPos.x_anterior += cant_x;
	regPos.y_anterior += cant_y;
	regPos.posX_player += cant_x;
	regPos.posY_player += cant_y;
	for (unsigned i = 0; i < rectangulos.size(); i++){
		rectangulos[i].x_ini += cant_x;
		rectangulos[i].y_ini += cant_y;
	}

	for(unsigned i = 0; i < caminoARecorrer.size(); i++){
		caminoARecorrer[i].x += cant_x;
		caminoARecorrer[i].y += cant_y;
	}

	map<EstadoEntidad, SDL_Rect>::iterator p = mapRectEstado.begin();
	while (p != mapRectEstado.end()){
		SDL_Rect rect = mapRectEstado[p->first];
		rect.x += cant_x;
		rect.y += cant_y;
		mapRectEstado[p->first] = rect;
		p++;
	}
}

Coordenada SpriteUnidad::getPosPies(){
	switch (estadoSprite){
	case CAMINANDO : return Coordenada( posicion.x + (posicion.w / 2), posicion.y + posicion.h );
	case ATACANDO : return Coordenada( posicion.x + (posicion.w / 2), posicion.y + posicion.h * 0.75 );
	case QUIETO : return Coordenada(posicion.x + (posicion.w / 3), posicion.y + posicion.h);
	default : return Coordenada( posicion.x + (posicion.w / 2), posicion.y + posicion.h );
	}
}

void SpriteUnidad::verificarEstadoEntidadConImagen(){
	if (entidad->getEstado() != estadoSprite){
		estadoSprite = entidad->getEstado();
		switch (estadoSprite){
		case CAMINANDO:
					   imagen = imagenCaminando;
					   posicion = mapRectEstado[CAMINANDO];
					   fps = FPS_CAMINATA;
					   frames = framesCaminata;
					   cant_Img_Distintas = FRAMES_CAMINATA;
				       break;
		case ATACANDO:
					   imagen = imagenAtaque;
					   posicion = mapRectEstado[ATACANDO];
					   fps = FPS_ATAQUE;
					   frames = framesAtaque;
					   cant_Img_Distintas = FRAMES_ATAQUE;
				       break;
		case MURIENDO:
					   imagen = imagenMuerte;
					   posicion = mapRectEstado[MURIENDO];
					   fps = FPS_MUERTE;
					   frames = framesMuerte;
					   fps = 10;
					   cant_Img_Distintas = FRAMES_MUERTE;
					   indexSpriteActual = 0;
				       break;
		case CONSTRUYENDO:
					   imagen = imagenConstruyendo;
					   posicion = mapRectEstado[CONSTRUYENDO];
					   frames = framesConstruccion;
					   fps = FPS_CONSTRUYENDO;
					   cant_Img_Distintas = FRAMES_CONSTRUYENDO;
			           break;
		default :
					   imagen = imagenQuieto;
					   posicion = mapRectEstado[QUIETO];
					   frames = framesQuieto;
					   cant_Img_Distintas = 1;
				       break;
		}
	}
}

void SpriteUnidad::update(int vel_personaje, Mix_Chunk* sonido_caminar){
	verificarEstadoEntidadConImagen();
	switch (entidad->getEstado()){
		case CAMINANDO:
						if (this->quedaCaminoPorRecorrer()) {

							// Activo sonido del movimiento:
							Mix_PlayChannel(1, sonido_caminar, 0);
							/*	- calcular distancia al sig punto
							 *  - si es mayor a 1, seguir moviendo la posicion
							 *    sino cambiar al siguiente punto del camino (analizando la dirección)
							 *  - si ya no quedan camino a recorrer, cambiar estado de activar movimiento y acomodar
							 * */
							Coordenada c_prox_pixel = this->getCaminoARecorrer()[0];

							c_prox_pixel.x -= this->getPosicion().w / 2;
							c_prox_pixel.y -= this->getPosicion().h;

							float distancia = Calculador::calcularDistanciaEntrePixeles(Coordenada(this->regPos.posX_player, this->regPos.posY_player), c_prox_pixel);

							if (distancia > 1){
								if (this->regPos.posX_player != c_prox_pixel.x) {
									float x_result = (this->regPos.posX_player - ((this->regPos.posX_player - c_prox_pixel.x) / distancia) * vel_personaje  * 0.05);
									this->setPosX(int(x_result));
									this->regPos.posX_player = x_result;
								}
								if (this->regPos.posY_player != c_prox_pixel.y) {
									float y_result = (this->regPos.posY_player - ((this->regPos.posY_player - c_prox_pixel.y) / distancia) * vel_personaje * 0.05);
									this->setPosY(int(y_result));
									this->regPos.posY_player = y_result;
								}
							} else {

								/* guardar última coordenada para desocuparla y cambiar a la próxima */
								Coordenada c_ult_punto = this->getCaminoARecorrer()[0];
								this->quitarPrimeraCoordenada();

								/* Seteamos la dirección para el siguiente punto. */
								if (this->getCaminoARecorrer().size() > 0){
									try {
										//this->escenario->desocuparTile(Calculador::tileParaPixel(c_ult_punto, coord_ceros));
										if (this->revisarCamino(c_ult_punto)) {
											Coordenada c_prox_punto = this->getCaminoARecorrer()[0];
											c_prox_punto.x -= this->getPosicion().w / 2;
											c_prox_punto.y -= this->getPosicion().h / 2;
											Direccion direccion = Calculador::calcularDireccion(c_prox_punto, coordPixelSprite());
											this->setDireccion(direccion);
										}
									} catch ( FueraDeEscenario &e ) { setearNuevoCamino(Camino(), coord_ceros); }
								} else // Se acaba de terminar el camino, que es de un solo paso.
									throw PasoCompletado(entidad->getIDJug());
							}

						} else {
							/* Cuando se deja de mover, se debería quedar en
							 * una posición firme correspondiente a su dirección. */
							 this->activarMovimiento(false);
							 entidad->cambioEstado(QUIETO);
							 // Paro el sonido:
							 Mix_HaltChannel(1);
						}
					   break;
		case MURIENDO :
						if (indexSpriteActual+1 == FRAMES_MUERTE)
							entidad->cambioEstado(MUERTO);
						break;

		default : break;
	}
/*
	if (estadoSprite != QUIETO){
		if (currentTime() > (1000/getFps())){
			efectuarMovimiento();
		}
	}
*/

           // bool hayColision = DetectorDeColisiones::verificarColisiones(this,juego->getSpritesEntidades());
           // if (hayColision) this->activarMovimiento(false);
}

SpriteUnidad::~SpriteUnidad() {
	for (int i = 0; i < this->cant_Direcciones; i++){
		delete[] this->framesCaminata[i];
	}
	delete[] this->framesCaminata;

	for (int i = 0; i < this->cant_Direcciones; i++){
		delete[] this->framesAtaque[i];
	}
	delete[] this->framesAtaque;

	for (int i = 0; i < this->cant_Direcciones; i++){
		delete[] this->framesQuieto[i];
	}
	delete[] this->framesQuieto;

	for (int i = 0; i < this->cant_Direcciones; i++){
		delete[] this->framesMuerte[i];
	}
	delete[] this->framesMuerte;

	if (entidad->getTipo() == ALDEANO){
		for (int i = 0; i < this->cant_Direcciones; i++){
			delete[] this->framesConstruccion[i];
		}
		delete[] this->framesConstruccion;
	}
}

