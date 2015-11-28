/*
 * Jugador.h
 *
 *  Created on: 9 de nov. de 2015
 *      Author: emanuel
 */

#ifndef MODELO_JUGADOR_H_
#define MODELO_JUGADOR_H_
#include "../modelo/Escenario.h"
#include "../utils/ContenedorDeRecursos.h"
#include <map>
#include <vector>

class Jugador {
private:
	std::string nombre;
	int id_jug;

	std::map<TipoEntidad,int> mapRecursosEconomicos;

	vector<Unidad*> unidadesSeleccionadas;
	Edificio* edificioSeleccionado;
	Unidad* unidadActiva;

	int contador_dni_unidades;
	int contador_dni_edificios;
	std::vector<Unidad*> vec_unidades;
	std::map<int,Unidad* > unidades;
	std::map<int,Edificio*> edificios;

public:
	Jugador(std::string nombre, int id_jug);

	std::vector<Unidad*> getUnidades();
	std::vector<Edificio*> getEdificios();

	Unidad* getUnidadActiva();

	int getID();

	int getRecursosDisponibles();

	void descontarRecursos(int cant);

	void agregarEntidadSeleccionada(Entidad* entidad);

	Entidad* getEntidad(TipoEntidad tipo, int identificador);
/*
	void agregarUnidadSeleccionada(Unidad* unidad);

	void agregarEdificioSeleccionado(Edificio* edificio);
*/
	vector<Unidad*> getUnidadesSeleccionadas();

	Edificio* getEdificioSeleccionado();
	void liberarUnidadesSeleccionadas();
	void liberarEdificioSeleccionado();

	void agregarNuevaUnidad(Unidad* unidad);
	void agregarNuevoEdificio(Edificio* edificio);
	void guardarConstruccionTerminada(Edificio* edifRecienRecienInaugurado);
	void agregarRecursoEconomico(TipoEntidad tipo, int cant);

	std::map<TipoEntidad,int> getMapRecursosEconomicos();

	void ejecutoresOlvidarInteraccionCon(Entidad* muerto);
	void limpiarSeleccionDeUnidadMuerta(Unidad* moribundo);
	vector<Entidad*> revisarMuertosPropios();

	virtual ~Jugador();
};

#endif /* MODELO_JUGADOR_H_ */
