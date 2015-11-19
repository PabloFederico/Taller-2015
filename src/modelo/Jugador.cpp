/*
 * Jugador.cpp
 *
 *  Created on: 9 de nov. de 2015
 *      Author: emanuel
 */

#include "Jugador.h"

Jugador::Jugador(std::string nombre, int id) {
	this->nombre = nombre;
	id_jug = id;
	centroCivico = NULL;
	contador_dni_edificios = 0;
	contador_dni_unidades = 0;
	unidades.clear();
	edificios.clear();
	vec_unidades.clear();
	unidadActiva = NULL;

	for (int i = (int)MADERA; i <= (int)ORO; i++)
		mapRecursosEconomicos[(TipoEntidad)i] = 0;
}

void Jugador::agregarUnidadSeleccionada(Unidad* unidad){
	unidadesSeleccionadas.push_back(unidad);
}

vector<Unidad*> Jugador::getUnidadesSeleccionadas(){
	return unidadesSeleccionadas;
}

void Jugador::agregarCentroCivico(CentroCivico* centro){
	centroCivico = centro;
}

void Jugador::agregarNuevaUnidad(Unidad* nuevaUnidad){
	contador_dni_unidades++;
	unidades[contador_dni_unidades] = nuevaUnidad;
	nuevaUnidad->set_id_jugador(id_jug);
	nuevaUnidad->set_identificador(contador_dni_unidades);
	if (unidadActiva == NULL){
		unidadActiva = nuevaUnidad;
	}
	vec_unidades.push_back(nuevaUnidad);
}

void Jugador::agregarRecursoEconomico(TipoEntidad tipo, int cant) {
	mapRecursosEconomicos[tipo] += cant;
}

std::map<TipoEntidad,int> Jugador::getMapRecursosEconomicos() {
	return mapRecursosEconomicos;
}

Unidad* Jugador::getUnidadActiva(){
	return unidadActiva;
}

CentroCivico* Jugador::getCentroCivico(){
	return centroCivico;
}

void Jugador::agregarNuevoEdificio(Edificio* edificio){
	contador_dni_edificios++;
	edificios[contador_dni_edificios] = edificio;
	edificio->set_id_jugador(id_jug);
	edificio->set_identificador(contador_dni_edificios);
}

vector<Unidad*> Jugador::getUnidades(){
	return vec_unidades;
}

void Jugador::interaccionesDeUnidades() {
	for (std::vector<Unidad*>::iterator uniIt = this->vec_unidades.begin(); uniIt < this->vec_unidades.end(); ++uniIt) {
		try {
			(*uniIt)->interactuar();
		} catch ( Recoleccion &r ) {
			agregarRecursoEconomico(r.tipo, r.cant);
		}
	}
}

Jugador::~Jugador() {
	//for (unsigned i = 0; i < vec_unidades.size(); i++){
	//	delete vec_unidades[i];
	//}
	mapRecursosEconomicos.clear();
	delete centroCivico;
}

