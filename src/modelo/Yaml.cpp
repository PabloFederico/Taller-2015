/*
 * Yaml.cpp
 *
 *  Created on: 29 de oct. de 2015
 *      Author: emanuel
 */

#include "Yaml.h"
#include <yaml-cpp/yaml.h>
#include "../modelo/ConfigDefault.h"
#include "../utils/Calculador.h"
#include "../utils/Log.h"

ConfiguracionJuego Yaml::cargarConfiguracionJuego(std::string archivoConfig){
	ConfiguracionJuego conf_game;

	map<string,TipoEntidad> tipos;
		tipos["arbol"] = ARBOL;
		tipos["tierra"] = TIERRA;
		tipos["agua"] = AGUA;
		tipos["castillo"] = CASTILLO;
		tipos["cuartel"] = CUARTEL;
		tipos["centro_civico"] = CENTRO_CIVICO;
		tipos["soldado"] = SOLDADO;
		tipos["aldeano"] = ALDEANO;
		tipos["animal"] = ANIMAL;


	YAML::Node config;
	try {
		config = YAML::LoadFile("config.yaml");
	}
	catch (YAML::BadFile &e) {
		crearConfigDefault();
		Log::imprimirALog(ERR_FAT,"Archivo de configuración no encontrado. Se ha creado uno nuevo por defecto.");
		config = YAML::LoadFile("config.yaml");
	}
	catch (YAML::ParserException &e) {
		Log::imprimirALog(WAR,"Error en el archivo de configuración: se tomarán los valores por defecto.");
		return OdioYAML();
	}

	InfoEscenario infoEsc;

	try
	{
		if (config["nombre_jugador"]) conf_game.nombreJugador = config["nombre_jugador"].as<string>();

		if (config["pantalla"]["alto"] && config["pantalla"]["alto"].as<int>() > 0)
			conf_game.alto_pantalla = config["pantalla"]["alto"].as<int>();
		else Log::imprimirALog(WAR,"Alto de pantalla no definido, se tomará " + conf_game.alto_pantalla);

		if (config["pantalla"]["ancho"] && config["pantalla"]["ancho"].as<int>() > 0)
			conf_game.ancho_pantalla = config["pantalla"]["ancho"].as<int>();
		else Log::imprimirALog(WAR,"Ancho de pantalla no definido, se tomará " + conf_game.ancho_pantalla);

		if (config["configuracion"]["vel_personaje"])
			conf_game.vel_personaje = Calculador::ChequeoDeBorde(250, config["configuracion"]["vel_personaje"].as<int>());
		else Log::imprimirALog(WAR,"Velocidad de personaje no definido, se tomará " + conf_game.vel_personaje);

		if (config["configuracion"]["margen_scroll"] && config["configuracion"]["margen_scroll"].as<int>() >= 0)
			conf_game.margen_scroll = config["configuracion"]["margen_scroll"].as<int>();
		else Log::imprimirALog(WAR,"Margen de scroll no definido, se usará: " + conf_game.margen_scroll);

		if (config["configuracion"]["rango_de_vision"] && config["configuracion"]["rango_de_vision"].as<int>() > 0)
			conf_game.rango_vision = config["configuracion"]["rango_de_vision"].as<int>();
		else Log::imprimirALog(WAR,"Rango de visión no definido, se usará: " + conf_game.rango_vision);

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
				else Log::imprimirALog(ERR,"Error: No se encontró imagen para "+unTipo["nombre"].as<string>());

				if (unTipo["alto_base"] && unTipo["alto_base"].as<int>() > 0)	iE.alto = unTipo["alto_base"].as<int>();
				if (unTipo["ancho_base"] && unTipo["ancho_base"].as<int>() > 0) iE.ancho = unTipo["ancho_base"].as<int>();
				if (unTipo["pixel_ref_x"])										iE.pixel_ref_x = unTipo["pixel_ref_x"].as<int>();
				if (unTipo["pixel_ref_y"])										iE.pixel_ref_y = unTipo["pixel_ref_y"].as<int>();
				if (unTipo["fps"] && unTipo["fps"].as<int>() >= 0)				iE.fps = unTipo["fps"].as<int>();
				if (unTipo["delay"] && unTipo["delay"].as<int>() >= 0) 			iE.delay = unTipo["delay"].as<int>();
				if (unTipo["descripcion"])										iE.descripcion = unTipo["descripcion"].as<string>();

				conf_game.agregarInfoEntidad(iE);
				//this->vectorInfoTiposEntidades.push_back(iE);
			}
		}

		if (config["escenario"] && config["escenario"][0]["size_x"].as<int>() > 0 && config["escenario"][0]["size_y"].as<int>() > 0) {
			//for(std::size_t i = 0; i < config["escenario"].size(); ++i) {			// Adaptable a varios escenarios
				//vecEscenarios.push_back(InfoEscenario(config["escenario"][i]));
				YAML::Node unEscenario = config["escenario"][0];

				infoEsc.nombre = unEscenario["nombre"].as<std::string>();
				infoEsc.size_x = unEscenario["size_x"].as<int>();
				infoEsc.size_y = unEscenario["size_y"].as<int>();
				for(std::size_t i = 0; i < unEscenario["entidades"].size(); ++i) {
					YAML::Node ent = unEscenario["entidades"][i];
					TipoEntidad tipo = tipos[ent["tipo"].as<string>()];
					if (tipo != 0) {
						std::vector<InfoEntidad>::iterator it;
						for (it = conf_game.entidades.begin(); it < conf_game.entidades.end(); ++it)
							if (it->tipo == tipo)
								break;
						if (it != conf_game.entidades.end()) {
							int x = Calculador::ChequeoDeBorde(infoEsc.size_x-(it->ancho), ent["x"].as<int>());
							int y = Calculador::ChequeoDeBorde(infoEsc.size_y-(it->alto), ent["y"].as<int>());
							if ((x!=ent["x"].as<int>()) || (y!=ent["y"].as<int>()))
								Log::imprimirALog(WAR,"Coordenadas inválidas para '" + ent["tipo"].as<string>() + "'; reposicionado");

							infoEsc.agregarEntidad( make_pair(x,y), tipo );
						} else Log::imprimirALog(WAR,"Error: El tipo '" + ent["tipo"].as<string>() + "' no fue configurado");

					} else Log::imprimirALog(ERR,"Error: Tipo '" + ent["tipo"].as<string>() + "' desconocido");
				}
				/*
				YAML::Node protag = unEscenario["protagonista"][0];
				infoEsc.protagonista = SOLDADO;	//hardcodeo
				if (tipos[protag["tipo"].as<string>()] == 0)
					Log::imprimirALog(ERR,"Error: Tipo '" + protag["tipo"].as<string>() + "' desconocido");
				else infoEsc.protagonista = tipos[protag["tipo"].as<string>()];

				infoEsc.posX_protagonista = Calculador::ChequeoDeBorde(infoEsc.size_x-1, protag["x"].as<int>());
				infoEsc.posY_protagonista = Calculador::ChequeoDeBorde(infoEsc.size_y-1, protag["y"].as<int>());
				*/
				// Asume que protagnista ocupa un único tile
			//}
		} else {
			infoEsc = infoEscenarioDefault();
			Log::imprimirALog(ERR,"Escenario no definido, se usará uno por defecto");
		}

	} catch( YAML::BadConversion &e )			//Hay forma de hacer un catch múltiple?
	{
		//infoEsc = infoEscenarioDefault();
		Log::imprimirALog(ERR,"Error en el archivo de configuración: se tomarán los valores por defecto.");
		return OdioYAML();
	} catch( YAML::BadSubscript &e )
	{
		//infoEsc = infoEscenarioDefault();
		Log::imprimirALog(ERR,"Error en el archivo de configuración: se tomarán los valores por defecto.");
		return OdioYAML();
	} catch( YAML::ParserException &e )
	{
		//infoEsc = infoEscenarioDefault();
		Log::imprimirALog(ERR,"Error en el archivo de configuración: se tomarán los valores por defecto.");
		return OdioYAML();
	}
	if (!infoEsc) {		// Si no se cargó un escenario válido, se revierte al default.
		//infoEsc = infoEscenarioDefault();
		Log::imprimirALog(ERR,"Errores en la configuración de escenario; se utilizará uno predeterminado");
		return OdioYAML();
	}
	tipos.clear();
	conf_game.agregarInfoEscenarios(infoEsc);
	return conf_game;
}

