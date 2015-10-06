/*
 * Scroll.h
 *
 *  Created on: 1 de oct. de 2015
 *      Author: emanuel
 */

#ifndef VISTA_SCROLL_H_
#define VISTA_SCROLL_H_
#include "../vista/Sprite.h"

class Scroll {
private:
	int *cero_x;
	int *cero_y;
	int width;
	int height;
	int margen_scroll;
	int LIMITE_DESPLAZAMIENTO_EN_X;
	int LIMITE_DESPLAZAMIENTO_EN_Y;

public:
	Scroll(int *cero_x, int *cero_y, int width, int height, int margen);

	void setearLimites(int limite_X, int limite_Y);

	void procesarScroll(int mouseX, int mouseY, Sprite* sprite,
				  float &posPlayerX, float &posPlayerY,
				  int &x_anterior, int &y_anterior);

	virtual ~Scroll();
};

#endif /* VISTA_SCROLL_H_ */
