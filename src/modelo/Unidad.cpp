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
	//std::cout << "Interacción "<<clock()<<" "<<this->reloj<<": "<<clock()-reloj<<" "<<CLOCKS_PER_SEC*DELAY_INTERACCION<<" - "<<(clock()-reloj)/CLOCKS_PER_SEC<<std::endl;//

	this->reloj = clock();
	try {
		Coordenada posReceptor = receptor->getPosicion();
		pair<int,int> tamReceptor = receptor->getTam();
		for (int i = posReceptor.x; i < posReceptor.x+tamReceptor.first; i++)
			for (int j = posReceptor.y; j < posReceptor.y+tamReceptor.second; j++)
				// Distancia máxima hardcodeada de 1 tile; TODO rangoAtaque
				if (distanciaEuclidiana(this->getPosicion(), Coordenada(i,j)) < 2) {
					//std::cout << "Interacción de "<<getInfo()<<" "<<get_identificador()<<std::endl;//
					//if (receptor->esConstruccion()) { cambioEstado(CONSTRUYENDO); // TODO } else
					if (receptor->esAtacable()) {
						cambioEstado(ATACANDO);
						this->lastimar(this->receptor);
					} else if (receptor->esRecurso() && this->esRecolector()) {
						cambioEstado(RECOLECTANDO);
						int recolectado = 0;
						recolectado = recolectar(this->receptor);
						if (recolectado > 0) {
							throw Recoleccion(receptor->getTipo(), recolectado);
							// Ojo que si llega a este punto, no se correrá nada debajo
						}
					}
					return;
				}

	} catch ( EntidadMurio &e ) {
		this->olvidarInteraccion();
	}
}

int Unidad::generarGolpe() {
	// TODO: Fórmula mágica
	if (this->tipo == SOLDADO)
		return 3; // hardcodeado malll pero mallllll
	if (this->tipo == ALDEANO)
		return 1;
	return 0;
}

void Unidad::lastimar(Entidad* victima) {
	victima->sufrirGolpe(this->generarGolpe());
}

int Unidad::recolectar(Entidad* recurso) {
	return receptor->sufrirRecoleccion();
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
	// play sonido de muerte
}

Unidad::~Unidad() {
	// TODO Auto-generated destructor stub
}

