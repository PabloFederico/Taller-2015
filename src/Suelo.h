/*
 * Suelo.h
 *
 *  Created on: Sep 15, 2015
 *      Author: martin
 */

#ifndef SUELO_H_
#define SUELO_H_

#include "Entidad.h"

class Suelo: public Entidad {
public:
	Suelo(TipoEntidad tipo);
	virtual ~Suelo();
};

#endif /* SUELO_H_ */
