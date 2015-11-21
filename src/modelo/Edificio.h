/*
 * Edificio.h
 *
 *  Created on: 9 de nov. de 2015
 *      Author: emanuel
 */

#ifndef MODELO_EDIFICIO_H_
#define MODELO_EDIFICIO_H_
#include "../modelo/Entidad.h"

class Edificio: public Entidad {
protected:
	int dni;

public:
	Edificio(TipoEntidad tipo, int id_jug, int dni = 0);

	void set_id_jugador(int nuevoDuenio);

	void set_identificador(int nuevoDNI);
	int get_identificador();

	bool esEdificio();

	std::string enc();
	Edificio* dec(std::string s);

	void morir();
	virtual ~Edificio();
};

#endif /* MODELO_EDIFICIO_H_ */
