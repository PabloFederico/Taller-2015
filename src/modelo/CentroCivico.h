/*
 * CentroCivico.h
 *
 *  Created on: 9 de nov. de 2015
 *      Author: emanuel
 */

#ifndef MODELO_CENTROCIVICO_H_
#define MODELO_CENTROCIVICO_H_
#include "../modelo/Unidad.h"
#include "../modelo/Edificio.h"

class CentroCivico: public Edificio {
private:
public:
	CentroCivico(int id_jug);

	Unidad* crearNuevaUnidad(TipoEntidad tipo);

	void morir();

	virtual ~CentroCivico();
};

#endif /* MODELO_CENTROCIVICO_H_ */
