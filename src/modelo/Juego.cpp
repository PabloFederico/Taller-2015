/*
 * Juego.cpp
 *
 *  Created on: 6 de sept. de 2015
 *      Author: emanuel
 */

#include "../modelo/Juego.h"
#include <iostream>


Juego::Juego() {
	this->cero_x = NULL;
	this->cero_y = NULL;
	this->contenedor = NULL;
	this->escenario = NULL;
	this->protagonista = NULL;
	this->screenWidth = 800;	// Default
	this->screenHeight = 600;	// Default
	this->vel_personaje = 50;
	this->margen_scroll = 30;

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
void imprimirAlLog(string output) {
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];
	time (&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer,80,"%Y-%m-%d %I:%M:%S",timeinfo);
	std::string timestamp(buffer);

	std::ofstream fLog("TP.log", std::ofstream::app);
	fLog << "[" << timestamp << "] " << output << std::endl;
	fLog.close();
	std::cerr << output << std::endl;
}

/********************************************************************************/
void Juego::cargarJuego(){
	// Valores default
	this->screenWidth = 800;
	this->screenHeight = 600;
	this->vel_personaje = 30;
	this->margen_scroll = 50;
	//	std::vector<InfoEscenario> vecEscenarios;

	//---------------------------------------------------------------------------------------------!!
	InfoEscenario infoEsc = parsearConfig();
	// !!! Para el que no le funciona YAML, comentar la línea de arriba y descomentar la de abajo.
	//InfoEscenario infoEsc = OdioYAML();
	//---------------------------------------------------------------------------------------------!!


	// Acá me imagino la posibilidad de un selector de escenarios.

	this->escenario = new Escenario(infoEsc);
	this->protagonista = this->escenario->getProtagonista();
}

