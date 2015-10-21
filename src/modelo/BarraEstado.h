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

class BarraEstado {
private:
	int height;
	int width;
	std::map<TipoRecurso, int> mapRecursosEconomicos;

public:
	BarraEstado(int ancho, int alto);

	void agregarRecursoEconomico(TipoRecurso tipo);

	std::pair<int,int> getDimension();

	std::map<TipoRecurso, int> getRecursosEconomicos();

	virtual ~BarraEstado();
};

#endif /* MODELO_BARRAESTADO_H_ */
