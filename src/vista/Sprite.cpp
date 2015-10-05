/*
 * Sprite.cpp
 *
 *  Created on: 16 de sept. de 2015
 *      Author: emanuel
 */

#include "../vista/Sprite.h"

/********************************************************************************/
Sprite::Sprite(int cant_Direcciones, Uint32 cant_Img_Distintas, Imagen* imagen, SDL_Rect posicion){
	this->cant_Direcciones = cant_Direcciones;
	this->cant_Img_Distintas = cant_Img_Distintas;
	this->imagen = imagen;
	this->posicion = posicion;
	this->frames = new SDL_Rect*[cant_Direcciones];

	for (int i = 0; i < cant_Direcciones; i++){
		this->frames[i] = new SDL_Rect[cant_Img_Distintas];
	}
	this->fps = 30;
	this->delay = 0;
	this->miliseg_inicial = SDL_GetTicks();
	this->tiempoTranscurridoAlFinDeCiclo = 0;

	this->cargarFrames();

	this->direccion = 0;
	this->indexSpriteActual = 0;

	this->enMovimiento = false;

	this->frameActual = this->frames[this->direccion][this->indexSpriteActual];
}

/********************************************************************************/
void Sprite::cargarFrames(){
	int posX;
	int posY = 0;
	int ancho = this->imagen->getPixelsX() / this->cant_Img_Distintas;
	int alto = this->imagen->getPixelsY() / this->cant_Direcciones;
	for (int i = 0; i < this->cant_Direcciones; i++){
		posX = 0;
		for (int j = 0; j < this->cant_Img_Distintas; j++){
			SDL_Rect frame;
			frame.x = posX;
			frame.y = posY;
			frame.w = ancho;
			frame.h = alto;
			this->frames[i][j] = frame;
			posX = posX + ancho;
		}
		posY = posY + alto;
	}
}
/********************************************************************************/
Imagen* Sprite::getImagen(){
	return this->imagen;
}

/********************************************************************************/
SDL_Rect Sprite::getPosicion(){
	return this->posicion;
}

/********************************************************************************/
void Sprite::setPosX(int x){
	this->posicion.x = x;
}

void Sprite::setPosY(int y){
	this->posicion.y = y;
}

/********************************************************************************/
void Sprite::mover(int cant_x, int cant_y){
	this->posicion.x += cant_x;
	this->posicion.y += cant_y;
}

/********************************************************************************/
void Sprite::setDireccion(int direccion){
	this->direccion = direccion;
	this->indexSpriteActual = 0;
	this->frameActual = this->frames[direccion][0];
}

/********************************************************************************/
bool Sprite::estaEnMovimiento(){
	return this->enMovimiento;
}

/********************************************************************************/
void Sprite::activarMovimiento(bool valor){
	this->enMovimiento = valor;
}

/********************************************************************************/
void Sprite::efectuarMovimiento(){
	if (this->delay && this->indexSpriteActual == 0){
		tiempoTranscurridoAlFinDeCiclo += currentTime();
		if (tiempoTranscurridoAlFinDeCiclo > this->delay){
			this->indexSpriteActual++;
			tiempoTranscurridoAlFinDeCiclo = 0;
		}
	}else{
		if (this->indexSpriteActual < this->cant_Img_Distintas)
			this->indexSpriteActual++;
		else this->indexSpriteActual = 0;
	}

	this->frameActual = this->frames[this->direccion][this->indexSpriteActual % cant_Img_Distintas];
	resetTime();
}

/********************************************************************************/
SDL_Rect Sprite::getFrameActual(){
	return this->frameActual;
}

/********************************************************************************/
void Sprite::setFps(int fps){
	this->fps = fps;
}

/********************************************************************************/
int Sprite::getFps(){
	return this->fps;
}

/********************************************************************************/
SDL_Rect Sprite::getSDLRect(int i, int j){
	return this->frames[i][j];
}

/********************************************************************************/
void Sprite::setDelay(int delay){
	this->delay = delay;
}

/********************************************************************************/
void Sprite::resetTime(){
	this->miliseg_inicial = SDL_GetTicks();
}

/********************************************************************************/
int Sprite::currentTime(){
	int currentTime = SDL_GetTicks();
	return currentTime - this->miliseg_inicial;
}

/********************************************************************************/
void Sprite::acomodar(){
	this->frameActual = this->frames[this->direccion][cant_Direcciones-1];
}
/********************************************************************************/
Sprite::~Sprite() {
	for (int i = 0; i < this->cant_Direcciones; i++){
		delete[] this->frames[i];
	}
	delete[] this->frames;
}

