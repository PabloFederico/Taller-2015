/*
 * Juego.cpp
 *
 *  Created on: 6 de sept. de 2015
 *      Author: emanuel
 */

#include "Juego.h"


Juego::Juego() {
	this->escenario = NULL;
	this->protagonista = NULL;
	this->screenWidth = 800;	// Default
	this->screenHeight = 600;	// Default
	this->vel_personaje = 1;
	this->margen_scroll = 30;
//	this->mapEntidades = new std::map<std::string,VistaEntidad*>();
	this->cargarJuego();
}

/********************************************************************************/
vector<InfoEntidad> Juego::getInfoTiposEntidades(){
	return this->vectorInfoTiposEntidades;
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
	// Valores default
	this->screenWidth = 800;
	this->screenHeight = 600;
	this->vel_personaje = 5;
	this->margen_scroll = 30;
	//	std::vector<InfoEscenario> vecEscenarios;

	map<string,TipoEntidad> tipos;
		tipos["arbol"] = ARBOL;
		tipos["tierra"] = TIERRA;
		tipos["agua"] = AGUA;
		tipos["castillo"] = CASTILLO;
		tipos["soldado"] = SOLDADO;
		tipos["juana_de_arco"] = JUANA_DE_ARCO;


	YAML::Node config;
	try {
		config = YAML::LoadFile("config.yaml");
	}
	catch (YAML::BadFile &e) {
		crearConfigDefault();
		config = YAML::LoadFile("config.yaml");
	}

	InfoEscenario infoEsc;

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
				YAML::Node unTipo = config["tipos"][i];

				if (tipos[unTipo["nombre"].as<string>()] != 0) {
					InfoEntidad iE;
					iE.tipo = tipos[unTipo["nombre"].as<string>()];
					if ((unTipo["imagen"]) && (access(unTipo["imagen"].as<string>().c_str(), F_OK) != -1))		// Verificación de existencia
												iE.path = unTipo["imagen"].as<string>();
					else std::cerr << "Error: No se encontró imagen para" << unTipo["nombre"].as<string>() << std::endl;
					if (unTipo["alto_base"]) 	iE.alto = unTipo["alto_base"].as<int>();
					if (unTipo["ancho_base"]) 	iE.ancho = unTipo["ancho_base"].as<int>();
					if (unTipo["pixel_ref_x"])	iE.pixel_ref_x = unTipo["pixel_ref_x"].as<int>();
					if (unTipo["pixel_ref_y"])	iE.pixel_ref_y = unTipo["pixel_ref_y"].as<int>();
					if (unTipo["fps"]) 			iE.fps = unTipo["fps"].as<int>();
					if (unTipo["delay"]) 		iE.delay = unTipo["delay"].as<int>();

					this->vectorInfoTiposEntidades.push_back(iE);
				}
				else std::cerr << "Error: Tipo '" << unTipo["nombre"].as<string>() << "' desconocido" << std::endl;
				// Se podría sencillamente cambiar a que un tipo desconocido se agregue al map.
			}
		}

		if (config["escenario"]) {
			//for(std::size_t i = 0; i < config["escenario"].size(); ++i) {			// Adaptable a varios escenarios
				//vecEscenarios.push_back(InfoEscenario(config["escenario"][i]));
				YAML::Node unEscenario = config["escenario"][0];

				//infoEsc.nombre = unEscenario["nombre"].as<std::string>();
				infoEsc.size_x = unEscenario["size_x"].as<int>();
				infoEsc.size_y = unEscenario["size_y"].as<int>();
				for(std::size_t i = 0; i < unEscenario["entidades"].size(); ++i) {
					YAML::Node ent = unEscenario["entidades"][i];
					if (tipos[ent["tipo"].as<string>()] != 0)
						infoEsc.agregarEntidad(make_pair(ent["x"].as<int>(), ent["y"].as<int>()), tipos[ent["tipo"].as<string>()]);
					else std::cerr << "Error: Tipo '" << ent["tipo"].as<string>() << "' desconocido" << std::endl;
				}

				YAML::Node protag = unEscenario["protagonista"][0];
				infoEsc.protagonista = SOLDADO;
				if (tipos[protag["tipo"].as<string>()] == 0)
					std::cerr << "Error: Tipo '" << protag["tipo"].as<string>() << "' desconocido" << std::endl;
				else infoEsc.protagonista = tipos[protag["tipo"].as<string>()];
				infoEsc.posX_protagonista = protag["x"].as<int>();
				infoEsc.posY_protagonista = protag["y"].as<int>();
			//}
		} else {
			infoEsc = infoEscenarioDefault();
			std::cout << "Escenario no definido, se usará uno por defecto" << std::endl;
		}

	}
	catch( YAML::BadConversion &e )			//Hay forma de hacer un catch múltiple?
	{
		infoEsc = infoEscenarioDefault();
		std::cout << "Error en el archivo de configuración: se tomará el resto de los valores por defecto." << std::endl;
	} catch( YAML::BadSubscript &e )
	{
		infoEsc = infoEscenarioDefault();
		std::cout << "Error en el archivo de configuración: se tomará el resto de los valores por defecto." << std::endl;
	}
	if (!infoEsc) {	// Si no se cargó un escenario válido, se revierte al default.
		infoEsc = infoEscenarioDefault();
		std::cerr << "Errores en la configuración de escenario; se utilizará uno predeterminado" << std::endl;
	}

	// Acá me imagino la posibilidad de un selector de escenarios.

	this->escenario = new Escenario(infoEsc);
	this->protagonista = this->escenario->getProtagonista();

}

/********************************************************************************/
InfoEscenario Juego::infoEscenarioDefault() {
	InfoEscenario infoEscenario;

	infoEscenario.size_x = 20;
	infoEscenario.size_y = 20;
	infoEscenario.agregarEntidad(std::make_pair(10,0), AGUA);
	infoEscenario.agregarEntidad(std::make_pair(11,0), AGUA);
	infoEscenario.agregarEntidad(std::make_pair(4,10), TIERRA);
	infoEscenario.agregarEntidad(std::make_pair(4,11), TIERRA);
	infoEscenario.agregarEntidad(std::make_pair(4,12), TIERRA);
	infoEscenario.agregarEntidad(std::make_pair(4,5), ARBOL);
	infoEscenario.agregarEntidad(std::make_pair(4,6), ARBOL);
	infoEscenario.agregarEntidad(std::make_pair(5,5), ARBOL);
	infoEscenario.agregarEntidad(std::make_pair(4,12), ARBOL);
	infoEscenario.agregarEntidad(std::make_pair(10,10), CASTILLO);

	infoEscenario.protagonista = SOLDADO;
	infoEscenario.posX_protagonista = 3;
	infoEscenario.posY_protagonista = 3;

	return infoEscenario;
}
/********************************************************************************/
pair<int,int> Juego::dimensionVentana(){
	return make_pair(this->screenWidth,this->screenHeight);
}

/********************************************************************************/
int Juego::getMargenScroll(){
	return this->margen_scroll;
}

/********************************************************************************/
Juego::~Juego() {
/*	std::map<std::string,VistaEntidad*>::iterator p = this->mapEntidades->begin();
	while (p != this->mapEntidades->end()){		// Por qué no lo siguiente: for (p = this->mapEntidades->begin(); p != this->mapEntidades->end(); p++) \n delete (*p).second;
		VistaEntidad* vEnte = (*p).second;
		p++;
		delete vEnte;
	}*/
	delete this->escenario;
}

