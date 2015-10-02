/*
 * CapaNegra.h
 *
 *  Created on: 1 de oct. de 2015
 *      Author: emanuel
 */

#ifndef CAPANEGRA_H_
#define CAPANEGRA_H_
#include <vector>
using namespace std;

class CapaNegra {
private:
	bool **tiles;
	int tiles_x;
	int tiles_y;
	int tilesOcultos;

	vector<pair<int,int> > generarPuntos(int x, int y);

public:
	CapaNegra(int tiles_x, int tiles_y);

	pair<int,int> getDimension();

	void descubrirDesdePunto(int x, int y);

	bool tileOculto(int x, int y);

	bool totalmenteDescubierta();

	virtual ~CapaNegra();
};

#endif /* CAPANEGRA_H_ */
