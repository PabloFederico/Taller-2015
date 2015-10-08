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

	vector<PosTipoEntidad> vecPosTipoEntidades = info.getPosicionesEntidades();
	for (unsigned i = 0; i < vecPosTipoEntidades.size(); i++){
		int x = vecPosTipoEntidades[i].x;
		int y = vecPosTipoEntidades[i].y;
		TipoEntidad tipo = vecPosTipoEntidades[i].tipo;
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

/********************************************************************************/
// Verifica que (x;y) corresponden a una posición en el escenario que no esté ocupado por Castillo. 	EXTENDER/GENERALIZAR
bool Escenario::tileEsOcupable(int x, int y) {
	if (x < 0 || y < 0 || x >= this->size_x || y >= this->size_y)
		return false;

	std::vector<PosEntidad>::iterator it;
	for (it = this->posicionesEntidades->begin(); it < this->posicionesEntidades->end(); ++it) {
		if (it->x == x && it->y == y && it->entidad->ocupaSuTile())
			return false;
	}
	return true;
}

/********************************************************************************/
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

