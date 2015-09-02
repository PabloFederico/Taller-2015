/*
 * Entidad.h
 *
 *  Created on: 2 de sept. de 2015
 *      Author: pgfederi
 */

#ifndef ENTIDAD_H_
#define ENTIDAD_H_

#include <string>
using namespace std;


class Entidad {

	private:
		string nombre;
		string imagen_path;
		float fps;
		float delay;
		int ancho_base;
		int alto_base;
		float pixel_ref_x;
		float pixel_ref_y;


	public:
		Entidad();
		virtual ~Entidad();
};

#endif /* ENTIDAD_H_ */
