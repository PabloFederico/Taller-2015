/*
 * Juego.h
 *
 *  Created on: 6 de sept. de 2015
 *      Author: emanuel
 */

#ifndef JUEGO_H_
#define JUEGO_H_
#include "Entidad.h"
#include "Suelo.h"
#include "Escenario.h"
#include <map>

class Juego {
private:
	int screenWidth;
	int screenHeight;
	std::map<TipoEntidad,Entidad*> *mapEntidades;
	Escenario *escenario;
	Entidad *protagonista;

	/* Carga la configuración inicial del juego a traves
	 * de un archivo de configuración */
	void cargarJuego();

	/* Carga en un map los tipos de entidades posibles del juego
	 * (TIERRA , AGUA , ARBOL , CASTILLO, SOLDADO , etc)*/
	void cargarDatosEntidad(TipoEntidad,Entidad* entidad);

public:
	Juego();

	std::pair<int,int> dimensionVentana();

	Escenario* getEscenario();

	std::map<TipoEntidad,Entidad*>* getMapEntidades();

	Entidad* getProtagonista();

	virtual ~Juego();
};

#endif /* JUEGO_H_ */
