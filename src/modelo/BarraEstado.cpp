/*
 * BarraInfo.cpp
 *
 *  Created on: 21 de oct. de 2015
 *      Author: emanuel
 */

#include "../modelo/BarraEstado.h"

BarraEstado::BarraEstado(int ancho, int alto) {
	this->height = alto;
	this->width = ancho;
	mapRecursosEconomicos[ORO] = 0;
	mapRecursosEconomicos[MADERA] = 0;
	mapRecursosEconomicos[COMIDA] = 0;
}

std::pair<int, int> BarraEstado::getDimension(){
	return std::make_pair(width,height);
}

void BarraEstado::agregarRecursoEconomico(TipoRecurso tipo){
	mapRecursosEconomicos[tipo]++;
}

std::map<TipoRecurso, int> BarraEstado::getRecursosEconomicos(){
	return mapRecursosEconomicos;
}

BarraEstado::~BarraEstado() {
	mapRecursosEconomicos.clear();
}