/********************************************************************************/
InfoEscenario Juego::parsearConfig() {
	map<string,TipoEntidad> tipos;
		tipos["arbol"] = ARBOL;
		tipos["tierra"] = TIERRA;
		tipos["agua"] = AGUA;
		tipos["castillo"] = CASTILLO;
		tipos["soldado"] = SOLDADO;
		tipos["juana_de_arco"] = JUANA_DE_ARCO;
		tipos["animal"] = ANIMAL;


	YAML::Node config;
	try {
		config = YAML::LoadFile("config.yaml");
	}
	catch (YAML::BadFile &e) {
		crearConfigDefault();
		imprimirAlLog("Archivo de configuración no encontrado. Se ha creado uno nuevo por defecto.");
		config = YAML::LoadFile("config.yaml");
	}
	catch (YAML::ParserException &e) {
		imprimirAlLog("Error en el archivo de configuración: se tomarán los valores por defecto.");
		return OdioYAML();
	}

	InfoEscenario infoEsc;

	try
	{
		if (config["pantalla"]["alto"] && config["pantalla"]["alto"].as<int>() > 0)
			this->screenHeight = config["pantalla"]["alto"].as<int>();
		else imprimirAlLog("Alto de pantalla no definido, se tomará " + this->screenHeight);
		if (config["pantalla"]["ancho"] && config["pantalla"]["ancho"].as<int>() > 0)
			this->screenWidth = config["pantalla"]["ancho"].as<int>();
		else imprimirAlLog("Ancho de pantalla no definido, se tomará " + this->screenWidth);

		if (config["configuracion"]["vel_personaje"])
			vel_personaje = ChequeoDeBorde(250, config["configuracion"]["vel_personaje"].as<int>());
		else imprimirAlLog("Velocidad de personaje no definido, se tomará " + vel_personaje);
		if (config["configuracion"]["margen_scroll"] && config["configuracion"]["margen_scroll"].as<int>() >= 0)
			margen_scroll = config["configuracion"]["margen_scroll"].as<int>();
		else imprimirAlLog("Margen de scroll no definido, se usará: " + margen_scroll);

		if (config["tipos"]) {
			for(std::size_t i = 0; i < config["tipos"].size(); ++i) {
				YAML::Node unTipo = config["tipos"][i];

				InfoEntidad iE;
				if (tipos[unTipo["nombre"].as<string>()] == 0) {
					tipos[unTipo["nombre"].as<string>()] = OTROS;
					iE.tipo = OTROS;
				} else iE.tipo = tipos[unTipo["nombre"].as<string>()];
				if ((unTipo["imagen"]) && (access(unTipo["imagen"].as<string>().c_str(), F_OK) != -1))		// Verificación de existencia
											iE.path = unTipo["imagen"].as<string>();
				else imprimirAlLog("Error: No se encontró imagen para " + unTipo["nombre"].as<string>());
				if (unTipo["alto_base"] && unTipo["alto_base"].as<int>() > 0)	iE.alto = unTipo["alto_base"].as<int>();
				if (unTipo["ancho_base"] && unTipo["ancho_base"].as<int>() > 0) iE.ancho = unTipo["ancho_base"].as<int>();
				if (unTipo["pixel_ref_x"])										iE.pixel_ref_x = unTipo["pixel_ref_x"].as<int>();
				if (unTipo["pixel_ref_y"])										iE.pixel_ref_y = unTipo["pixel_ref_y"].as<int>();
				if (unTipo["fps"] && unTipo["fps"].as<int>() >= 0)				iE.fps = unTipo["fps"].as<int>();
				if (unTipo["delay"] && unTipo["delay"].as<int>() >= 0) 			iE.delay = unTipo["delay"].as<int>();

				this->vectorInfoTiposEntidades.push_back(iE);
			}
		}

		if (config["escenario"] && config["escenario"][0]["size_x"].as<int>() > 0 && config["escenario"][0]["size_y"].as<int>() > 0) {
			//for(std::size_t i = 0; i < config["escenario"].size(); ++i) {			// Adaptable a varios escenarios
				//vecEscenarios.push_back(InfoEscenario(config["escenario"][i]));
				YAML::Node unEscenario = config["escenario"][0];

				//infoEsc.nombre = unEscenario["nombre"].as<std::string>();
				infoEsc.size_x = unEscenario["size_x"].as<int>();
				infoEsc.size_y = unEscenario["size_y"].as<int>();
				for(std::size_t i = 0; i < unEscenario["entidades"].size(); ++i) {
					YAML::Node ent = unEscenario["entidades"][i];
					TipoEntidad tipo = tipos[ent["tipo"].as<string>()];
					if (tipo != 0) {
						std::vector<InfoEntidad>::iterator it;
						for (it = this->vectorInfoTiposEntidades.begin(); it < this->vectorInfoTiposEntidades.end(); ++it)
							if (it->tipo == tipo)
								break;
						if (it != this->vectorInfoTiposEntidades.end()) {
							int x = ChequeoDeBorde(infoEsc.size_x-(it->ancho), ent["x"].as<int>());
							int y = ChequeoDeBorde(infoEsc.size_y-(it->alto), ent["y"].as<int>());
							if ((x!=ent["x"].as<int>()) || (y!=ent["y"].as<int>()))
								imprimirAlLog("Coordenadas inválidas para '" + ent["tipo"].as<string>() + "'; reposicionado");
							infoEsc.agregarEntidad( make_pair(x,y), tipo );
						} else imprimirAlLog("Error: El tipo '" + ent["tipo"].as<string>() + "' no fue configurado");
					} else imprimirAlLog("Error: Tipo '" + ent["tipo"].as<string>() + "' desconocido");
				}

				YAML::Node protag = unEscenario["protagonista"][0];
				infoEsc.protagonista = SOLDADO;	//hardcodeo
				if (tipos[protag["tipo"].as<string>()] == 0)
					imprimirAlLog("Error: Tipo '" + protag["tipo"].as<string>() + "' desconocido");
				else infoEsc.protagonista = tipos[protag["tipo"].as<string>()];
				infoEsc.posX_protagonista = ChequeoDeBorde(infoEsc.size_x-1, protag["x"].as<int>());
				infoEsc.posY_protagonista = ChequeoDeBorde(infoEsc.size_y-1, protag["y"].as<int>());
				// Asume que protagnista ocupa un único tile
			//}
		} else {
			infoEsc = infoEscenarioDefault();
			imprimirAlLog("Escenario no definido, se usará uno por defecto");
		}

	}
	catch( YAML::BadConversion &e )			//Hay forma de hacer un catch múltiple?
	{
		//infoEsc = infoEscenarioDefault();
		imprimirAlLog("Error en el archivo de configuración: se tomarán los valores por defecto.");
		return OdioYAML();
	} catch( YAML::BadSubscript &e )
	{
		//infoEsc = infoEscenarioDefault();
		imprimirAlLog("Error en el archivo de configuración: se tomarán los valores por defecto.");
		return OdioYAML();
	} catch( YAML::ParserException &e )
	{
		//infoEsc = infoEscenarioDefault();
		imprimirAlLog("Error en el archivo de configuración: se tomarán los valores por valores por defecto.");
		return OdioYAML();
	}
	if (!infoEsc) {		// Si no se cargó un escenario válido, se revierte al default.
		//infoEsc = infoEscenarioDefault();
		imprimirAlLog("Errores en la configuración de escenario; se utilizará uno predeterminado");
		return OdioYAML();
	}
	tipos.clear();
	return infoEsc;
}

/********************************************************************************/
int Juego::ChequeoDeBorde(int max, int input) {
	if (input >= 0) {
		if (input <= max)
			return input;
		else return max;
	} else return 0;
}

