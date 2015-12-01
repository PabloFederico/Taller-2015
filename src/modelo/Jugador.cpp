/*
 * Jugador.cpp
 *
 *  Created on: 9 de nov. de 2015
 *      Author: emanuel
 */

#include "Jugador.h"
#include "../vista/SpriteUnidad.h"

Jugador::Jugador(std::string nombre, int id) {
	this->nombre = nombre;
	id_jug = id;
	contador_dni_edificios = 0;
	contador_dni_unidades = 0;
	unidades.clear();
	edificios.clear();
	vec_unidades.clear();
	unidadActiva = NULL;
	edificioSeleccionado = NULL;

	for (int i = (int)MADERA; i <= (int)ORO; i++)
		mapRecursosEconomicos[(TipoEntidad)i] = 50;	// Monto inicial.
}

/********************************************************************************/

int Jugador::getID(){
	return id_jug;
}

int Jugador::getUltimoDNIdeUnidadAsignado() {
	return this->contador_dni_unidades;
}

void Jugador::agregarEntidadSeleccionada(Entidad* entidad){
	if (entidad->esUnidad())
		unidadesSeleccionadas.push_back((Unidad*)entidad);
	else if (entidad->esEdificio())
		edificioSeleccionado = (Edificio*)entidad;
	//else el resto se ignoran majestuosamente
}

// Devuelve entidad propia de dni identificador, o NULL en caso de no encontrarla.
Entidad* Jugador::getEntidad(TipoEntidad tipo, int identificador) {
	if (EsUnidad(tipo)) {
		std::map<int,Unidad* >::iterator it = unidades.find(identificador);
		if (it != unidades.end())
			return (*it).second;
	} else if (EsEdificio(tipo)) {
		std::map<int,Edificio*>::iterator it = edificios.find(identificador);
		if (it != edificios.end())
			return (*it).second;
	}
	return NULL;
}

/*
void Jugador::agregarUnidadSeleccionada(Unidad* unidad){
	unidadesSeleccionadas.push_back(unidad);
}
*/
vector<Unidad*> Jugador::getUnidadesSeleccionadas(){
	return unidadesSeleccionadas;
}

Edificio* Jugador::getEdificioSeleccionado(){
	return edificioSeleccionado;
}

void Jugador::liberarUnidadesSeleccionadas(){
	unidadesSeleccionadas.clear();
}

void Jugador::liberarEdificioSeleccionado(){
	edificioSeleccionado = NULL;
}

vector<Unidad*> Jugador::getUnidades(){
	return vec_unidades;
}

vector<Edificio*> Jugador::getEdificios() {
	vector<Edificio*> vec_edificios;
	for (map<int,Edificio*>::iterator it = this->edificios.begin(); it != this->edificios.end(); ++it)
		vec_edificios.push_back(it->second);
	return vec_edificios;
}

std::map<TipoEntidad,int> Jugador::getMapRecursosEconomicos() {
	return mapRecursosEconomicos;
}

Unidad* Jugador::getUnidadActiva(){
	return unidadActiva;
}

/********************************************************************************/
/***				Jugador solo concierne al jugador local					  ***/

void Jugador::agregarNuevaUnidad(Unidad* nuevaUnidad){
	// El caso más común es que todavía esté en 0, pero esto abarca más casos.
	if (nuevaUnidad->get_identificador() <= contador_dni_unidades)
		contador_dni_unidades++;
	else
		contador_dni_unidades = nuevaUnidad->get_identificador();
	int id = contador_dni_unidades;

	unidades[id] = nuevaUnidad;
	nuevaUnidad->set_id_jugador(id_jug);
	nuevaUnidad->set_identificador(id);
	if (unidadActiva == NULL){
		unidadActiva = nuevaUnidad;		// discutible
	}
	vec_unidades.push_back(nuevaUnidad);
}

void Jugador::agregarNuevoEdificio(Edificio* edificio) {
	// El caso más común es que todavía esté en 0, pero esto abarca más casos.
	if (edificio->get_identificador() <= contador_dni_edificios)
		contador_dni_edificios++;
	else contador_dni_edificios = edificio->get_identificador();
	int id = contador_dni_edificios;

	edificios[id] = edificio;
	edificio->set_id_jugador(this->id_jug);
	edificio->set_identificador(id);
}

