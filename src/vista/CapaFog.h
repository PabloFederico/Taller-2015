/*
 * CapaNegra.h
 *
 *  Created on: 1 de oct. de 2015
 *      Author: emanuel
 */

#ifndef VISTA_CAPAFOG_H_
#define VISTA_CAPAFOG_H_
#include <vector>
#include "../utils/Enumerados.h"
using namespace std;

class CapaFog {
private:
	EstadoCapa **tiles;
	int tiles_x;
	int tiles_y;

	vector<pair<int,int> > generarPuntos(int x, int y);

	void actualizarCapaGris();

public:
	CapaFog(int tiles_x, int tiles_y);

	pair<int,int> getDimension();

	void descubrirDesdePunto(int x, int y);

	EstadoCapa getEstadoTile(int x, int y);

	virtual ~CapaFog();
};

#endif /* VISTA_CAPAFOG_H_ */