/********************************************************************************/
InfoEscenario Juego::infoEscenarioDefault() {
	InfoEscenario infoEscenario;

	infoEscenario.size_x = 20;
	infoEscenario.size_y = 20;
	infoEscenario.agregarEntidad(std::make_pair(10,0), AGUA);
	infoEscenario.agregarEntidad(std::make_pair(11,0), AGUA);
	infoEscenario.agregarEntidad(std::make_pair(12,0), AGUA);
	infoEscenario.agregarEntidad(std::make_pair(13,0), AGUA);
	infoEscenario.agregarEntidad(std::make_pair(10,1), AGUA);
	infoEscenario.agregarEntidad(std::make_pair(11,1), AGUA);
	infoEscenario.agregarEntidad(std::make_pair(12,1), AGUA);
	infoEscenario.agregarEntidad(std::make_pair(13,1), AGUA);

	infoEscenario.agregarEntidad(std::make_pair(4,10), TIERRA);
	infoEscenario.agregarEntidad(std::make_pair(4,11), TIERRA);
	infoEscenario.agregarEntidad(std::make_pair(4,12), TIERRA);

	infoEscenario.agregarEntidad(std::make_pair(4,5), ARBOL);
	infoEscenario.agregarEntidad(std::make_pair(4,6), ARBOL);
	infoEscenario.agregarEntidad(std::make_pair(5,5), ARBOL);
	infoEscenario.agregarEntidad(std::make_pair(4,12), ARBOL);

	infoEscenario.agregarEntidad(std::make_pair(0,10), CASTILLO);

	infoEscenario.agregarEntidad(std::make_pair(10,14), ANIMAL);

	//código de prueba
	std::cout<<infoEscenario.getPosicionesEntidades().size()<<std::endl;

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
	delete this->escenario;
}



/*************!!!***********************!!!**************************!!!********/
InfoEscenario Juego::OdioYAML() {
	map<string,TipoEntidad> tipos;
		tipos["arbol"] = ARBOL;
		tipos["tierra"] = TIERRA;
		tipos["agua"] = AGUA;
		tipos["castillo"] = CASTILLO;
		tipos["soldado"] = SOLDADO;
		tipos["juana_de_arco"] = JUANA_DE_ARCO;
		tipos["animal"] = ANIMAL;

	InfoEntidad infoArbol;
	infoArbol.tipo = tipos["arbol"];
	infoArbol.path = "images/arbol.png";

	InfoEntidad infoTierra;
	infoTierra.tipo = tipos["tierra"];
	infoTierra.path = "images/tierra.png";

	InfoEntidad infoAgua;
	infoAgua.tipo = tipos["agua"];
	infoAgua.path = "images/agua.png";

	InfoEntidad infoCastillo;
	infoCastillo.tipo = tipos["castillo"];
	infoCastillo.path = "images/castillo.png";
	infoCastillo.ancho = 4;
	infoCastillo.alto = 3;

	InfoEntidad infoSoldado;
	infoSoldado.tipo = tipos["soldado"];
	infoSoldado.path = "images/soldado.png";
	infoSoldado.fps = 50;
	infoSoldado.delay = 0;

	InfoEntidad infoJuana;
	infoJuana.tipo = tipos["juana_de_arco"];
	infoJuana.path = "images/juana.png";
	infoJuana.fps = 10;

	InfoEntidad infoAnimal;
	infoAnimal.tipo = tipos["animal"];
	infoAnimal.path = "images/animal.png";
	infoAnimal.fps = 50;


	this->vectorInfoTiposEntidades.push_back(infoArbol);
	this->vectorInfoTiposEntidades.push_back(infoTierra);
	this->vectorInfoTiposEntidades.push_back(infoAgua);
	this->vectorInfoTiposEntidades.push_back(infoCastillo);
	this->vectorInfoTiposEntidades.push_back(infoSoldado);
	this->vectorInfoTiposEntidades.push_back(infoJuana);
	this->vectorInfoTiposEntidades.push_back(infoAnimal);

	tipos.clear();
	return infoEscenarioDefault();
}

/***************************************************/
int Juego::getVelocidad(){
	return this->vel_personaje;
}

void Juego::cargarRecursos(ContenedorDeRecursos *container){
	this->contenedor = container;
}

Sprite* Juego::getSpritePlayer(){
	return this->contenedor->getSpriteDeEntidad(protagonista);
}

void Juego::setCeros(int *x, int *y){
	this->cero_x = x;
	this->cero_y = y;
}

std::pair<int*,int*> Juego::getCeros(){
	return std::make_pair(cero_x,cero_y);
}

void Juego::reiniciar(){
	delete this->escenario;
	delete this->contenedor;
	cargarJuego();
}

void Juego::actualizarPosicionesEntidades(int cant_x, int cant_y){
	this->contenedor->actualizarPosicionesEntidades(cant_x,cant_y);
}
