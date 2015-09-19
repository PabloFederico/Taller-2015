/*
 * Juego.cpp
 *
 *  Created on: 6 de sept. de 2015
 *      Author: emanuel
 */

#include "Juego.h"
#include "Soldado.h"
#include "Castillo.h"
#include "VistaEntidad.h"
#include <stdio.h>


Juego::Juego() {
	this->escenario = NULL;
	this->protagonista = NULL;
	this->screenWidth = 800;	// Default
	this->screenHeight = 600;	// Default
	this->mapEntidades = new std::map<std::string,VistaEntidad*>();
	this->cargarJuego();
}

/********************************************************************************/
void Juego::cargarConfigEntidad(std::string tipo, VistaEntidad *vEntidad){
	this->mapEntidades->insert(std::make_pair(tipo,vEntidad));
}

/********************************************************************************/
std::map<std::string,VistaEntidad*>* Juego::getMapEntidades(){
	return this->mapEntidades;
}

/********************************************************************************/
void Juego::cargarEscenario(std::vector<DatosEscenario> vecEscenarios) {
	int size_x = 30, size_y = 30;	// Default?
	/* Posible selección de escenario */

	size_x = vecEscenarios[0].size_x;
	size_y = vecEscenarios[0].size_y;

	this->escenario = new Escenario(size_x,size_y);


	// Código de prueba
	Entidad *arbol = new Entidad("arbol");
	Suelo *tierra = new Suelo("tierra");
	Suelo *agua = new Suelo("agua");
	Castillo *castillo = new Castillo("castillo");
	Soldado *soldado = new Soldado("soldado");

	/* Falta agregarEntidad de cada una en el escenario : cambiar tipo de vector a lo nuevo. */
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
	///////////////////////


	/*Entidad *arbol = new Entidad(ARBOL);
	arbol->setPath("images/arbol.png");

	Suelo *tierra = new Suelo(TIERRA);
	tierra->setPath("images/tierra.png");

	Suelo *agua = new Suelo(AGUA);
	agua->setPath("images/agua.png");

	Castillo *castillo = new Castillo(CASTILLO);
	castillo->setPath("images/castle.png");

	Soldado *soldado = new Soldado(SOLDADO);
	soldado->setPath("images/man.png");

	this->cargarDatosEntidad(ARBOL,arbol);
	this->cargarDatosEntidad(TIERRA,tierra);
	this->cargarDatosEntidad(SOLDADO,soldado);
	this->cargarDatosEntidad(AGUA,agua);
	this->cargarDatosEntidad(CASTILLO,castillo);*/

	/*this->escenario->agregarEntidad(std::make_pair(10,0),agua);
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
*/

	cargarEscenario(parsearConfig());
}

/********************************************************************************/
std::pair<int,int> Juego::dimensionVentana(){
	return std::make_pair(this->screenWidth,this->screenHeight);
}

/********************************************************************************/
std::vector<DatosEscenario> Juego::parsearConfig() {
	YAML::Node config = YAML::LoadFile("config.yaml");
	/*
	int alto;
	if (config["pantalla"]["prueba"])
		alto = config["pantalla"]["prueba"].as<int>();

	std::cout << alto;
	*/
	// Valores por default.		!! Algún otro parámetro?
	int vel_personaje = 3;
	int margen_scroll = 30;		// Acá se tienen los valores, hacer con ellos lo que se quiera. //
	std::vector<DatosEscenario> vecEscenarios;

	try
	{
		if (config["pantalla"]["alto"])
			this->screenHeight = config["pantalla"]["alto"].as<int>();
		else std::cout << "Alto de pantalla no definido, se tomará " << this->screenHeight << std::endl;
		if (config["pantalla"]["ancho"])
			this->screenWidth = config["pantalla"]["ancho"].as<int>();
		else std::cout << "Ancho de pantalla no definido, se tomará " << this->screenWidth << std::endl;

		if (config["configuracion"]["vel_personaje"])
			vel_personaje = config["configuracion"]["vel_personaje"].as<int>();
		else std::cout << "Velocidad de personaje no definido, se tomará " << vel_personaje << std::endl;
		if (config["configuracion"]["margen_scroll"])
			margen_scroll = config["configuracion"]["margen_scroll"].as<int>();
		else std::cout << "Margen de scroll no definido, se usará: " << margen_scroll << std::endl;

		if (config["tipos"]) {
			for(std::size_t i = 0; i < config["tipos"].size(); ++i) {
				// Separar este código
				YAML::Node tipo = config["tipos"][i];
				VistaEntidad *ve = new VistaEntidad( tipo["nombre"].as<string>() );
				if (tipo["imagen"]) 	ve->setPath(tipo["imagen"].as<string>());
				if (tipo["alto_base"]) 	ve->setAltoBase(tipo["alto_base"].as<int>());
				if (tipo["ancho_base"]) ve->setAnchoBase(tipo["ancho_base"].as<int>());
				if (tipo["pixel_ref_x"]) ve->setPixelRefX(tipo["pixel_ref_x"].as<int>());
				if (tipo["pixel_ref_y"]) ve->setPixelRefY(tipo["pixel_ref_y"].as<int>());
				if (tipo["fps"]) 		ve->setFps(tipo["fps"].as<int>());
				if (tipo["delay"]) 		ve->setDelay(tipo["delay"].as<int>());

				this->cargarConfigEntidad(tipo["nombre"].as<string>(), ve);
			}
		}

		if (config["escenario"]) {
			for(std::size_t i = 0; i < config["escenario"].size(); ++i) {
				vecEscenarios.push_back(DatosEscenario(config["escenario"][i]));
			}
		} else std::cout << "Escenario no definido, se usará uno por defecto" << std::endl;
	}
	catch( YAML::BadConversion &e )
	{
		std::cout << "Error en el archivo de configuración: se tomará el resto de los valores por defecto." << std::endl;
	}

	return vecEscenarios;
}

/********************************************************************************/
Juego::~Juego() {
	std::map<std::string,VistaEntidad*>::iterator p = this->mapEntidades->begin();
	while (p != this->mapEntidades->end()){		// Por qué no lo siguiente: for (p = this->mapEntidades->begin(); p != this->mapEntidades->end(); p++) \n delete (*p).second;
		VistaEntidad* vEnte = (*p).second;
		p++;
		delete vEnte;
	}
	delete this->escenario;
}

