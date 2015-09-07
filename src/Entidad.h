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
	TipoEntidad tipo;
	string path;

public:
	Entidad(TipoEntidad tipo);

	/* Devuelve el path de la imagen de la entidad*/
	std::string getPath();

	/* Devuelve el tipo de entidad*/
	TipoEntidad getTipo();

	/* Setea el path de la imagen de la entidad */
	void setPath(std::string path);

	virtual ~Entidad();
};

#endif /* ENTIDAD_H_ */

