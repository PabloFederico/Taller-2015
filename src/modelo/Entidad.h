/*
 * Entidad.h
 *
 *  Created on: 2 de sept. de 2015
 *      Author: cosmefulanito
 */

#ifndef MODELO_ENTIDAD_H_
#define MODELO_ENTIDAD_H_

using namespace std;
#include <string>
#include <sstream>
#include <iostream>
#include "../utils/Constantes.h"
#include "../utils/Enumerados.h"
#include "../utils/Exceptions.h"
#include "../utils/Common.h"


class Entidad {
protected:
	TipoEntidad tipo;
	int idJug;
	int dni;

	int vidaRestante, armadura, ataque;
	EstadoEntidad estado;
	bool movible,ocupador;
	int ancho,alto;
	std::string info;
	Entidad* receptor;
	Coordenada c; // Pos de la entidad
	Coordenada* coordMasProximaDelReceptor;

	clock_t reloj;

public:
	Entidad(TipoEntidad tipo, int num_jug = 0, int identificador = 0);

	TipoEntidad getTipo();
	EstadoEntidad getEstado();
	int getVidaRestante();
	void setVidaRestante(int valor);
	int getIDJug();
	int get_identificador();
	bool perteneceAJugador(int id_jug);

	virtual void cambioEstado(EstadoEntidad est);
	void finalizaAccion();

	void interactuarCon(Entidad* receptor);
	void virtual olvidarInteraccion();
	//void interactuar();

	Coordenada getPosicion();
	void setPosicion(Coordenada c);

	bool esRecolector();
	bool esConstructor();

	bool esRecurso();
	bool esConstruccion();
	bool esEdificio();
	bool esUnidad();

	void setTam(int ancho, int alto);
	std::pair<int,int> getTam();

	std::string getInfo();
	std::string getVidaString();
	std::string getIDJugYVidaString();

	bool esAtacable();
	bool esMovible();
	bool ocupaSuTile();

	void sufrirGolpe(int fuerza);
	int sufrirRecoleccion(int cant);
	int obtenerArmor();
	int obtenerAtk();

	std::string enc();
	static Entidad dec(std::string);

	bool sigueViva();
	void morir();
	virtual ~Entidad();
};

#endif /* MODELO_ENTIDAD_H_ */

