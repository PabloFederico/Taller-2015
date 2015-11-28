/*
 * ConfigDefault.h
 *
 *  Created on: Sep 19, 2015
 *      Author: martin
 */

#ifndef UTILS_CONFIGDEFAULT_H_
#define UTILS_CONFIGDEFAULT_H_

#include <yaml-cpp/yaml.h>

#include <iostream>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <fstream>


void crearConfigDefault ();


//inline const std::string NombreDeJug(int id_jug);
inline const std::string NombreDeJug(int id_jug) {
	std::map<int,std::string> nombres;
	nombres[1] = "Atila the Hun";
	nombres[2] = "Ramses II";
	nombres[3] = "Henry III";
	nombres[4] = "Cleopatra";
	nombres[5] = "Achilles";
	nombres[6] = "Caesar";
	nombres[7] = "Juana de Arco";
	nombres[8] = "Xerxes";
	nombres[9] = "Hannibal IX";
	nombres[10] = "Death";

	if (id_jug < 10)
		return nombres[id_jug];

	std::ostringstream Encode;
	Encode << "Jaimito "<<id_jug;
	return Encode.str();
}


#endif /* UTILS_CONFIGDEFAULT_H_ */
