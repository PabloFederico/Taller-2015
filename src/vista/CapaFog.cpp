/*
 * CapaNegra.cpp
 *
 *  Created on: 1 de oct. de 2015
 *      Author: emanuel
 */

#include "CapaFog.h"

CapaFog::CapaFog(int tiles_x, int tiles_y) {
	this->tiles_x = tiles_x;
	this->tiles_y = tiles_y;

	this->tiles = new EstadoCapa*[tiles_x];

	for (int i = 0; i < tiles_x; i++){
		this->tiles[i] = new EstadoCapa[tiles_y];
	}
	for (int i = 0; i < tiles_x; i++){
		for (int j = 0; j < tiles_y; j++){
			this->tiles[i][j] = ESTADO_NEGRO;
		}
	}
}

pair<int,int> CapaFog::getDimension(){
	return make_pair(tiles_x,tiles_y);
}

vector<pair<int,int> > CapaFog::generarPuntos(int x, int y){
	int cant_quitar = 10;
	vector<pair<int,int> > puntos;
	int x_relativo;
	int y_relativo = y - cant_quitar;
	int aux = x - cant_quitar;
	for (int i = 0; i < 2 * cant_quitar + 1; i++){
		x_relativo = aux;
		for (int j = 0; j < 2 * cant_quitar + 1; j++){
			if ((x_relativo >= 0) && (x_relativo < tiles_x) && (y_relativo >= 0) && (y_relativo < tiles_y)){
				puntos.push_back(make_pair(x_relativo,y_relativo));
			}
			x_relativo++;
		}
		y_relativo++;
	}
	return puntos;
}

void CapaFog::descubrirDesdePunto(int x, int y){
	this->actualizarCapaGris();
	vector<pair<int,int> > puntos = this->generarPuntos(x,y);
	for (unsigned i = 0; i < puntos.size(); i++){
		int tileX = puntos[i].first;
		int tileY = puntos[i].second;
		this->tiles[tileX][tileY] = ESTADO_COLOR;
	}

}

void CapaFog::actualizarCapaGris(){
	for (int i = 0; i < tiles_x; i++){
		for (int j = 0; j < tiles_y; j++){
			if (this->tiles[i][j] == ESTADO_COLOR){
				this->tiles[i][j] = ESTADO_GRIS;
			}
		}
	}
}

EstadoCapa CapaFog::getEstadoTile(int x, int y){
	return this->tiles[x][y];
}

CapaFog::~CapaFog() {
	for (int i = 0; i < tiles_x; i++){
		delete[] this->tiles[i];
	}
	delete[] this->tiles;
}

