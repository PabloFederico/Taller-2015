/*
 * Calculador.h
 *
 *  Created on: 16 de sept. de 2015
 *      Author: emanuel
 */

#ifndef CALCULADOR_H_
#define CALCULADOR_H_
#include <map>

class Calculador {
private:
	int cero_x;
	int cero_y;
	int escala_x;
	int escala_y;

public:
	Calculador(int cero_x, int cero_y, int escala_x, int escala_y);

	double calcularDistancia(int X1, int Y1, int X2, int Y2);

	std::pair<int,int> calcularPosicionRelativa(int x, int y);

	virtual ~Calculador();
};

#endif /* CALCULADOR_H_ */
