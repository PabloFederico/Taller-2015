/*
 * Tile.cpp
 *
 *  Created on: 25 de oct. de 2015
 *      Author: emanuel
 */

#include "Tile.h"

Tile::Tile() {

}

bool Tile::estaLibre(){
	if (entidades.size() == 0) return true;
	if (entidades.size() == 1 && (!entidades[0]->ocupaSuTile())) return true;

	return false;
}

vector<Entidad*> Tile::getEntidades(){
	return entidades;
}

void Tile::agregarEntidad(Entidad* entidad){
	entidades.push_back(entidad);
}

Tile::~Tile() {
	//for (unsigned i = 0; i < entidades.size(); i++){
	//	delete entidades[i];
	for (std::vector<Entidad*>::iterator it = entidades.begin(); it < entidades.end(); ++it) {
		Entidad* aux = *it;
		delete aux;
	}
	entidades.clear();
}

