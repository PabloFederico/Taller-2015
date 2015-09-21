/*
 * Escenario.cpp
 *
 *  Created on: 2 de sept. de 2015
 *      Author: pgfederi
 */
#include <stdio.h>
#include "Escenario.h"

Escenario::Escenario(InfoEscenario info){
	this->size_x = info.size_x;
	this->size_y = info.size_y;
	//this->mapPosicionesEntidades = new std::map<std::pair<int,int>, std::vector<Entidad*>* >();
	this->mapPosicionesEntidades = new Map<pair<int,int>,vector<Entidad*>*>();

	map<pair<int,int>,vector<TipoEntidad> > mapAux = info.getPosicionesEntidades();
	map<pair<int,int>,vector<TipoEntidad> >::iterator it = info.getPosicionesEntidades().begin();
	while(it != info.getPosicionesEntidades().end()){
		pair<int,int> pos = (*it).first;
		vector<TipoEntidad> tipos = mapAux[pos];
		for (unsigned i = 0; i < tipos.size(); i++){
			TipoEntidad tipo = TipoEntidad(tipos[i]);
			Entidad *entidad = new Entidad(tipo);
			this->agregarEntidad(pos,entidad);
		}
		it++;
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
Map<pair<int,int>, vector<Entidad*>* >* Escenario::getPosEntidades(){
	return this->mapPosicionesEntidades;
}

/********************************************************************************/
void Escenario::agregarEntidad(pair<int,int> pos, Entidad* entidad){
	map<std::pair<int,int>,vector<Entidad*>* >::iterator p = this->mapPosicionesEntidades->find(pos);
	if (p == this->mapPosicionesEntidades->end()){
		vector<Entidad*> *entidades = new vector<Entidad*>();
		entidades->push_back(entidad);
		//this->mapPosicionesEntidades->insert(std::make_pair(pos,entidades));
		this->mapPosicionesEntidades->insert(pos,entidades);
	}
	else (*p).second->push_back(entidad);
}

/********************************************************************************/
Escenario::~Escenario() {
	map<pair<int,int>,vector<Entidad*>* >::iterator p = this->mapPosicionesEntidades->begin();
	while (p != this->mapPosicionesEntidades->end()){
		vector<Entidad*> *vectorEntidades = (*p).second;
		for (unsigned i = 0; i < vectorEntidades->size(); i++){
			Entidad* entidad = (*vectorEntidades)[i];
			delete entidad;
		}
		p++;
		delete vectorEntidades;	// ¿Esto borra sólo el vector o también hace delete de cada entidad en el mismo?
	}
	delete this->mapPosicionesEntidades;
}

