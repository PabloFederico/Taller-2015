/*
 * Juego.h
 *
 *  Created on: 6 de sept. de 2015
 *      Author: emanuel
 */

#ifndef MODELO_JUEGO_H_
#define MODELO_JUEGO_H_
#include <stdio.h>
#include <ctime>
#include <map>
#include <yaml-cpp/yaml.h>
#include <unistd.h>

#include "../utils/Map.h"
#include "../modelo/Castillo.h"
#include "../modelo/ConfigDefault.h"
#include "../utils/ContenedorDeRecursos.h"
#include "../modelo/Entidad.h"
#include "../modelo/Escenario.h"
#include "../modelo/Exceptions.h"
#include "../vista/Sprite.h"

class Juego {
private:
	int *cero_x;
	int *cero_y;

	int screenWidth;
	int screenHeight;
	
	vector<InfoEntidad> vectorInfoTiposEntidades;
	
	Escenario *escenario;
	Entidad *protagonista;

	int vel_personaje;
	int margen_scroll;

	/* Carga la configuración inicial del juego a traves
	 * de un archivo de configuración */
	void cargarJuego();

	void cargarImagenes();


	int ChequeoDeBorde(int max, int input);

	InfoEscenario infoEscenarioDefault();

	InfoEscenario parsearConfig();

	//de prueba		!!!
	InfoEscenario OdioYAML();

	ContenedorDeRecursos *contenedor;

public:
	Juego();
	int getVelocidad();
	pair<int,int> dimensionVentana();

	Escenario* getEscenario();

	vector<InfoEntidad> getInfoTiposEntidades();
	//map<std::string,VistaEntidad*>* getMapEntidades();

	Entidad* getProtagonista();

	int getMargenScroll();

	void cargarRecursos(ContenedorDeRecursos *container);

	Map<Entidad*, Sprite*>* getSpritesEntidades();

	Sprite* getSpritePlayer();

	void setCeros(int *x, int *y);

	std::pair<int*,int*> getCeros();

	void reiniciar();

	void actualizarPosicionesEntidades(int cant_x, int cant_y);

	virtual ~Juego();
};

#endif /* MODELO_JUEGO_H_ */
