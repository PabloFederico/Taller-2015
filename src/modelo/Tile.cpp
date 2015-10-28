/*
 * Tile.cpp
 *
 *  Created on: 25 de oct. de 2015
 *      Author: emanuel
 */

#include "Tile.h"
#include <algorithm>


Tile::Tile() {}

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

void Tile::eliminarEntidad(Entidad* entidad){
	vector<Entidad*>::iterator it = find(entidades.begin(), entidades.end(), entidad);
	if (it != entidades.end())
		entidades.erase(it);
	else throw NoSeRecibio();
}

Entidad* Tile::quitarEntidad(int id_jug) {
	vector<Entidad*>::iterator it;
	for (it = entidades.begin(); it < entidades.end(); ++it)
		if ((*it)->getIDJug() == id_jug)
			break;
	if (it == entidades.end())
		throw NoSeRecibio();
	Entidad* ent = *it;
	entidades.erase(it);
	return ent;
}

Tile::~Tile() {
	entidades.clear();
}
