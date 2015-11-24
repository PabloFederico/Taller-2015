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
	edificioSeleccionado = NULL;

	for (int i = (int)MADERA; i <= (int)ORO; i++)
		mapRecursosEconomicos[(TipoEntidad)i] = 0;
}

int Jugador::getID(){
	return id_jug;
}

void Jugador::agregarEntidadSeleccionada(Entidad* entidad){
	if (entidad->esUnidad())
		unidadesSeleccionadas.push_back((Unidad*)entidad);
	else edificioSeleccionado = (Edificio*)entidad;
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

void Jugador::agregarNuevoEdificio(Edificio* edificio, int idJug = -1){
	if (idJug == -1)
		idJug = this->id_jug;
	contador_dni_edificios++;
	int id = contador_dni_edificios;
	edificios[id] = edificio;
	edificio->set_id_jugador(idJug);
	edificio->set_identificador(id);
}

Edificio* Jugador::terminarConstruccion(ConstruccionTermino c) {	/// SOLO LOCALES !!!
	Edificio *construc = edificios[c.dni];
	construc->morir();
	delete construc;
	edificios.erase(c.dni);

	Edificio *nuevoEdificio = new Edificio(c.tipoEdif, c.idJug, c.dni);
	nuevoEdificio->set_identificador(c.dni);
	nuevoEdificio->set_id_jugador(c.idJug);
	nuevoEdificio->setPosicion(Coordenada(c.x,c.y));
	nuevoEdificio->sufrirGolpe(100 - c.vidaRestante); // hardcodeo de vida inicial de construcción
	edificios[c.dni] = nuevoEdificio;
	return nuevoEdificio;
}

vector<Unidad*> Jugador::getUnidades(){
	return vec_unidades;
}

void Jugador::interaccionesDeUnidades(Escenario* escenario, ContenedorDeRecursos* contenedor, Coordenada coord_ceros) {
	for (std::vector<Unidad*>::iterator uniIt = this->vec_unidades.begin(); uniIt < this->vec_unidades.end(); ++uniIt) {
		try {
			(*uniIt)->interactuar();
		} catch ( Recoleccion &r ) {
			agregarRecursoEconomico(r.tipo, r.cant);
		} catch ( ConstruccionTermino &c ) {
			Entidad *muerto = edificios[c.dni];
			std::cout << muerto->enc()<<" construccion terminada"<<std::endl;//
			escenario->quitarEntidad(muerto->getPosicion(), muerto);
			contenedor->borrarSpriteDeEntidad(muerto);
			//Falta algo todo?

			Edificio* edif = terminarConstruccion(c);					/// SOLO LOCALES !!!
			contenedor->generarYGuardarSpriteEntidad(edif, coord_ceros, escenario);
			escenario->agregarEntidad(edif->getPosicion(), edif);
		}
	}
}

void Jugador::limpiarSeleccionDeUnidadMuerta(Unidad* moribundo) {
	for (vector<Unidad*>::iterator itAux = unidadesSeleccionadas.begin(); itAux < unidadesSeleccionadas.end(); ++itAux)
		if ((*itAux) == moribundo) {
			unidadesSeleccionadas.erase(itAux);
			break;
		}
	if (unidadActiva == moribundo)
		unidadActiva = NULL;
}

vector<Entidad*> Jugador::revisarMuertosPropios() {
	vector<Entidad*> cuerpos;
	for (std::vector<Unidad*>::iterator uniIt = this->vec_unidades.begin(); uniIt < this->vec_unidades.end(); ++uniIt) {
		if (!(*uniIt)->sigueViva()) {
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
	//for (unsigned i = 0; i < vec_unidades.size(); i++){
	//	delete vec_unidades[i];
	//}
	mapRecursosEconomicos.clear();
	delete centroCivico;
}

