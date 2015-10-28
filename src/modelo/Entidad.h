/*
 * Entidad.h
 *
 *  Created on: 2 de sept. de 2015
 *      Author: pgfederi
 */

#ifndef MODELO_ENTIDAD_H_
#define MODELO_ENTIDAD_H_

using namespace std;
#include <string>
#include <sstream>
#include "../utils/Enumerados.h"

class Entidad {
private:
	TipoEntidad tipo;
	int idJug;
	bool movible,ocupador;
	int ancho,alto;
	std::string info;
	bool petrificado;

public:
	Entidad(TipoEntidad tipo, int num_jug = 0);

	TipoEntidad getTipo();
	int getIDJug();

	bool estaPetrificado();
	void petrificar();
	void despetrificar();

	void setTam(int ancho, int alto);
	std::pair<int,int> getTam();

	std::string getInfo();

	bool esMovible();
	bool ocupaSuTile();

	std::string enc();
	static Entidad* dec(std::string);

	virtual ~Entidad();
};

#endif /* MODELO_ENTIDAD_H_ */

