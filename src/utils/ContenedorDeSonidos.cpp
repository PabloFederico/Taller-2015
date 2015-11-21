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
	if (caminar == NULL) printf("Error al cargar la cancion. Error: %s\n", Mix_GetError());
	this->mapSonidos->insert(CAMINAR,caminar);

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
