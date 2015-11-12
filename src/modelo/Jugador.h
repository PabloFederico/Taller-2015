/*
 * Jugador.h
 *
 *  Created on: 9 de nov. de 2015
 *      Author: emanuel
 */

#ifndef MODELO_JUGADOR_H_
#define MODELO_JUGADOR_H_
#include "../modelo/CentroCivico.h"
#include <map>
#include <vector>

class Jugador {
private:
	std::string nombre;
	int id_jug;
	int contador_dni_unidades;
	int contador_dni_edificios;
	CentroCivico* centroCivico;
	std::map<int,Unidad* > unidades;
	std::vector<Unidad*> vec_unidades;
	std::map<int,Edificio*> edificios;
	Unidad* unidadActiva;

public:
	Jugador(std::string nombre, int id_jug);

	std::vector<Unidad*> getUnidades();

	Unidad* getUnidadActiva();

	void agregarCentroCivico(CentroCivico* centro);

	void agregarNuevaUnidad(Unidad* unidad);

	void agregarNuevoEdificio(Edificio* edificio);

	CentroCivico* getCentroCivico();

	virtual ~Jugador();
};

#endif /* MODELO_JUGADOR_H_ */
