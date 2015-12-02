/*
 * Entidad.cpp
 *
 *  Created on: 2 de sept. de 2015
 *      Author: pgfederi
 */

#include "../modelo/Entidad.h"

#include <math.h>

Entidad::Entidad(TipoEntidad tipo, int num_jug, int identificador): idJug(num_jug), dni(identificador) {
	this->reloj = clock();
	this->receptor = NULL;
	this->coordMasProximaDelReceptor = NULL;

	this->estado = QUIETO;
	this->vidaRestante = 1;
	this->armadura = 0;
	this->ataque = 0;
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
			vidaRestante = 30;
			movible = true;
			ocupador = true;
			info = "Soldado";
			armadura = 3;
			ataque = 10;
			break;
		case ALDEANO:
			vidaRestante = 20;
			movible = true;
			ocupador = true;
			info = "Aldeano";
			armadura = 5;
			ataque = 3;
			break;
		case ARQUERO:
			vidaRestante = 40;
			movible = true;
			ocupador = true;
			info = "Arquero";
			armadura = 2;
			ataque = 6;
			break;
		case ANIMAL:
			vidaRestante = 5;
			movible = true;
			ocupador = true;
			info = "Animal";
			break;
		case MINA_ORO:
			vidaRestante = 10;
			movible = false;
			ocupador = true;
			info = "Mina de Oro";
			break;
		case MINA_PIEDRA:
			vidaRestante = 10;
			movible = false;
			ocupador = true;
			info = "Mina de Piedra";
			break;
		case CASTILLO:
			vidaRestante = 100;
			movible = false;
			ocupador = true;
			info = "Castillo";
			break;
		case CUARTEL:
			vidaRestante = 80;
			movible = false;
			ocupador = true;
			info = "Cuartel";
			break;
		case BARRACK:
			vidaRestante = 80;
			movible = false;
			ocupador = true;
			info = "Barraca_Arquero";
			break;
		case CONSTRUCCION_BARRACK:
		case CONSTRUCCION_CUARTEL:
		case CONSTRUCCION:
			vidaRestante = 50;
			movible = false;
			ocupador = true;
			info = "Construccion";
			break;
		case CENTRO_CIVICO:
			vidaRestante = 80;
			movible = false;
			ocupador = true;
			info = "Centro civico";
			ancho = 4;
			alto = 4;
			break;
		case ARBOL:
			vidaRestante = 5;
			movible = false;
			ocupador = true;
			info = "Arbol";
			break;
		case MADERA:
			vidaRestante = 30;
			movible = false;
			ocupador = true;
			info = "Madera";
			break;
		case COMIDA:
			vidaRestante = 30;
			movible = false;
			ocupador = true;
			info = "Comida";
			break;
		case PIEDRA:
			vidaRestante = 30;
			movible = false;
			ocupador = true;
			info = "Piedra";
			break;
		case ORO:
			vidaRestante = 30;
			movible = false;
			ocupador = true;
			info = "Oro";
			break;
		case BANDERA:
			vidaRestante = 10;
			movible = false;
			ocupador = true;
			info = "BANDERA";
			break;
		case REY:
			vidaRestante = 77;
			movible = true;
			ocupador = true;
			info = "Rey de los perdedores";
			armadura = 7;
			ataque = 0;
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
	return this->idJug;
}

int Entidad::get_identificador() {
	return this->dni;
}

bool Entidad::perteneceAJugador(int id_jug) {
	return (id_jug == this->idJug);
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
	return (tipo == CONSTRUCCION
		 || tipo == CONSTRUCCION_BARRACK
		 || tipo == CONSTRUCCION_CUARTEL);
}

bool Entidad::esRecolector() {
	return (tipo == ALDEANO);
}
bool Entidad::esConstructor() {
	return (tipo == ALDEANO);
}

