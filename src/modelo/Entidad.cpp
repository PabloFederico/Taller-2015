/*
 * Entidad.cpp
 *
 *  Created on: 2 de sept. de 2015
 *      Author: pgfederi
 */

#include "../modelo/Entidad.h"

Entidad::Entidad(TipoEntidad tipo, int num_jug): idJug(num_jug) {
	this->reloj = clock();
	this->receptor = NULL;
	this->estado = QUIETO;
	this->vidaRestante = 1;
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
			vidaRestante = 10;//100;
			movible = true;
			ocupador = true;
			info = "Soldado";
			break;
		case ALDEANO:
			vidaRestante = 5;//50;
			movible = true;
			ocupador = true;
			info = "Aldeano";
			break;
		case ARQUERO:
			vidaRestante = 70;
			movible = true;
			ocupador = true;
			info = "Arquero";
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
			vidaRestante = 10;
			movible = false;
			ocupador = true;
			info = "Arbol";
			break;
		case MADERA:
			vidaRestante = 150;
			movible = false;
			ocupador = true;
			info = "Madera";
			break;
		case COMIDA:
			vidaRestante = 15;//150;
			movible = false;
			ocupador = true;
			info = "Comida";
			break;
		case PIEDRA:
			vidaRestante = 150;
			movible = false;
			ocupador = true;
			info = "Piedra";
			break;
		case ORO:
			vidaRestante = 150;
			movible = false;
			ocupador = true;
			info = "Oro";
			break;
		default:
			vidaRestante = 1; // Así nunca mueren
			movible = false;
			ocupador = true;
			info = "";
			break;
	}
}

int Entidad::getIDJug(){
	return idJug;
}

int Entidad::getVidaRestante() {
	return this->vidaRestante;
}

void Entidad::setVidaRestante(int valor) {
	if (valor >= 0) this->vidaRestante = valor;
}

TipoEntidad Entidad::getTipo(){
	return this->tipo;
}

EstadoEntidad Entidad::getEstado() {
	return this->estado;
}

void Entidad::cambioEstado(EstadoEntidad est) {
	//this->estado = est; // Todos los EstadoEntidad son para Unidad nomás. Lo descarta.
}

void Entidad::finalizaAccion() {
	this->estado = QUIETO;
}

bool Entidad::esRecurso(){
	return EsRecurso(this->tipo);
}
bool Entidad::esConstruccion() {
	return (tipo == CONSTRUCCION);
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

bool Entidad::esAtacable() {
	return (esUnidad() || esEdificio() || tipo == ANIMAL || tipo == ARBOL); // medio harcodeo; reemplazable por una variable esAtacable
}

bool Entidad::esMovible(){
	return this->movible;
}

bool Entidad::ocupaSuTile() {
	return this->ocupador;
}

Coordenada Entidad::getPosicion(){
	return c;
}

void Entidad::setPosicion(Coordenada nuevaCoord){
	c = nuevaCoord;
}

void Entidad::interactuarCon(Entidad* receptor) {
	if (this->getIDJug() == receptor->getIDJug())
		return;	// No existe acción contra otra entidad propia (al menos por ahora).
	if (receptor->esAtacable() || receptor->esRecurso()) {
		this->receptor = receptor;
		this->reloj = clock();
		// SOLDADO, ALDEANO, ANIMAL; EDIFICIO, CENTRO_CIVICO, CUARTEL, CASTILLO // MADERA, COMIDA, PIEDRA, ORO
	}
	// ARBOL, DEFAULT
}

void Entidad::olvidarInteraccion() {
	finalizaAccion();
	this->receptor = NULL;
}


void Entidad::sufrirGolpe(int fuerzaGolpe) {
	if (!esAtacable()) return;
	if (this->vidaRestante <= 0)
		throw EntidadMurio();
	this->vidaRestante -= fuerzaGolpe;
	if (this->vidaRestante <= 0) {
		this->vidaRestante = 0;
		if (!esRecurso())
			throw EntidadMurio();
		else std::cout << "Esto sí es posible, de alguna forma"<<std::endl;//
	}
}

int Entidad::sufrirRecoleccion(int cant = 5) {
	if (!esRecurso()) return 0;
	int recolectado = cant;
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
	enc << info<<"; vida: "<<vidaRestante;	//Elegir un formato TODO
	return enc.str();
}

std::string Entidad::getVidaString() {
	ostringstream enc;
	enc << vidaRestante;
	return enc.str();
}

// Para comunicación de redes
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
//

bool Entidad::sigueViva() {
	return (this->vidaRestante > 0);
}

void Entidad::morir() {
	//
}

Entidad::~Entidad() {
	// TODO Auto-generated destructor stub
}

