/*
 * Unidad.cpp
 *
 *  Created on: 9 de nov. de 2015
 *      Author: emanuel
 */

#include "Unidad.h"
#include <string>
#include <sstream>
#include <math.h>
using namespace std;


Unidad::Unidad(TipoEntidad tipo, int id_jug): Entidad(tipo,id_jug) {
	this->dni = 0;
	petrificado = false;
	ostringstream ssInfo;
	ssInfo << info<<" (Jugador "<<id_jug<<")";
	info = ssInfo.str();
}



float distanciaEuclidiana(Coordenada a, Coordenada z) {
	return sqrt( pow(z.x-a.x,2) + pow(z.y-a.y,2) );
}

void Unidad::interactuar() {
	// Verifica que haya con quien interactuar y de que haya pasado el tiempo requerido.
	if (receptor == NULL || (clock() - this->reloj) < CLOCKS_PER_SEC*DELAY_INTERACCION) return;

	this->reloj = clock();
	try {
		Coordenada posReceptor = receptor->getPosicion();
		pair<int,int> tamReceptor = receptor->getTam();
		for (int i = posReceptor.x; i < posReceptor.x+tamReceptor.first; i++)
			for (int j = posReceptor.y; j < posReceptor.y+tamReceptor.second; j++)
				// Distancia máxima hardcodeada de 1 tile; TODO rangoAtaque
				if (distanciaEuclidiana(this->getPosicion(), Coordenada(i,j)) < 2) {

					//if (receptor->esConstruccion()) { cambioEstado(CONSTRUYENDO); // TODO } else
					if (receptor->esEdificio() || receptor->esUnidad()) {
						cambioEstado(ATACANDO);
						this->lastimar(this->receptor);
					} else if (receptor->esRecurso()) {
						cambioEstado(RECOLECTANDO);
						int recolectado = 0;
						recolectado = receptor->sufrirRecoleccion();
						if (recolectado > 0)
							throw Recoleccion(receptor->getTipo(), recolectado);
							// Ojo que si llega a este punto, no se correrá nada debajo
					}
					return;
				}

	} catch ( EntidadMurio &e ) {
		olvidarInteraccion();
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


void Unidad::set_id_jugador(int nuevoDuenio){
	idJug = nuevoDuenio;
}

int Unidad::get_identificador(){
	return dni;
}

void Unidad::set_identificador(int nuevoDNI){
	dni = nuevoDNI;
}

bool Unidad::esUnidad(){
	return true;
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


std::string Unidad::enc(){
	ostringstream enc;
	enc << idJug<<","<<tipo<<","<<ancho<<","<<alto;
	return enc.str();
}

Unidad* Unidad::dec(std::string s){
	int id,ti,an,al;
	stringstream ss(s);
	ss >> id; ss.ignore();
	ss >> ti; ss.ignore();
	ss >> an; ss.ignore();
	ss >> al;
	Unidad *u = new Unidad(TipoEntidad(ti),id);
	u->setTam(an, al);
	return u;
}

Unidad::~Unidad() {
	// TODO Auto-generated destructor stub
}

