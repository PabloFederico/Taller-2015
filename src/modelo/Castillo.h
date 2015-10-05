/*
 * Castillo.h
 *
 *  Created on: 6 de sept. de 2015
 *      Author: emanuel
 */

#ifndef MODELO_CASTILLO_H_
#define MODELO_CASTILLO_H_
#include <map>
#include "../modelo/Entidad.h"

class Castillo : public Entidad{
/*private:
	int ancho_base;
	int alto_base;
	int pixel_ref_x;
	int pixel_ref_y;
*/

public:
/*	Castillo(std::string tipo);

	void setDimension(int ancho, int alto);

	std::pair<int,int> getDimension();

	std::pair<int,int> getPixelRef();

	void setPixelRef(int ref_x, int ref_y);
*/
	Castillo(TipoEntidad tipo);

	virtual ~Castillo();
};

#endif /* MODELO_CASTILLO_H_ */
