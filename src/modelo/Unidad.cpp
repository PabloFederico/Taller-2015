/*
 * Unidad.cpp
 *
 *  Created on: 9 de nov. de 2015
 *      Author: emanuel
 */

#include "Unidad.h"
#include <string>
#include <iostream>
#include <sstream>
#include <math.h>
#include <random>
using namespace std;


Unidad::Unidad(TipoEntidad tipo, int id_jug, int dni): Entidad(tipo,id_jug) {
	this->dni = dni;
	//petrificado = false;
	ostringstream ssInfo;
	ssInfo << info<<" (Jugador "<<id_jug<<")";
	info = ssInfo.str();

	this->rangoAccion = 1; // tiles de alcance, hardcodeado
	if (tipo == ARQUERO)
		this->rangoAccion = 3; // hardcodeado
}


float distanciaEuclidiana(Coordenada a, Coordenada z) {
	return sqrt( pow(z.x-a.x,2) + pow(z.y-a.y,2) );
}

// Buscar posición más cercana del receptor, si no fue buscada antes.
void Unidad::setCoordMasProximaDelReceptor() {
	Coordenada posReceptor = receptor->getPosicion();
	pair<int,int> tamReceptor = receptor->getTam();

	float distMinima = 10000;
	for (int i = posReceptor.x; i < posReceptor.x+tamReceptor.first; i++)
		for (int j = posReceptor.y; j < posReceptor.y+tamReceptor.second; j++) {
			float distAux = distanciaEuclidiana(this->getPosicion(), Coordenada(i,j));
			if (distAux < distMinima) {
				delete coordMasProximaDelReceptor;
				coordMasProximaDelReceptor = new Coordenada(i,j);
				distMinima = distAux;
			}
		}
}

bool Unidad::estaEnRangoDelReceptor() {
	return (distanciaEuclidiana(this->getPosicion(), *coordMasProximaDelReceptor) < (this->rangoAccion + 1));
}

// throws Recoleccion, ConstruccionTermino, UnidadDebeAcercarse
void Unidad::interactuar() {
	if (receptor == NULL)
		return;
	if (coordMasProximaDelReceptor == NULL) {
		setCoordMasProximaDelReceptor();

	} else if (estaEnRangoDelReceptor()) {
		if ((clock() - this->reloj) < CLOCKS_PER_SEC*DELAY_INTERACCION) return;	//No está respetando el tiempo pedido. TODO

		this->reloj = clock();
		try {
			if (receptor->esConstruccion() && this->esConstructor() && receptor->perteneceAJugador(this->idJug)) {
				cambioEstado(CONSTRUYENDO);
				this->continuarConstruccion();	// throws ConstruccionTermino
			} else if (receptor->esAtacable() && !receptor->perteneceAJugador(this->idJug)) {
				cambioEstado(ATACANDO);
				this->lastimar(this->receptor);
			} else if (receptor->esRecurso() && this->esRecolector()) {
				cambioEstado(RECOLECTANDO);
				int recolectado = 0;
				TipoEntidad tipoRecurso = this->receptor->getTipo();
				recolectado = recolectar(this->receptor);
				if (recolectado > 0) {
					throw Recoleccion(tipoRecurso, recolectado);
				}
			}
			return;

		} catch ( EntidadMurio &e ) {
			this->olvidarInteraccion();
		} catch ( ConstruccionTermino &e ) {
			this->olvidarInteraccion();
			throw e;
		}
	} else {
		throw UnidadDebeAcercarse(coordMasProximaDelReceptor->x, coordMasProximaDelReceptor->y);
	}
}

void Unidad::olvidarInteraccion() {
	finalizaAccion();
	this->receptor = NULL;
	this->coordMasProximaDelReceptor = NULL;
}

int Unidad::generarGolpe() {
	random_device rd_gen;
	mt19937 gen(rd_gen());
	uniform_int_distribution<int> distribucion(this->obtenerAtk()/2,this->obtenerAtk());
	int num_rndm = distribucion(gen);
	int dmg = num_rndm -(receptor->obtenerArmor()*0.25);
	if (dmg < 0) return 0; //por ahi el ataque es muy poronga con respecto a la armor de un edificio, ponele
	return dmg;
}

void Unidad::continuarConstruccion() {
	((Construccion*)this->receptor)->continuarConstruyendo();
}

void Unidad::lastimar(Entidad* victima) {
	victima->sufrirGolpe(this->generarGolpe());
}

int Unidad::recolectar(Entidad* recurso) {
	int cantidad = 5; // hardcodeado
	bool acabeRecurso = (this->receptor->getVidaRestante() <= cantidad);
	int recolectado = receptor->sufrirRecoleccion(cantidad);
	if (acabeRecurso)
		this->olvidarInteraccion();
	return recolectado;
}


void Unidad::olvidarReceptorSiEs(Entidad* entidad) {
	if (this->receptor == entidad)
		olvidarInteraccion();
}


void Unidad::set_id_jugador(int nuevoDuenio){
	this->idJug = nuevoDuenio;
}

int Unidad::get_identificador(){
	return dni;
}

void Unidad::set_identificador(int nuevoDNI){
	dni = nuevoDNI;
}

int Unidad::getRangoAccion() {
	return this->rangoAccion;
}

bool Unidad::esUnidad() {
	return true;
}

bool Unidad::esRecolector() {
	return (tipo == ALDEANO);
}

bool Unidad::esConstructor() {
	return (tipo == ALDEANO);
}

//bool Unidad::estaPetrificado(){
//	return petrificado;
//}

//void Unidad::petrificar(){
//	petrificado = true;
//}

//void Unidad::despetrificar(){
//	petrificado = false;
//}

void Unidad::cambioEstado(EstadoEntidad est) {
	this->estado = est;
}

void Unidad::morir() {
	Entidad::morir();
}

Unidad::~Unidad() {
	// TODO Auto-generated destructor stub
}

