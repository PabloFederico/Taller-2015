/*
 * Flecha.h
 *
 *  Created on: 28 de nov. de 2015
 *      Author: emanuel
 */

#ifndef MODELO_FLECHA_H_
#define MODELO_FLECHA_H_
#include "Unidad.h"
#include "../utils/Direccion.h"

class Flecha: public Entidad {
private:
	Unidad* emisor;
	Coordenada origen;
	Direccion direccion;

public:
	Flecha(Unidad* emisor, Direccion direccion);

	int getIdEmisor();

	Direccion getDireccion();

	bool llegoASuLimite();

	void lastimar(Entidad* receptor);

	virtual ~Flecha();
};

#endif /* MODELO_FLECHA_H_ */