void Jugador::guardarConstruccionTerminada(Edificio* edifRecienRecienInaugurado) {
	int dni = edifRecienRecienInaugurado->get_identificador();
	this->edificios.erase(dni);
	this->edificios[dni] = edifRecienRecienInaugurado;
}

void Jugador::agregarRecursoEconomico(TipoEntidad tipo, int cant) {
	mapRecursosEconomicos[tipo] += cant;
}

bool Jugador::tieneRecursosParaCrearUnidad(TipoEntidad tipoUnidad){
	bool hayRecursos = false;
	switch (tipoUnidad){
	case ALDEANO :
				  if (mapRecursosEconomicos[MADERA] >= COSTO_CREAR_ALDEANO_MADERA &&
					  mapRecursosEconomicos[PIEDRA] >= COSTO_CREAR_ALDEANO_PIEDRA)
					  hayRecursos = true;
				  break;
	case ARQUERO:
				  if (mapRecursosEconomicos[MADERA] >= COSTO_CREAR_ARQUERO_MADERA &&
					  mapRecursosEconomicos[COMIDA] >= COSTO_CREAR_ARQUERO_COMIDA)
					  hayRecursos = true;
				  break;
	case SOLDADO:
				  if (mapRecursosEconomicos[MADERA] >= COSTO_CREAR_SOLDADO_MADERA &&
					  mapRecursosEconomicos[ORO] >= COSTO_CREAR_SOLDADO_ORO)
					  hayRecursos = true;
				  break;
	default : break;
	}
	return hayRecursos;
}

void Jugador::descontarRecursosPorCrearUnidad(TipoEntidad tipo){
	switch (tipo){
		case ALDEANO :
					  mapRecursosEconomicos[MADERA] -= COSTO_CREAR_ALDEANO_MADERA;
					  mapRecursosEconomicos[PIEDRA] -= COSTO_CREAR_ALDEANO_PIEDRA;
					  break;
		case ARQUERO:
					  mapRecursosEconomicos[MADERA] -= COSTO_CREAR_ARQUERO_MADERA;
					  mapRecursosEconomicos[COMIDA] -= COSTO_CREAR_ARQUERO_COMIDA;
					  break;
		case SOLDADO:
					  mapRecursosEconomicos[MADERA] -= COSTO_CREAR_SOLDADO_MADERA;
					  mapRecursosEconomicos[ORO] -= COSTO_CREAR_SOLDADO_ORO;
					  break;
		default : break;
	}
}

/********************************************************************************/

void Jugador::limpiarSeleccionDeUnidadMuerta(Unidad* moribundo) {
	for (vector<Unidad*>::iterator itAux = unidadesSeleccionadas.begin(); itAux < unidadesSeleccionadas.end(); ++itAux)
		if ((*itAux) == moribundo) {
			unidadesSeleccionadas.erase(itAux);
			break;
		}
	if (unidadActiva == moribundo)
		unidadActiva = NULL;
}

void Jugador::ejecutoresOlvidarInteraccionCon(Entidad* muerto) {
	for (map<int,Unidad*>::iterator it = unidades.begin(); it != unidades.end(); ++it)
		(*it).second->olvidarReceptorSiEs(muerto);
}

vector<Entidad*> Jugador::revisarMuertosPropios() {
	vector<Entidad*> cuerpos;
	for (std::vector<Unidad*>::iterator uniIt = this->vec_unidades.begin(); uniIt < this->vec_unidades.end(); ++uniIt) {
		//if (!(*uniIt)->sigueViva()) {
		if ((*uniIt)->getEstado() == MUERTO){
			Unidad* moribundo = *uniIt;

			unidades.erase(moribundo->get_identificador());
			vec_unidades.erase(uniIt);
			uniIt = this->vec_unidades.begin(); //por las dudas

			cuerpos.push_back(moribundo);
		}
	}
	for (std::map<int,Edificio*>::iterator ediIt = this->edificios.begin(); ediIt != this->edificios.end(); ++ediIt) {
		if (!ediIt->second->sigueViva()) {
			Edificio* dilapidado = ediIt->second;

			edificios.erase(ediIt);
			ediIt = this->edificios.begin(); //por las dudas

			cuerpos.push_back(dilapidado);
		}
	}
	return cuerpos;
}

Jugador::~Jugador() {
	this->unidades.clear();
	this->edificios.clear();
	this->vec_unidades.clear();
	this->mapRecursosEconomicos.clear();
}

