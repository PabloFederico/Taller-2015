/*
 * Entidad.cpp
 *
 *  Created on: 2 de sept. de 2015
 *      Author: pgfederi
 */

#include "../modelo/Entidad.h"

Entidad::Entidad(TipoEntidad tipo, int num_jug): idJug(num_jug)  {
	this->reloj = clock();
	this->receptor = NULL;
	this->vidaRestante = 0;
	this->ancho = 1;
	this->alto = 1;
	this->tipo = tipo;
	switch (tipo) {
		case PASTO:
			movible = false;
			ocupador = false;
			info = "";
			break;
		case TIERRA:
			movible = false;
			ocupador = false;
			info = "";
			break;
		case AGUA:
			movible = false;
			ocupador = true;
			info = "Agua";
			break;
		case SOLDADO:
			vidaRestante = 100;
			movible = true;
			ocupador = true;
			info = "Soldado";
			break;
		case ALDEANO:
			vidaRestante = 50;
			movible = true;
			ocupador = true;
			info = "Aldeano";
			break;
		case ANIMAL:
			vidaRestante = 30;
			movible = true;
			ocupador = true;
			info = "Animal";
			break;
		case CASTILLO:
			vidaRestante = 300;
			movible = false;
			ocupador = true;
			info = "Castillo";
			break;
		case ARBOL:
			movible = false;
			ocupador = true;
			info = "Arbol";
			break;
		case ORO:
			vidaRestante = 150;
			movible = false;
			ocupador = false;
			info = "Oro";
			break;
		case COMIDA:
			vidaRestante = 150;
			movible = false;
			ocupador = false;
			info = "Comida";
			break;
		case MADERA:
			vidaRestante = 150;
			movible = false;
			ocupador = false;
			info = "Madera";
			break;
		case PIEDRA:
			vidaRestante = 150;
			movible = false;
			ocupador = false;
			info = "Piedra";
			break;
		default:
			movible = false;
			ocupador = true;
			info = "";
			break;
	}
	x = 0;
	y = 0;
}

int Entidad::getIDJug(){
	return idJug;
}

TipoEntidad Entidad::getTipo(){
	return this->tipo;
}

bool Entidad::esRecurso(){
	return EsRecurso(this->tipo);
}
bool Entidad::esEdificio(){
	return EsEdificio(this->tipo);
}
bool Entidad::esUnidad(){
	return EsUnidad(this->tipo);
}

void Entidad::setTam(int ancho, int alto) {
	this->ancho = ancho;
	this->alto  = alto;
}

pair<int,int> Entidad::getTam() {
	return std::pair<int,int>(this->ancho, this->alto);
}

bool Entidad::esMovible(){
	return this->movible;
}

bool Entidad::ocupaSuTile() {
	return this->ocupador;
}

Coordenada Entidad::getPosicion(){
	return Coordenada(x,y);
}

void Entidad::setPosicion(Coordenada nuevaCoord){
	x = nuevaCoord.x;
	y = nuevaCoord.y;	// Por qué no guardarla como Coordenada y listo? (MC)
}

void Entidad::interactuarCon(Entidad* receptor) {
	if (this->getIDJug() == receptor->getIDJug())
		return;	// No existe acción contra otra entidad propia (por ahora).
	if (receptor->esEdificio() || receptor->esUnidad() || receptor->esRecurso()) {
		this->receptor = receptor;
		this->reloj = clock();
		// SOLDADO, ALDEANO, ANIMAL // EDIFICIO, CENTRO_CIVICO, CUARTEL, CASTILLO // MADERA, COMIDA, ORO, PIEDRA
	}
	// ARBOL, DEFAULT
}

void Entidad::olvidarInteraccion() {
	this->receptor = NULL;
}

void Entidad::sufrirGolpe(int fuerzaGolpe) {
	if (this->vidaRestante <= 0)
		throw EntidadMurio();
	this->vidaRestante -= fuerzaGolpe;
	if (this->vidaRestante <= 0 && !esRecurso())
		throw EntidadMurio();
}

int Entidad::sufrirRecoleccion() {
	int recolectado = 5;	// hardcodeado
	if (this->vidaRestante <= 0)
		throw EntidadMurio();
	this->vidaRestante -= recolectado;
	if (this->vidaRestante < 0) {
		recolectado += this->vidaRestante;
		this->vidaRestante = 0;
	}
	return recolectado;
}

//void Entidad::interactuar() {
	// TODO? Acción por default a con un receptor para unidades que no sean Edificio o Unidad?
//}


std::string Entidad::getInfo(){
	ostringstream enc;
	enc << info<<", vida: "<<vidaRestante;	//Elegir un formato
	return enc.str();
}


std::string Entidad::enc() {
	ostringstream enc;
	enc << idJug<<","<<tipo<<","<<ancho<<","<<alto;
	return enc.str();
}

Entidad* Entidad::dec(std::string s) {
	int id,ti,an,al;
	stringstream ss(s);
	ss >> id; ss.ignore();
	ss >> ti; ss.ignore();
	ss >> an; ss.ignore();
	ss >> al;
	Entidad *e = new Entidad(TipoEntidad(ti), id);
	e->setTam(an, al);
	return e;
}

Entidad::~Entidad() {
	// TODO Auto-generated destructor stub
}

