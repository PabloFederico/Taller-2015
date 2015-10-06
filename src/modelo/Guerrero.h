/*
 * Soldado.h
 *
 *  Created on: 6 de sept. de 2015
 *      Author: emanuel
 */

#ifndef MODELO_GUERRERO_H_
#define MODELO_GUERRERO_H_
#include "../modelo/Entidad.h"

class Guerrero : public Entidad{
private:
	int fps;
	int delay;
public:
	Guerrero(TipoEntidad tipo);
	int getFps();
	void setFps(int fps);
	int getDelay();
	void setDelay(int delay);
	virtual ~Guerrero();
};

#endif /* MODELO_GUERRERO_H_ */
