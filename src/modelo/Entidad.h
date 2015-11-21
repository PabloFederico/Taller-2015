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
#include <iostream>
#include "../utils/Enumerados.h"
#include "../utils/Exceptions.h"
#include "../utils/Common.h"

//class Coordenada;

//typedef Coordenada Coord;

class Entidad {
protected:
	TipoEntidad tipo;
	int idJug;

	int vidaRestante;
	EstadoEntidad estado;
	bool movible,ocupador;
	int ancho,alto;
	std::string info;
	Entidad* receptor;
	//bool petrificado;
	int x,y;

	clock_t reloj;

public:
	Entidad(TipoEntidad tipo, int num_jug = 0);

	TipoEntidad getTipo();
	EstadoEntidad getEstado();
	int getIDJug();

	void cambioEstado(EstadoEntidad est);
	void finalizaAccion();
	//bool estaPetrificado();
	//void petrificar();
	//void despetrificar();

	void interactuarCon(Entidad* receptor);
	void olvidarInteraccion();
	//void interactuar();

	Coordenada getPosicion();

	void setPosicion(Coordenada c);

	bool esRecurso();
	bool esEdificio();
	bool esUnidad();

	void setTam(int ancho, int alto);
	std::pair<int,int> getTam();

	std::string getInfo();
	std::string getVidaString();

	bool esAtacable();
	bool esMovible();
	bool ocupaSuTile();

	void sufrirGolpe(int fuerza);
	int sufrirRecoleccion();

	std::string enc();
	static Entidad* dec(std::string);

	bool sigueViva();
	void morir();
	virtual ~Entidad();
};

#endif /* MODELO_ENTIDAD_H_ */

