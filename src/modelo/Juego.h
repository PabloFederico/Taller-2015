/*
 * Juego.h
 *
 *  Created on: 6 de sept. de 2015
 *      Author: emanuel
 */

#ifndef MODELO_JUEGO_H_
#define MODELO_JUEGO_H_
#include <stdio.h>
#include <map>
#include <yaml-cpp/yaml.h>
#include <unistd.h>

#include "../utils/Map.h"
#include "../modelo/ConfigDefault.h"
#include "../utils/ContenedorDeRecursos.h"
#include "../modelo/Entidad.h"
#include "../modelo/BarraEstado.h"
#include "../modelo/Escenario.h"
#include "../modelo/Exceptions.h"
#include "../red/Connection.h"
#include "../red/Client.h"
#include "../vista/Sprite.h"

class Juego {
private:
	int *cero_x;
	int *cero_y;

	int screenWidth;
	int screenHeight;
	
	vector<InfoEntidad> vectorInfoTiposEntidades;
	EntidadFactory *fabricaDeEntidades;
	
	Escenario *escenario;
	Entidad *protagonista;
	vector<Entidad*> enemigos;

	Connection* connection;

	int vel_personaje;
	int margen_scroll;

	/* Carga la configuración inicial del juego a traves
	 * de un archivo de configuración */
	void cargarJuego(InfoEscenario* infoEscRed);

	void cargarImagenes();


	int ChequeoDeBorde(int max, int input);

	InfoEscenario infoEscenarioDefault();

	InfoEscenario parsearConfig();

	//de prueba		!!!
	InfoEscenario OdioYAML();

	ContenedorDeRecursos *contenedor;

	BarraEstado *barraEstado;

public:
	Juego(Connection* lan, InfoEscenario* infoEscRed);

	Connection* const getConnection();
	bool esCliente();

	void cargarEnemigo(Entidad* ent);

	Map<Entidad*, Sprite*>* getSpritesEntidades();
	vector<InfoEntidad> getInfoTiposEntidades();
	Entidad* getProtagonista();
	Sprite* getSpritePlayer();
	pair<int,int> getDimensionVentana();
	int getVelocidad();
	int getMargenScroll();
	BarraEstado* getBarraEstado();
	Escenario* getEscenario();

	void setCeros(int *x, int *y);
	Coordenada getCoordCeros();
	std::pair<int*,int*> getCeros();

	void actualizarPosicionesEntidades(int cant_x, int cant_y);

	void agregarContenedorDeRecursos(ContenedorDeRecursos *container);

	/* El nombre del método lo dice \TODO\ */
	void generarRecursosAleatoriosParaElEscenario();

	void reiniciar();
	virtual ~Juego();
};

#endif /* MODELO_JUEGO_H_ */
