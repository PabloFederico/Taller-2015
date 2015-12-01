/*
 * Yaml.cpp
 *
 *  Created on: 29 de oct. de 2015
 *      Author: emanuel
 */

#include "../utils/Yaml.h"

#include <yaml-cpp/yaml.h>
#include "../utils/Calculador.h"
#include "../utils/ConfigDefault.h"
#include "../utils/Log.h"

ConfiguracionJuego Yaml::cargarConfiguracionJuego(std::string archivoConfig) {
	ConfiguracionJuego conf_game;

	map<string,TipoEntidad> tipos;
		tipos["arbol"] = ARBOL;
		tipos["tierra"] = TIERRA;
		tipos["agua"] = AGUA;
		tipos["castillo"] = CASTILLO;
		tipos["cuartel"] = CUARTEL;
		tipos["barraca"] = BARRACK;
		//		tipos["barraca_2"] = BARRACK_2;
		//		tipos["barraca_3"] = BARRACK_3;
		tipos["centro_civico"] = CENTRO_CIVICO;
		tipos["construccion"] = CONSTRUCCION;
		tipos["construccion_cuartel"] = CONSTRUCCION_CUARTEL;
		tipos["construccion_barraca"] = CONSTRUCCION_BARRACK;
		tipos["soldado"] = SOLDADO;
		tipos["aldeano"] = ALDEANO;
		tipos["arquero"] = ARQUERO;
		tipos["animal"] = ANIMAL;
		tipos["mina_oro"] = MINA_ORO;
		tipos["mina_piedra"] = MINA_PIEDRA;


	YAML::Node config;
	try {
		config = YAML::LoadFile(archivoConfig);
	}
	catch (YAML::BadFile &e) {
		crearConfigDefault();
		Log::imprimirALog(ERR_FAT,"Archivo de configuración no encontrado. Se ha creado uno nuevo por defecto.");
		config = YAML::LoadFile(archivoConfig);
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
			//}
		} else {
			infoEsc = infoEscenarioDefault();
			Log::imprimirALog(ERR,"Escenario no definido, se usará uno por defecto");
		}

	} catch( YAML::BadConversion &e )			//Hay forma de hacer un catch múltiple?
	{
		//infoEsc = infoEscenarioDefault();
		Log::imprimirALog(ERR,"Error BadConversion en el archivo de configuración: se tomarán los valores por defecto.");
		return OdioYAML();
	} catch( YAML::BadSubscript &e )
	{
		//infoEsc = infoEscenarioDefault();
		Log::imprimirALog(ERR,"Error BadSubscript en el archivo de configuración: se tomarán los valores por defecto.");
		return OdioYAML();
	} catch( YAML::ParserException &e )
	{
		//infoEsc = infoEscenarioDefault();
		Log::imprimirALog(ERR,"Error ParserException en el archivo de configuración: se tomarán los valores por defecto.");
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
		tipos["barraca"] = BARRACK;//_1;
//		tipos["barraca_2"] = BARRACK_2;
//		tipos["barraca_3"] = BARRACK_3;
		tipos["cuartel"] = CUARTEL;
		tipos["centro_civico"] = CENTRO_CIVICO;
		tipos["construccion"] = CONSTRUCCION;
		tipos["construccion_barraca"] = CONSTRUCCION_BARRACK;
		tipos["construccion_cuartel"] = CONSTRUCCION_CUARTEL;
		tipos["soldado"] = SOLDADO;
		tipos["aldeano"] = ALDEANO;
		tipos["arquero"] = ARQUERO;
		tipos["animal"] = ANIMAL;
		tipos["mina_oro"] = MINA_ORO;
		tipos["mina_piedra"] = MINA_PIEDRA;

	configDefault.nombreJugador = "Village Slayer";
	configDefault.direccion_ip = "127.0.0.1";
	configDefault.ancho_pantalla = 800;
	configDefault.alto_pantalla = 640;
	configDefault.margen_scroll = 15;
	configDefault.vel_personaje = 50;
	configDefault.rango_vision = 5;


	InfoEntidad infoArbol;
	infoArbol.tipo = tipos["arbol"];
	infoArbol.path = "images/utils/arbol.png";
	infoArbol.descripcion = "Arbol";

	InfoEntidad infoTierra;
	infoTierra.tipo = tipos["tierra"];
	infoTierra.path = "images/relieve/tierra.png";

	InfoEntidad infoAgua;
	infoAgua.tipo = tipos["agua"];
	infoAgua.path = "images/relieve/agua.png";

	InfoEntidad infoBarraca1;
	infoBarraca1.tipo = tipos["barraca"];
	infoBarraca1.path = "images/utils/Barracks5.png";
	infoBarraca1.ancho = 4;
	infoBarraca1.alto = 4;
	infoBarraca1.descripcion = "Barraca_Arquero";

	InfoEntidad infoBarraca1Constr;
	infoBarraca1Constr.tipo = tipos["construccion_barraca"];
	infoBarraca1Constr.path = "images/utils/Barracks5.png";
	infoBarraca1Constr.ancho = 4;
	infoBarraca1Constr.alto = 4;
	infoBarraca1Constr.descripcion = "Construcción de barraca";
/*
	InfoEntidad infoBarraca2;
	infoBarraca2.tipo = tipos["barraca_2"];
	infoBarraca2.path = "images/utils/Barracks4.png";
	infoBarraca2.ancho = 4;
	infoBarraca2.alto = 4;
	infoBarraca2.descripcion = "Barraca_2";

	InfoEntidad infoBarraca3;
	infoBarraca3.tipo = tipos["barraca_3"];
	infoBarraca3.path = "images/utils/Barracks5.png";
	infoBarraca3.ancho = 4;
	infoBarraca3.alto = 4;
	infoBarraca3.descripcion = "Barraca_3";
*/
	InfoEntidad infoSoldado;
	infoSoldado.tipo = tipos["soldado"];
	infoSoldado.path = "images/sprites/soldado_camina.png";
	infoSoldado.fps = 15;
	infoSoldado.descripcion = "Soldado";

	InfoEntidad infoAldeano;
	infoAldeano.tipo = tipos["aldeano"];
	infoAldeano.path = "images/sprites/aldeano_camina.png";
	infoAldeano.fps = 40;
	infoAldeano.descripcion = "Aldeano";

	InfoEntidad infoArquero;
	infoArquero.tipo = tipos["arquero"];
	infoArquero.path = "images/sprites/arquero_camina.png";
	infoArquero.fps = 20;
	infoArquero.descripcion = "Arquero";

	InfoEntidad infoAnimal;
	infoAnimal.tipo = tipos["animal"];
	infoAnimal.path = "images/sprites/vaca_come.png";
	infoAnimal.delay = 5000;
	infoAnimal.fps = 10;
	infoAnimal.descripcion = "Animal";

	InfoEntidad infoCuartel;
	infoCuartel.tipo = tipos["cuartel"];
	infoCuartel.path = "images/utils/Barracks7.png";
	infoCuartel.descripcion = "Cuartel";
	infoCuartel.ancho = 4;
	infoCuartel.alto = 4;

	InfoEntidad infoCuartelConstr;
	infoCuartelConstr.tipo = tipos["construccion_cuartel"];
	infoCuartelConstr.path = "images/utils/Barracks7.png";
	infoCuartelConstr.descripcion = "Construcción de cuartel";
	infoCuartelConstr.ancho = 4;
	infoCuartelConstr.alto = 4;

	InfoEntidad infoCentroCivico;
	infoCentroCivico.tipo = tipos["centro_civico"];
	infoCentroCivico.path = "images/utils/Barracks3.png";
	infoCentroCivico.descripcion = "Centro Civico";
	infoCentroCivico.ancho = 4;
	infoCentroCivico.alto = 4;

	InfoEntidad infoConstruccion;
	infoConstruccion.tipo = tipos["construccion"];
	infoConstruccion.path = "images/utils/Barracks3.png";
	infoConstruccion.descripcion = "Construcción";
	infoConstruccion.ancho = 4;
	infoConstruccion.alto = 4;

	InfoEntidad infoMinaPiedra;
	infoMinaPiedra.tipo = tipos["mina_piedra"];
	infoMinaPiedra.path = "images/utils/mina_piedra2.png";
	infoMinaPiedra.descripcion = "Mina de Piedra";

	InfoEntidad infoMinaOro;
	infoMinaOro.tipo = tipos["mina_oro"];
	infoMinaOro.path = "images/utils/mina_oro.png";
	infoMinaOro.descripcion = "Mina de Oro";

	configDefault.agregarInfoEntidad(infoArbol);
	configDefault.agregarInfoEntidad(infoAgua);
	configDefault.agregarInfoEntidad(infoCuartel);
	configDefault.agregarInfoEntidad(infoCuartelConstr);
	configDefault.agregarInfoEntidad(infoCentroCivico);
	configDefault.agregarInfoEntidad(infoConstruccion);
	configDefault.agregarInfoEntidad(infoSoldado);
	configDefault.agregarInfoEntidad(infoArquero);
	configDefault.agregarInfoEntidad(infoAldeano);
	configDefault.agregarInfoEntidad(infoAnimal);
	configDefault.agregarInfoEntidad(infoBarraca1);
	configDefault.agregarInfoEntidad(infoBarraca1Constr);
//	configDefault.agregarInfoEntidad(infoBarraca2);
//	configDefault.agregarInfoEntidad(infoBarraca3);
	configDefault.agregarInfoEntidad(infoTierra);
	configDefault.agregarInfoEntidad(infoMinaOro);
	configDefault.agregarInfoEntidad(infoMinaPiedra);

	tipos.clear();
	configDefault.agregarInfoEscenarios(infoEscenarioDefault());
	return configDefault;
}

