/*
 * Unidad.h
 *
 *  Created on: 9 de nov. de 2015
 *      Author: emanuel
 */

#ifndef MODELO_UNIDAD_H_
#define MODELO_UNIDAD_H_
#include "../modelo/Entidad.h"
#include "../utils/Enumerados.h"
#include "../utils/Constantes.h"

class Unidad: public Entidad {
private:
	int dni;
	bool petrificado;

public:
	Unidad(TipoEntidad tipo, int id_jug);
	bool esUnidad();

	int generarGolpe();
	void lastimar(Entidad* victima);
	bool esRecolector();
	void interactuar();

	void set_id_jugador(int nuevoDuenio);
	void set_identificador(int nuevoDNI);
	int get_identificador();

	bool estaPetrificado();
	void petrificar();
	void despetrificar();

	void cambioEstado(EstadoEntidad est);

	std::string enc();

	static Unidad* dec(std::string);

	virtual ~Unidad();
};

#endif /* MODELO_UNIDAD_H_ */
