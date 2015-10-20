/*
 * EntidadFactory.h
 *
 *  Created on: Oct 16, 2015
 *      Author: martin
 */

#ifndef MODELO_ENTIDADFACTORY_H_
#define MODELO_ENTIDADFACTORY_H_

#include "../utils/Enumerados.h"
#include "../utils/Structs.h"
#include "../modelo/Entidad.h"
#include <vector>
#include <algorithm>


class EntidadFactory {

private:
	vector<InfoEntidad> vInfoEntidades;

public:
	EntidadFactory(std::vector<InfoEntidad> v);
	Entidad* nuevaEntidad(TipoEntidad tipo);
	virtual ~EntidadFactory();

};

#endif /* MODELO_ENTIDADFACTORY_H_ */
