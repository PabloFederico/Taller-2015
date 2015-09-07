/*
 * Castillo.h
 *
 *  Created on: 6 de sept. de 2015
 *      Author: emanuel
 */

#ifndef CASTILLO_H_
#define CASTILLO_H_
#include "Entidad.h"
#include <map>

class Castillo : public Entidad{
private:
	int ancho_base;
	int alto_base;
	int pixel_ref_x;
	int pixel_ref_y;

public:
	Castillo(TipoEntidad tipo);

	void setDimension(int ancho, int alto);

	std::pair<int,int> getDimension();

	std::pair<int,int> getPixelRef();

	void setPixelRef(int ref_x, int ref_y);

	virtual ~Castillo();
};

#endif /* CASTILLO_H_ */
