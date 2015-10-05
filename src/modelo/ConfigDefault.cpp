/*
 * ConfigDefault.cpp
 *
 *  Created on: Sep 19, 2015
 *      Author: martin
 */

#include "../modelo/ConfigDefault.h"


void crearConfigDefault () {
	std::vector< std::map<std::string,std::string> > vec;

	std::map<std::string, std::string> arbol;
	arbol.insert(std::make_pair("nombre", "arbol"));
	arbol.insert(std::make_pair("imagen", "images/arbol.png"));
	vec.push_back(arbol);

	std::map<std::string, std::string> castillo;
	castillo.insert(std::make_pair("nombre", "castillo"));
	castillo.insert(std::make_pair("imagen", "images/castle.png"));
	castillo.insert(std::make_pair("ancho_base", "2"));
	castillo.insert(std::make_pair("alto_base", "2"));
	castillo.insert(std::make_pair("pixel_ref_x", "30"));
	castillo.insert(std::make_pair("pixel_ref_y", "40"));
	vec.push_back(castillo);

	std::map<std::string, std::string> soldado;
	soldado.insert(std::make_pair("nombre", "soldado"));
	soldado.insert(std::make_pair("imagen", "images/soldado.png"));
	soldado.insert(std::make_pair("fps", "10"));
	soldado.insert(std::make_pair("delay", "0"));
	vec.push_back(soldado);

	std::map<std::string, std::string> juana_de_arco;
	juana_de_arco.insert(std::make_pair("nombre", "juana_de_arco"));
	juana_de_arco.insert(std::make_pair("imagen", "images/juana.png"));
	juana_de_arco.insert(std::make_pair("fps", "10"));
	vec.push_back(juana_de_arco);

	std::map<std::string, std::string> tierra;
	tierra.insert(std::make_pair("nombre", "tierra"));
	tierra.insert(std::make_pair("imagen", "images/tierra.png"));
	vec.push_back(tierra);

	std::map<std::string, std::string> agua;
	agua.insert(std::make_pair("nombre", "agua"));
	agua.insert(std::make_pair("imagen", "images/agua.png"));
	vec.push_back(agua);


	YAML::Emitter out;
	//out.SetIndent(5);

	out << YAML::BeginMap;

		// pantalla
		out << YAML::Key << "pantalla";
		out << YAML::Value << YAML::BeginMap;
			out << YAML::Key << "ancho" << YAML::Value << 800;
			out << YAML::Key << "alto" 	<< YAML::Value << 600;
		out << YAML::EndMap << YAML::Newline << YAML::Newline;

		// configuracion
		out << YAML::Key << "configuracion";
		out << YAML::Value << YAML::BeginMap;
			out << YAML::Key << "vel_personaje" << YAML::Value << 3;
			out << YAML::Key << "margen_scroll" << YAML::Value << 30;
		out << YAML::EndMap << YAML::Newline << YAML::Newline;

		// tipos
		out << YAML::Key << "tipos";
		out << YAML::Value << YAML::BeginSeq;

			// Imprimir entidades
			std::vector< std::map<std::string,std::string> >::iterator it;
			for (it = vec.begin(); it < vec.end(); ++it) {
				out << YAML::BeginMap;
				std::map<std::string,std::string>::iterator p = it->find("nombre");
				out << YAML::Key << p->first << YAML::Value << p->second;

				for (p = it->begin(); p != it->end(); p++) {
					if (p->first != "nombre")
						out << YAML::Key << p->first << YAML::Value << p->second;
				}

				out << YAML::EndMap << YAML::Newline;
			}

		out << YAML::EndSeq << YAML::Newline;

		// escenario(s)
		out << YAML::Key << "escenario";
		out << YAML::Value << YAML::BeginSeq;

			out << YAML::BeginMap;
				out << YAML::Key << "nombre" << YAML::Value << "orleans";
				out << YAML::Key << "size_x" << YAML::Value << 100;
				out << YAML::Key << "size_y" << YAML::Value << 100;
				out << YAML::Key << "entidades";
				out << YAML::Value << YAML::BeginSeq;
					out << YAML::Load("{x: 10, y: 15, tipo: tierra}");
					out << YAML::Load("{x: 10, y: 20, tipo: tierra}");
					out << YAML::Load("{x: 10, y: 15, tipo: agua}");
					out << YAML::Load("{x: 30, y: 55, tipo: castillo}");
				out << YAML::EndSeq;
				// Único protagonista
				out << YAML::Key << "protagonista";
				out << YAML::Value << YAML::BeginSeq << YAML::BeginMap;
					out << YAML::Key << "tipo" << YAML::Value << "juana_de_arco";
					out << YAML::Key << "x" << YAML::Value << 50;
					out << YAML::Key << "y" << YAML::Value << 50;
				out << YAML::EndMap << YAML::EndSeq;
			out << YAML::EndMap;

		out << YAML::EndSeq;

	out << YAML::EndMap;

	std::ofstream fout("config.yaml");
	fout << out.c_str();
	fout.close();
}
