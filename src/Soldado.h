/*
 * Soldado.h
 *
 *  Created on: 6 de sept. de 2015
 *      Author: emanuel
 */

#ifndef SOLDADO_H_
#define SOLDADO_H_
#include "Entidad.h"

class Soldado : public Entidad{
private:
	int fps;
	int delay;

public:
	Soldado(std::string tipo);
	void setFps(int fps);
	void setDelay(int delay);
	virtual ~Soldado();
};

#endif /* SOLDADO_H_ */
