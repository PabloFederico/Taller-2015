/*
 * Edificio.h
 *
 *  Created on: 9 de nov. de 2015
 *      Author: emanuel
 */

#ifndef MODELO_EDIFICIO_H_
#define MODELO_EDIFICIO_H_
#include "../modelo/Entidad.h"

const int COSTO_ALDEANO = 10;
const int COSTO_ARQUERO = 10;
const int COSTO_SOLDADO = 10;

class Edificio: public Entidad {

public:
	Edificio(TipoEntidad tipo, int id_jug, int dni = 0);

	void set_id_jugador(int nuevoDuenio);

	void set_identificador(int nuevoDNI);
	int get_identificador();

	int getCostoPorUnidad();

	bool esEdificio();

	void morir();
	virtual ~Edificio();
};

#endif /* MODELO_EDIFICIO_H_ */
