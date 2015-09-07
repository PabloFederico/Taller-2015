/*
 * Escenario.cpp
 *
 *  Created on: 2 de sept. de 2015
 *      Author: pgfederi
 */
#include <stdio.h>

#include "Escenario.h"

Escenario::Escenario(int ancho, int largo){
	this->size_x = ancho;
	this->size_y = largo;
	this->mapEntidades = new std::map<std::pair<int,int>, std::vector<Entidad*>* >();
}

std::pair<int,int> Escenario::getDimension(){
	return std::make_pair(this->size_x,this->size_y);
}

std::map<std::pair<int,int>, std::vector<Entidad*>* >* Escenario::getPosEntidades(){
	return this->mapEntidades;
}

void Escenario::agregarEntidad(std::pair<int,int> pos, Entidad* entidad){
	std::map<std::pair<int,int>,std::vector<Entidad*>* >::iterator p = this->mapEntidades->find(pos);
	if (p == this->mapEntidades->end()){
		std::vector<Entidad*> *entidades = new std::vector<Entidad*>();
		entidades->push_back(entidad);
		this->mapEntidades->insert(std::make_pair(pos,entidades));
	}
	else (*p).second->push_back(entidad);
}

Escenario::~Escenario() {
	std::map<std::pair<int,int>,std::vector<Entidad*>* >::iterator p = this->mapEntidades->begin();
	while (p != this->mapEntidades->end()){
		std::vector<Entidad*> *vectorEntidades = (*p).second;
		p++;
		delete vectorEntidades;
	}
	delete this->mapEntidades;
}

