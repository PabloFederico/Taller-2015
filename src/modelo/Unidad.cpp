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
	petrificado = false;
	ostringstream ssInfo;
	ssInfo << info<<" (Jugador "<<id_jug<<")";
	info = ssInfo.str();
}



float distanciaEuclidiana(Coordenada a, Coordenada z) {
	return sqrt( pow(z.x-a.x,2)+pow(z.y-a.y,2) );
}

// throws Recoleccion cuando se recolectó algún recurso.
void Unidad::interactuar() {
	// Verifica que haya con quien interactuar y de que haya pasado el tiempo requerido.
	if (receptor == NULL || (clock() - this->reloj) < CLOCKS_PER_SEC*DELAY_INTERACCION) return;	//No está respetando el tiempo pedido. TODO

	this->reloj = clock();
	try {
		Coordenada posReceptor = receptor->getPosicion();
		pair<int,int> tamReceptor = receptor->getTam();
		for (int i = posReceptor.x; i < posReceptor.x+tamReceptor.first; i++)
			for (int j = posReceptor.y; j < posReceptor.y+tamReceptor.second; j++)
				// Distancia máxima hardcodeada de 1 tile; TODO rangoAtaque
				if (distanciaEuclidiana(this->getPosicion(), Coordenada(i,j)) < 2) {
					if (receptor->esConstruccion() && this->esConstructor()) {
						cambioEstado(CONSTRUYENDO);
						this->continuarConstruccion();	// throws ConstruccionTermino
						// Ojo que si llega a este punto, puede que no se corra nada debajo
					} else if (receptor->esAtacable()) {
						cambioEstado(ATACANDO);
						this->lastimar(this->receptor);
					} else if (receptor->esRecurso() && this->esRecolector()) {
						cambioEstado(RECOLECTANDO);
						int recolectado = 0;
						TipoEntidad tipoRecurso = this->receptor->getTipo();
						recolectado = recolectar(this->receptor);
						if (recolectado > 0) {
							throw Recoleccion(tipoRecurso, recolectado);
							// Ojo que si llega a este punto, no se correrá nada debajo
						}
					}
					return;
				}

	} catch ( EntidadMurio &e ) {
		this->olvidarInteraccion();
	} catch ( ConstruccionTermino &e ) {
		this->olvidarInteraccion();
		throw e;
	}
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


void Unidad::set_id_jugador(int nuevoDuenio){
	idJug = nuevoDuenio;
}

int Unidad::get_identificador(){
	return dni;
}

void Unidad::set_identificador(int nuevoDNI){
	dni = nuevoDNI;
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

bool Unidad::estaPetrificado(){
	return petrificado;
}

void Unidad::petrificar(){
	petrificado = true;
}

void Unidad::despetrificar(){
	petrificado = false;
}

void Unidad::cambioEstado(EstadoEntidad est) {
	this->estado = est;
}


// Para red
std::string Unidad::enc(){
	ostringstream enc;
	enc << idJug<<","<<dni<<","<<tipo<<","<<ancho<<","<<alto;
	return enc.str();
}
Unidad* Unidad::dec(std::string s){
	int id,dni,ti,an,al;
	stringstream ss(s);
	ss >> id; ss.ignore();
	ss >> dni;ss.ignore();
	ss >> ti; ss.ignore();
	ss >> an; ss.ignore();
	ss >> al;
	Unidad *u = new Unidad(TipoEntidad(ti),id,dni);
	u->setTam(an, al);
	return u;
}
//

void Unidad::morir() {
	Entidad::morir();
}

Unidad::~Unidad() {
	// TODO Auto-generated destructor stub
}

