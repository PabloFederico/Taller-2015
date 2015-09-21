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

enum TipoEntidad  { DEFAULT, TIERRA , AGUA , ARBOL , CASTILLO, SOLDADO, JUANA_DE_ARCO };

struct InfoEntidad{
	TipoEntidad tipo;
	string path;
	int ancho;
	int alto;
	int pixel_ref_x;
	int pixel_ref_y;
	int fps;
	int delay;
	InfoEntidad(){
		tipo = DEFAULT;
		path = "";
		ancho = 1;
		alto = 1;
		pixel_ref_x = 0;
		pixel_ref_y = 0;
		fps = 10;
		delay = 0;
	};
};

class Entidad {
private:
	//std::string tipo;
	TipoEntidad tipo;

public:
	//Entidad(std::string tipo);
	Entidad(TipoEntidad tipo);

	/* Devuelve el tipo de entidad*/
	//std::string getTipo();
	TipoEntidad getTipo();

	virtual ~Entidad();
};

#endif /* ENTIDAD_H_ */