ConfiguracionJuego Yaml::OdioYAML() {
	ConfiguracionJuego configDefault;

	map<string,TipoEntidad> tipos;
		tipos["arbol"] = ARBOL;
		tipos["tierra"] = TIERRA;
		tipos["agua"] = AGUA;
		tipos["castillo"] = CASTILLO;
		tipos["cuartel"] = CUARTEL;
		tipos["centro_civico"] = CENTRO_CIVICO;
		tipos["soldado"] = SOLDADO;
		tipos["aldeano"] = ALDEANO;
		tipos["animal"] = ANIMAL;

	configDefault.nombreJugador = "JugadorXXX";
	configDefault.direccion_ip = "10.0.0.1";
	configDefault.ancho_pantalla = 1080;
	configDefault.alto_pantalla = 640;
	configDefault.margen_scroll = 20;
	configDefault.vel_personaje = 50;
	configDefault.rango_vision = 5;


	InfoEntidad infoArbol;
	infoArbol.tipo = tipos["arbol"];
	infoArbol.path = "images/arbol.png";
	infoArbol.descripcion = "Arbol";

	InfoEntidad infoTierra;
	infoTierra.tipo = tipos["tierra"];
	infoTierra.path = "images/tierra.png";

	InfoEntidad infoAgua;
	infoAgua.tipo = tipos["agua"];
	infoAgua.path = "images/agua.png";
/*
	InfoEntidad infoCastillo;
	infoCastillo.tipo = tipos["castillo"];
	infoCastillo.path = "images/casss.png";
	infoCastillo.ancho = 4;
	infoCastillo.alto = 4;
	infoCastillo.descripcion = "Castillo";
*/
	InfoEntidad infoSoldado;
	infoSoldado.tipo = tipos["soldado"];
	infoSoldado.path = "images/soldado.png";
	infoSoldado.fps = 40;
	infoSoldado.descripcion = "Soldado";

	InfoEntidad infoAldeano;
	infoAldeano.tipo = tipos["aldeano"];
	infoAldeano.path = "images/aldeano.png";
	infoAldeano.fps = 40;
	infoAldeano.descripcion = "Aldeano";

	InfoEntidad infoAnimal;
	infoAnimal.tipo = tipos["animal"];
	infoAnimal.path = "images/animal1.png";
	infoAnimal.delay = 3000;
	infoAnimal.fps = 10;
	infoAnimal.descripcion = "Animal";

	InfoEntidad infoCuartel;
	infoCuartel.tipo = tipos["cuartel"];
	infoCuartel.path = "images/cuartel.png";
	infoCuartel.descripcion = "Cuartel";

	InfoEntidad infoCentroCivico;
	infoCentroCivico.tipo = tipos["centro_civico"];
	infoCentroCivico.path = "images/centro_civico.png";
	infoCentroCivico.descripcion = "Centro Civico";

	configDefault.agregarInfoEntidad(infoArbol);
	configDefault.agregarInfoEntidad(infoAgua);
	configDefault.agregarInfoEntidad(infoCuartel);
	configDefault.agregarInfoEntidad(infoCentroCivico);
	configDefault.agregarInfoEntidad(infoSoldado);
	configDefault.agregarInfoEntidad(infoAldeano);
	configDefault.agregarInfoEntidad(infoAnimal);
	configDefault.agregarInfoEntidad(infoTierra);

	tipos.clear();
	configDefault.agregarInfoEscenarios(infoEscenarioDefault());
	return configDefault;
}

InfoEscenario Yaml::infoEscenarioDefault() {
	InfoEscenario infoEscenario;

	infoEscenario.size_x = 50;
	infoEscenario.size_y = 50;
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


	infoEscenario.agregarEntidad(std::make_pair(10,14), ANIMAL);

	//código de prueba
	std::cout<<infoEscenario.getPosicionesEntidades().size()<<std::endl;

	return infoEscenario;
}
