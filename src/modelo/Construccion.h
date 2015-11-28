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
private:
	TipoEntidad tipoEdif;
	int progresoPorc;
public:
	Construccion(TipoEntidad tipo, int id_jug, int identificador = 0);
	TipoEntidad getTipoEdificio();
	bool esConstruccion();
	void continuarConstruyendo();
	std::string getInfo();
	std::string getVidaString();
	virtual ~Construccion();
};

#endif /* MODELO_CONSTRUCCION_H_ */
