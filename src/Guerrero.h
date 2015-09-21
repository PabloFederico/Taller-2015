/*
 * Soldado.h
 *
 *  Created on: 6 de sept. de 2015
 *      Author: emanuel
 */

#ifndef GUERRERO_H_
#define GUERRERO_H_
#include "Entidad.h"

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

#endif /* GUERRERO_H_ */
