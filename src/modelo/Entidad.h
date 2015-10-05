/*
 * Entidad.h
 *
 *  Created on: 2 de sept. de 2015
 *      Author: pgfederi
 */

#ifndef MODELO_ENTIDAD_H_
#define MODELO_ENTIDAD_H_

using namespace std;
#include <string>
#include "../utils/Enumerados.h"

class Entidad {
private:
	//std::string tipo;
	TipoEntidad tipo;
	bool movible;

public:
	//Entidad(std::string tipo);
	Entidad(TipoEntidad tipo);

	/* Devuelve el tipo de entidad*/
	//std::string getTipo();
	TipoEntidad getTipo();

	bool esMovible();

	virtual ~Entidad();
};

#endif /* MODELO_ENTIDAD_H_ */

