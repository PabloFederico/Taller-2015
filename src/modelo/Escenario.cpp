/*
 * Escenario.cpp
 *
 *  Created on: 2 de sept. de 2015
 *      Author: pgfederi
 */
#include "../modelo/Escenario.h"

Escenario::Escenario(InfoEscenario info){
	this->size_x = info.size_x;
	this->size_y = info.size_y;
	this->capa = new CapaNegra(size_x,size_y);

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

	info.getPosicionesEntidades().clear();

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

CapaNegra* Escenario::getCapa(){
	return this->capa;
}

/********************************************************************************/
Escenario::~Escenario() {
	for (unsigned i = 0; i < posicionesEntidades->size(); i++){
		Entidad* entidad = (*posicionesEntidades)[i].entidad;
		delete entidad;
	}
	this->posicionesEntidades->clear();
	delete this->posicionesEntidades;

	delete this->capa;
}

