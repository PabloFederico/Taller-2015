/*
 * Camara.h
 *
 *  Created on: 7 de oct. de 2015
 *      Author: emanuel
 */

#ifndef VISTA_CAMARA_H_
#define VISTA_CAMARA_H_
#include <map>

class Camara {
private:
	int *cero_x;
	int *cero_y;
	int margen_scroll;
	int width;
	int height;
	int limite_X;
	int limite_Y;

public:
	Camara(int *x, int *y);

	void mover(int cant_x, int cant_y);

	void setLimites(int x, int y);

	void setMargenScrolling(int margen);

	int getMargenScrolling();

	void setDimension(int width, int height);

	std::pair<int,int> getDimension();

	std::pair<int,int> getLimites();

	virtual ~Camara();
};

#endif /* VISTA_CAMARA_H_ */
