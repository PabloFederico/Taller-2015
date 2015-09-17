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
	this->screenHeigth = 600; // Default
	this->mapEntidades = new std::map<TipoEntidad,Entidad*>();

	this->cargarJuego();
}

/********************************************************************************/
void Juego::cargarDatosEntidad(TipoEntidad tipo, Entidad *entidad){
	this->mapEntidades->insert(std::make_pair(tipo,entidad));
}

/********************************************************************************/
std::map<TipoEntidad,Entidad*>* Juego::getMapEntidades(){
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
	this->escenario = new Escenario(50,50);	// Default?

	Entidad *arbol = new Entidad(ARBOL);
	arbol->setPath("arbol.png");

	Suelo *tierra = new Suelo(TIERRA);
	tierra->setPath("tierra.png");

	Suelo *agua = new Suelo(AGUA);
	agua->setPath("agua.png");

	Castillo *castillo = new Castillo(CASTILLO);
	castillo->setPath("castle.png");

	Soldado *soldado = new Soldado(SOLDADO);
	soldado->setPath("soldier.png");

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
	return std::make_pair(this->screenWidth,this->screenHeigth);
}

/********************************************************************************/
Juego::~Juego() {
	std::map<TipoEntidad,Entidad*>::iterator p = this->mapEntidades->begin();
	while (p != this->mapEntidades->end()){		// Por qué no lo siguiente: for (p = this->mapEntidades->begin(); p != this->mapEntidades->end(); p++) \n delete (*p).second;
		Entidad* ente = (*p).second;
		p++;
		delete ente;
	}
	delete this->escenario;
}

