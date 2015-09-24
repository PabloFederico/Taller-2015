/*
 * Escenario.cpp
 *
 *  Created on: 2 de sept. de 2015
 *      Author: pgfederi
 */
#include "Escenario.h"

Escenario::Escenario(InfoEscenario info){
	this->size_x = info.size_x;
	this->size_y = info.size_y;
	this->posicionesEntidades = new vector<PosEntidad>();

	vector<PosTipoEntidad> posEntidades = info.getPosicionesEntidades();
	for (unsigned i = 0; i < posEntidades.size(); i++){
		int x = posEntidades[i].x;
		int y = posEntidades[i].y;
		TipoEntidad tipo = posEntidades[i].tipo;
		Entidad *entidad = new Entidad(tipo);
		pair<int,int> pos(x,y);
		this->agregarEntidad(pos,entidad);
	}

	this->protagonista = new Entidad(info.protagonista);
	pair<int,int> pos(info.posX_protagonista,info.posY_protagonista);
	this->agregarEntidad(pos,this->protagonista);
}

/********************************************************************************/
Entidad* Escenario::getProtagonista(){
	return this->protagonista;
}

/********************************************************************************/
pair<int,int> Escenario::getDimension(){
	return std::make_pair(this->size_x,this->size_y);
}

/********************************************************************************/
vector<PosEntidad>* Escenario::getVectorEntidades(){
	return this->posicionesEntidades;
}

/********************************************************************************/
void Escenario::agregarEntidad(pair<int,int> pos, Entidad* entidad){
	PosEntidad posEntidad(pos.first,pos.second,entidad);
	this->posicionesEntidades->push_back(posEntidad);
}

/********************************************************************************/
Escenario::~Escenario() {
	for (unsigned i = 0; i < posicionesEntidades->size(); i++){
		Entidad* entidad = (*posicionesEntidades)[i].entidad;
		delete entidad;
	}
	delete this->posicionesEntidades;
}

