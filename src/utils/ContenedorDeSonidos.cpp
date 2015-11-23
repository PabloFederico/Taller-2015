/*
 * ContenedorDeSonidos.cpp
 *
 *  Created on: 20 de nov. de 2015
 *      Author: guidonegri
 */
#include "../utils/ContenedorDeSonidos.h"
#include "../utils/Constantes.h"
#include "../utils/Loader.h"

/********************************************************************************/

ContenedorDeSonidos::ContenedorDeSonidos(){
	this->mapSonidos = new Map<TipoSonido, Mix_Chunk*>();
	this->cargarSonidos();
}

/********************************************************************************/

void ContenedorDeSonidos::cargarSonidos(){
	Mix_Chunk *caminar = Loader::cargarSonido("sound/pasoss.wav");
	if (caminar == NULL) printf("Error al cargar sonido CAMINAR. Error: %s\n", Mix_GetError());
	this->mapSonidos->insert(CAMINAR, caminar);

	Mix_Chunk *morir_humano = Loader::cargarSonido("sound/morir.wav");
	if (morir_humano == NULL) printf("Error al cargar sonido MORIR HUMANO. Error: %s\n", Mix_GetError());
	this->mapSonidos->insert(MORIR_HUMANO, morir_humano);

	Mix_Chunk *morir_animal = Loader::cargarSonido("sound/muerteanimal.wav");
	if (morir_animal == NULL) printf("Error al cargar sonido MORIR ANIMAL. Error: %s\n", Mix_GetError());
	this->mapSonidos->insert(MORIR_ANIMAL, morir_animal);

	Mix_Chunk *comer = Loader::cargarSonido("sound/comer.wav");
	if (comer == NULL) printf("Error al cargar sonido COMER. Error: %s\n", Mix_GetError());
	this->mapSonidos->insert(COMER, comer);

	Mix_Chunk *talar = Loader::cargarSonido("sound/talar.wav");
	if (talar == NULL) printf("Error al cargar sonido TALAR. Error: %s\n", Mix_GetError());
	this->mapSonidos->insert(TALAR, talar);


	//CARGAR LOS RESTANTES
}

/********************************************************************************/

Mix_Chunk* ContenedorDeSonidos::getSonidoTipo(TipoSonido tipo){
		map<TipoSonido,Mix_Chunk* >::iterator p = this->mapSonidos->find(tipo);
		Mix_Chunk* sonido = (*p).second;
		return sonido;
	}


/********************************************************************************/

ContenedorDeSonidos::~ContenedorDeSonidos() {
	map<TipoSonido, Mix_Chunk* >::iterator it = this->mapSonidos->begin();
	while (it != this->mapSonidos->end()){
		Mix_Chunk *sonido = it->second;
		it++;
		Mix_FreeChunk(sonido);
	}
	delete this->mapSonidos;
}
