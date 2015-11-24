/*
 * BarraInfo.h
 *
 *  Created on: 21 de oct. de 2015
 *      Author: emanuel
 */

#ifndef MODELO_BARRAESTADO_H_
#define MODELO_BARRAESTADO_H_
#include <map>
#include "../utils/Enumerados.h"
#include "../modelo/Jugador.h"

class BarraEstado {
private:
	int height;
	int width;
	std::string infoDescripcion;
	Jugador* jugador;

public:
	BarraEstado(int ancho, int alto, Jugador* jug);

	std::pair<int,int> getDimension();

	std::string getDescripcion();

	Entidad* getEntidadActualEnBarra();

	int getIDJugador();

	std::map<TipoEntidad,int> getMapRecursosEconomicos();

	void setInformacion(std::string infoDescripcion);

	virtual ~BarraEstado();
};

#endif /* MODELO_BARRAESTADO_H_ */
