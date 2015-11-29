/*
 * Unidad.h
 *
 *  Created on: 9 de nov. de 2015
 *      Author: emanuel
 */

#ifndef MODELO_UNIDAD_H_
#define MODELO_UNIDAD_H_
#include "../modelo/Entidad.h"
#include "../modelo/Construccion.h"
#include "../utils/Enumerados.h"
#include "../utils/Constantes.h"

class Unidad: public Entidad {
protected:
	int rangoAccion;
	InfoAtaque infoAtaque;
	//bool petrificado;

public:
	Unidad(TipoEntidad tipo, int id_jug, int dni = 0);
	bool esUnidad();

	void setCoordMasProximaDelReceptor();
	bool estaEnRangoDelReceptor();

	void continuarConstruccion();
	int generarGolpe();
	void lastimar(Entidad* victima);
	int recolectar(Entidad* recurso);
	bool esRecolector();
	bool esConstructor();
	void interactuar();
	void olvidarInteraccion();

	void olvidarReceptorSiEs(Entidad* entidad);

	void set_id_jugador(int nuevoDuenio);
	void set_identificador(int nuevoDNI);
	int get_identificador();
	int getRangoAccion();

	bool emitioAtaque();

	//bool estaPetrificado();
	//void petrificar();
	//void despetrificar();

	//void olvidarInteraccion();

	void cambioEstado(EstadoEntidad est);

	std::string enc();
	static Unidad* dec(std::string);

	void morir();
	virtual ~Unidad();
};

#endif /* MODELO_UNIDAD_H_ */