InfoEscenario Yaml::infoEscenarioDefault() {
	InfoEscenario infoEscenario;

	infoEscenario.size_x = 50;
	infoEscenario.size_y = 50;
//	infoEscenario.agregarEntidad(std::make_pair(10,10), AGUA);
//	infoEscenario.agregarEntidad(std::make_pair(11,10), AGUA);
//	infoEscenario.agregarEntidad(std::make_pair(12,10), AGUA);
//	infoEscenario.agregarEntidad(std::make_pair(13,10), AGUA);
//	infoEscenario.agregarEntidad(std::make_pair(10,11), AGUA);
//	infoEscenario.agregarEntidad(std::make_pair(11,11), AGUA);
//	infoEscenario.agregarEntidad(std::make_pair(12,11), AGUA);
//	infoEscenario.agregarEntidad(std::make_pair(13,11), AGUA);
//
//	infoEscenario.agregarEntidad(std::make_pair(4,10), TIERRA);
//	infoEscenario.agregarEntidad(std::make_pair(4,11), TIERRA);
//	infoEscenario.agregarEntidad(std::make_pair(4,12), TIERRA);

//	infoEscenario.agregarEntidad(std::make_pair(4,5), ARBOL);
//	infoEscenario.agregarEntidad(std::make_pair(4,6), ARBOL);
//	infoEscenario.agregarEntidad(std::make_pair(5,5), ARBOL);
//	infoEscenario.agregarEntidad(std::make_pair(4,12), ARBOL);

	infoEscenario.agregarEntidad(std::make_pair(3,0),BARRACK);
	infoEscenario.agregarEntidad(std::make_pair(9,0),CENTRO_CIVICO);
//	infoEscenario.agregarEntidad(std::make_pair(15,0),BARRACK_3);
	infoEscenario.agregarEntidad(std::make_pair(6,7),CUARTEL);

//	infoEscenario.agregarEntidad(std::make_pair(10,14), ANIMAL);

//	infoEscenario.agregarEntidad(std::make_pair(10,12), MINA_PIEDRA);
//	infoEscenario.agregarEntidad(std::make_pair(8,10), MINA_ORO);

	//código de prueba
//	std::cout<<infoEscenario.getPosicionesEntidades().size()<<std::endl;//

	return infoEscenario;
}
