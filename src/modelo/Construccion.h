/*
 * Construccion.h
 *
 *  Created on: Nov 21, 2015
 *      Author: martin
 */

#ifndef MODELO_CONSTRUCCION_H_
#define MODELO_CONSTRUCCION_H_

#include "Edificio.h"

class Construccion: public Edificio {
public:
	Construccion(int id_jug);
	virtual ~Construccion();
};

#endif /* MODELO_CONSTRUCCION_H_ */
