/*
 * Juego.h
 *
 *  Created on: 6 de sept. de 2015
 *      Author: emanuel
 */

#ifndef JUEGO_H_
#define JUEGO_H_
#include "Exceptions.h"
#include "Entidad.h"
#include "VistaEntidad.h"
#include "Suelo.h"
#include "Escenario.h"
#include <map>
#include <yaml-cpp/yaml.h>


struct DatoPosicion {	/* Sin verificación de datos */
	int x, y;
	std::string tipo;

	DatoPosicion(): x(0), y(0), tipo("") {
		//if (this->tipo == "") throw bad_nombreTipoEntidad(this->tipo);
	}
	DatoPosicion(YAML::Node d):
		x(d["x"].as<int>()), y(d["y"].as<int>()), tipo(d["tipo"].as<std::string>()) {};
};

struct DatosEscenario {		/* Sin verificación de datos */
	std::string nombre;
	int size_x, size_y;
	std::vector<DatoPosicion> entidades;
	DatoPosicion protagonista;	/** Único protagonista; adaptable. */

	DatosEscenario(YAML::Node unEscenario) {
		this->nombre = unEscenario["nombre"].as<std::string>();
		this->size_x = unEscenario["size_x"].as<int>();
		this->size_y = unEscenario["size_y"].as<int>();
		for(std::size_t i = 0; i < unEscenario["entidades"].size(); ++i) {
			DatoPosicion dp(unEscenario["entidades"][i]);
			this->entidades.push_back(dp);
		}
		DatoPosicion protag(unEscenario["protagonista"][0]);
		this->protagonista = protag;
	};
};


class Juego {
private:
	int screenWidth;
	int screenHeight;
	std::map<std::string,VistaEntidad*> *mapEntidades;
	Escenario *escenario;
	Entidad *protagonista;

	/* Carga la configuración inicial del juego a traves
	 * de un archivo de configuración */
	void cargarJuego();

	void cargarEscenario(std::vector<DatosEscenario> vecEscenarios);

	/* Carga en un map los tipos de entidades posibles del juego
	 * (TIERRA , AGUA , ARBOL , CASTILLO, SOLDADO , etc)*/
	void cargarConfigEntidad(std::string,VistaEntidad* vEntidad);

	std::vector<DatosEscenario> parsearConfig(YAML::Node config);

public:
	Juego();

	std::pair<int,int> dimensionVentana();

	Escenario* getEscenario();

	std::map<std::string,VistaEntidad*>* getMapEntidades();

	Entidad* getProtagonista();

	virtual ~Juego();
};

#endif /* JUEGO_H_ */
