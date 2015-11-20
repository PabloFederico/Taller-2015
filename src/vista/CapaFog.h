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

	int rango_de_vision;

	vector<pair<int,int> > generarPuntos(int x, int y);

public:
	CapaFog(int tiles_x, int tiles_y);

	pair<int,int> getDimension();

	void actualizarCapaGris();

	void descubrirDesdePunto(int x, int y);

	EstadoCapa getEstadoTile(int x, int y);

	void setRangoDeVision(int rango);

	virtual ~CapaFog();
};

#endif /* VISTA_CAPAFOG_H_ */
