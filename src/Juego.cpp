/*
 * Juego.cpp
 *
 *  Created on: 6 de sept. de 2015
 *      Author: emanuel
 */

#include "Juego.h"
#include "Soldado.h"
#include "Castillo.h"
#include <stdio.h>

Juego::Juego() {
	this->escenario = NULL;
	this->protagonista = NULL;
	this->screenWidth = 800; // Default
	this->screenHeight = 600; // Default
	this->mapEntidades = new std::map<TipoEntidad,Entidad*>();
	this->cargarJuego();
}

/********************************************************************************/
void Juego::cargarDatosEntidad(TipoEntidad tipo, Entidad *entidad){
	this->mapEntidades->insert(std::make_pair(tipo,entidad));
}

/********************************************************************************/
std::map<TipoEntidad,Entidad*>* Juego::getEntidades(){
	return this->mapEntidades;
}

/********************************************************************************/
Escenario* Juego::getEscenario(){
	return this->escenario;
}

/********************************************************************************/
Entidad* Juego::getProtagonista(){
	return this->protagonista;
}

/********************************************************************************/
void Juego::cargarJuego(){
	/* Cargar estructura de YAML */

	/* modificar los tamanios de las ventanas segun YAML */

	/* Cargar la info de las entidades que se pueden usar */

	/* En la parte de escenario definimos las dimensiones del escenario
	 * y ubicamos las entidades en su respectiva posicion y la del protagonista
	 * */
	this->escenario = new Escenario(30,30);

	Entidad *arbol = new Entidad(ARBOL);
	arbol->setPath("images/arbol.png");

	Entidad *tierra = new Entidad(TIERRA);
	tierra->setPath("images/tierra.png");

	Entidad *agua = new Entidad(AGUA);
	agua->setPath("images/agua.png");

	Castillo *castillo = new Castillo(CASTILLO);
	castillo->setPath("images/castle.png");

	Soldado *soldado = new Soldado(SOLDADO);
	soldado->setPath("images/man.png");

	this->cargarDatosEntidad(ARBOL,arbol);
	this->cargarDatosEntidad(TIERRA,tierra);
	this->cargarDatosEntidad(SOLDADO,soldado);
	this->cargarDatosEntidad(AGUA,agua);
	this->cargarDatosEntidad(CASTILLO,castillo);

	this->escenario->agregarEntidad(std::make_pair(10,0),agua);
	this->escenario->agregarEntidad(std::make_pair(11,0),agua);

	this->escenario->agregarEntidad(std::make_pair(0,10),tierra);
	this->escenario->agregarEntidad(std::make_pair(0,11),tierra);
	this->escenario->agregarEntidad(std::make_pair(0,12),tierra);

	this->escenario->agregarEntidad(std::make_pair(4,5),arbol);
	this->escenario->agregarEntidad(std::make_pair(4,6),arbol);
	this->escenario->agregarEntidad(std::make_pair(5,5),arbol);
	this->escenario->agregarEntidad(std::make_pair(0,12),arbol);

	this->escenario->agregarEntidad(std::make_pair(8,8),castillo);

	this->escenario->agregarEntidad(std::make_pair(3,3),soldado);

	this->protagonista = soldado;
}

/********************************************************************************/
std::pair<int,int> Juego::dimensionVentana(){
	return std::make_pair(this->screenWidth,this->screenHeight);
}

/********************************************************************************/
Juego::~Juego() {
	std::map<TipoEntidad,Entidad*>::iterator p = this->mapEntidades->begin();
	while (p != this->mapEntidades->end()){
		Entidad* ente = (*p).second;
		p++;
		delete ente;
	}
	delete this->escenario;
}

