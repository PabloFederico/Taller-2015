/*
 * Juego.h
 *
 *  Created on: 6 de sept. de 2015
 *      Author: emanuel
 */

#ifndef JUEGO_H_
#define JUEGO_H_
#include <stdio.h>
#include <ctime>
#include "Exceptions.h"
#include "Entidad.h"
#include "VistaEntidad.h"
#include "ConfigDefault.h"
#include "Castillo.h"
#include "Escenario.h"
#include "Map.h"
#include <map>
#include <yaml-cpp/yaml.h>
#include <unistd.h>

/*
struct DatoPosicion {	// Sin verificación de datos
	int x, y;
	std::string tipo;

	DatoPosicion(): x(0), y(0), tipo("") {
		//if (this->tipo == "") throw bad_nombreTipoEntidad(this->tipo);
	}
	DatoPosicion(YAML::Node d):
		x(d["x"].as<int>()), y(d["y"].as<int>()), tipo(d["tipo"].as<std::string>()) {};
};

struct DatosEscenario {		// Sin verificación de datos
	std::string nombre;
	int size_x, size_y;
	std::vector<DatoPosicion> entidades;
	DatoPosicion protagonista;	//// Único protagonista; adaptable. /

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
*/

class Juego {
private:
	int screenWidth;
	int screenHeight;
	
	vector<InfoEntidad> vectorInfoTiposEntidades;
	
	Escenario *escenario;
	Entidad *protagonista;

	int vel_personaje;
	int margen_scroll;

	/* Carga la configuración inicial del juego a traves
	 * de un archivo de configuración */
	void cargarJuego();


	int ChequeoDeBorde(int max, int input);

	InfoEscenario infoEscenarioDefault();

	InfoEscenario parsearConfig();

	//de prueba		!!!
	InfoEscenario OdioYAML();

public:
	Juego();
	int getVelocidad();
	pair<int,int> dimensionVentana();

	Escenario* getEscenario();

	vector<InfoEntidad> getInfoTiposEntidades();
	//map<std::string,VistaEntidad*>* getMapEntidades();

	Entidad* getProtagonista();

	int getMargenScroll();

	virtual ~Juego();
};

#endif /* JUEGO_H_ */
