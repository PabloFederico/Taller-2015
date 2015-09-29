/*
 * Sprite.cpp
 *
 *  Created on: 16 de sept. de 2015
 *      Author: emanuel
 */

#include "Sprite.h"

Sprite::Sprite(int cant_Direcciones, Uint32 cant_Img_Distintas, Imagen* imagen){
	this->cant_Direcciones = cant_Direcciones;
	this->cant_Img_Distintas = cant_Img_Distintas;
	this->imagen = imagen;
	this->frames = new SDL_Rect*[cant_Direcciones];
	this->ticks =SDL_GetTicks(); //Veo cuantos ciclos va
	this->segundos = ticks/1000 ; // convierto esa cantidad de ticks a segundos (me interesa la parte entera nomas)
	this->num_sprite = segundos/cant_Img_Distintas; // Quiero un frame por segundo, divido los segs por cant de frames y cuando eso me de  mayor que
														// cant de img distintas, ahi vuelvo a animar otravez

	for (int i = 0; i < cant_Direcciones; i++){
		this->frames[i] = new SDL_Rect[cant_Img_Distintas];
	}
	this->fps = 30;
	this->delay = 0;
	this->milisegundos = SDL_GetTicks();

	this->cargarFrames();

	this->direccion = 0;
	this->indexSpriteActual = 0;

	this->frameActual = this->frames[this->direccion][this->indexSpriteActual];
}

/********************************************************************************/
void Sprite::cargarFrames(){
	int posX;
	int posY = 1;
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
void Sprite::setDireccion(int direccion){
	this->direccion = direccion;
	this->indexSpriteActual = 0;
	this->frameActual = this->frames[direccion][0];
}

/********************************************************************************/
void Sprite::efectuarMovimiento(){
	if (this->num_sprite < this->cant_Img_Distintas){
		//SDL_Delay(this->delay);
		this->indexSpriteActual++;
	}
	else this->indexSpriteActual = 0;

	this->frameActual = this->frames[this->direccion][this->indexSpriteActual%this->cant_Img_Distintas];

}

/********************************************************************************/
SDL_Rect Sprite::getSDLRectActual(){
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
	this->milisegundos = SDL_GetTicks();
}

/********************************************************************************/
int Sprite::currentTime(){
	int currentTime = SDL_GetTicks();
	return currentTime - this->milisegundos;
}

/********************************************************************************/
void Sprite::acomodar(){

	this->frameActual = this->frames[this->direccion][this->direccion];
}
/********************************************************************************/
Sprite::~Sprite() {
	for (int i = 0; i < this->cant_Direcciones; i++){
		delete[] this->frames[i];
	}
	delete[] this->frames;
}