int Entidad::obtenerArmor(){
	return this->armadura;
}
int Entidad::obtenerAtk(){
	return this->ataque;
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
	return (esUnidad() || esEdificio() || tipo == ANIMAL || tipo == ARBOL || tipo == MINA_ORO || tipo == MINA_PIEDRA); // medio harcodeo; reemplazable por una variable esAtacable
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

float distEuclid(Coordenada a, Coordenada z) {
	return sqrt( pow(z.x-a.x,2) + pow(z.y-a.y,2) );
}

void Entidad::interactuarCon(Entidad* receptor) {
	if (this->getIDJug() == receptor->getIDJug() && !receptor->esConstruccion())
		return;	// No existe acción contra otra entidad propia salvo construcciones.
	if (receptor->esAtacable() || receptor->esRecurso() || receptor->esConstruccion()) {
		olvidarInteraccion(); //verificar que se llame a la de Unidad
		this->receptor = receptor;
		this->reloj = clock();

		// Cambios de estado		//No muy lindo pero tampoco tan feo
		if (this->esUnidad() && distEuclid(this->getPosicion(),receptor->getPosicion()) <= 1) {
			if (receptor->esConstruccion() && this->esConstructor() && receptor->perteneceAJugador(this->getIDJug())) {
				cambioEstado(CONSTRUYENDO);
			} else if (receptor->esAtacable() && !receptor->perteneceAJugador(this->getIDJug())) {
				cambioEstado(ATACANDO);
			} else if (receptor->esRecurso() && this->esRecolector()) {
				cambioEstado(RECOLECTANDO);
			}
		}
	}
	// ARBOL, DEFAULT
}

void Entidad::olvidarInteraccion() {
	finalizaAccion();
	this->receptor = NULL;
	delete this->coordMasProximaDelReceptor;
}


void Entidad::sufrirGolpe(int fuerzaGolpe) {
	if (!esAtacable()) return;
	if (this->vidaRestante <= 0){
		estado = MURIENDO;
		throw EntidadMurio();
	}
	this->vidaRestante -= fuerzaGolpe;
	if (this->vidaRestante <= 0) {
		this->vidaRestante = 0;
		if (!esRecurso()){
			estado = MURIENDO;
			throw EntidadMurio();
		}
		else std::cout << "Es posible atacar un recurso, corregime"<<std::endl;//
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
	if (idJug != 0)
		enc << info<<" (Jugador "<<idJug<<")"<<"; vida: "<<vidaRestante;
	else if (vidaRestante > 1)
		enc << info<<"; vida: "<<vidaRestante;
	else
		enc << info;
	return enc.str();
}

std::string Entidad::getVidaString() {
	ostringstream enc;
	enc << vidaRestante;
	return enc.str();
}

std::string Entidad::getIDJugYVidaString() {
	ostringstream enc;
	enc << "(Jug. "<<idJug<<") "<<vidaRestante;
	return enc.str();
}

///// Para comunicación de redes
// Encodeado: "idJug,tipo,coord.enc(),dni"
std::string Entidad::enc() {
	ostringstream enc;
	enc << idJug<<","<<tipo<<","<<c.enc()<<","<<dni;
	return enc.str();
}

Entidad Entidad::dec(std::string s) {
	char cs[12];
	int id,ti,dni;
	stringstream ss(s);
	ss >> id; ss.ignore();
	ss >> ti; ss.ignore();
	ss.get(cs, 11, ','); ss.ignore();
	ss >> dni;
	Entidad *u = new Entidad(TipoEntidad(ti), id, dni);
	u->setPosicion(Coordenada::dec(cs));
	return *u;
}
/////

bool Entidad::sigueViva() {
	return (this->vidaRestante > 0);
}

void Entidad::morir() {
	switch (tipo) {
	case CENTRO_CIVICO:
		throw FinJuego(DESTRUIR_CENTRO_CIVICO);
	case BANDERA:
		throw FinJuego(CAPTURAR_BANDERA);
	case REY:
		throw FinJuego(PARTIDA_REGICIDA);
	default: break;
	}
}

Entidad::~Entidad() {
	olvidarInteraccion();
}

