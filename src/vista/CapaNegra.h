/*
 * CapaNegra.h
 *
 *  Created on: 1 de oct. de 2015
 *      Author: emanuel
 */

#ifndef VISTA_CAPANEGRA_H_
#define VISTA_CAPANEGRA_H_
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

#endif /* VISTA_CAPANEGRA_H_ */
