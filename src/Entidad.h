/*
 * Entidad.h
 *
 *  Created on: 2 de sept. de 2015
 *      Author: pgfederi
 */

#ifndef ENTIDAD_H_
#define ENTIDAD_H_
using namespace std;
#include <string>

enum TipoEntidad  { TIERRA , AGUA , ARBOL , CASTILLO, SOLDADO };

class Entidad {
private:
	std::string tipo;

public:
	Entidad(std::string tipo);

	/* Devuelve el tipo de entidad*/
	std::string getTipo();

	virtual ~Entidad();
};

#endif /* ENTIDAD_H_ */

