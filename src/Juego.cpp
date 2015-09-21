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

/********************************************************************************
void Juego::cargarConfigEntidad(std::string tipo, VistaEntidad *vEntidad){
	this->mapEntidades->insert(std::make_pair(tipo,vEntidad));
}

********************************************************************************
std::map<std::string,VistaEntidad*>* Juego::getMapEntidades(){
	return this->mapEntidades;
}

********************************************************************************
void Juego::cargarEscenario(std::vector<DatosEscenario> vecEscenarios) {
	int size_x = 30, size_y = 30;	// Default?
	// Posible selección de escenario

	size_x = vecEscenarios[0].size_x;
	size_y = vecEscenarios[0].size_y;

	this->escenario = new Escenario(size_x,size_y);


	// Código de prueba
	Entidad *arbol = new Entidad("arbol");
	Suelo *tierra = new Suelo("tierra");
	Suelo *agua = new Suelo("agua");
	Castillo *castillo = new Castillo("castillo");
	Soldado *soldado = new Soldado("soldado");

	// Falta agregarEntidad de cada una en el escenario : cambiar tipo de vector a lo nuevo.
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
	///////////////////////

	this->protagonista = soldado;
}

********************************************************************************/
Escenario* Juego::getEscenario(){
	return this->escenario;
}

/********************************************************************************/
Entidad* Juego::getProtagonista(){
	return this->protagonista;
}

/********************************************************************************/
void Juego::cargarJuego(){
	this->screenWidth = 800;
	this->screenHeight = 600;
	this->vel_personaje = 5;
	this->margen_scroll = 30;
	
/*	YAML::Node config;
	try {
		config = YAML::LoadFile("config.yaml");
	}
	catch (YAML::BadFile &e) {
		crearConfigDefault();
		config = YAML::LoadFile("config.yaml");
	}
	cargarEscenario(parsearConfig(config));
*/

	map<string,TipoEntidad> tipos;
	tipos["arbol"] = ARBOL;
	tipos["tierra"] = TIERRA;
	tipos["agua"] = AGUA;
	tipos["castillo"] = CASTILLO;
	tipos["soldado"] = SOLDADO;
	tipos["juana_de_arco"] = JUANA_DE_ARCO;

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
	infoSoldado.fps = 40;
	infoSoldado.delay = 5;

	InfoEntidad infoJuana;
	infoJuana.tipo = tipos["juana_de_arco"];
	infoJuana.path = "images/juana.png";
	infoJuana.fps = 10;

	this->vectorInfoTiposEntidades.push_back(infoArbol);
	this->vectorInfoTiposEntidades.push_back(infoTierra);
	this->vectorInfoTiposEntidades.push_back(infoAgua);
	this->vectorInfoTiposEntidades.push_back(infoCastillo);
	this->vectorInfoTiposEntidades.push_back(infoSoldado);
	this->vectorInfoTiposEntidades.push_back(infoJuana);


	InfoEscenario infoEscenario;
	infoEscenario.size_x = 100;
	infoEscenario.size_y = 100;
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

	infoEscenario.protagonista = tipos["soldado"];
	infoEscenario.posX_protagonista = 3;
	infoEscenario.posY_protagonista = 3;

	this->escenario = new Escenario(infoEscenario);

	this->protagonista = this->escenario->getProtagonista();

}

/********************************************************************************
std::pair<int,int> Juego::dimensionVentana(){
	return std::make_pair(this->screenWidth,this->screenHeight);
}
********************************************************************************
std::vector<DatosEscenario> Juego::parsearConfig(YAML::Node config) {

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
	catch( YAML::BadConversion &e )		// Hay forma de hacer un catch múltiple?
	{
		std::cout << "Error en el archivo de configuración: se tomará el resto de los valores por defecto." << std::endl;
	} catch( YAML::BadSubscript &e )
	{
		std::cout << "Error en el archivo de configuración: se tomará el resto de los valores por defecto." << std::endl;
	}

	return vecEscenarios;
}

********************************************************************************/
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

