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
#include "../utils/Calculador.h"
#include "../utils/Exceptions.h"
#include "../modelo/Entidad.h"
#include "../modelo/BarraEstado.h"
#include "../modelo/Escenario.h"
#include "../red/Connection.h"
#include "../red/Client.h"
#include "../vista/Sprite.h"

class Juego {
private:
	int idJug;
	string nombreJug;

	int *cero_x;
	int *cero_y;

	int screenWidth;
	int screenHeight;
	
	vector<InfoEntidad> vectorInfoTiposEntidades;
	EntidadFactory *fabricaDeEntidades;
	
	Escenario *escenario;
	Entidad *protagonista;
	vector<PosEntidad>* enemigos;

	Connection* connection;

	int vel_personaje;
	int margen_scroll;

	/* Carga la configuración inicial del juego a traves
	 * de un archivo de configuración */
	void cargarJuego(InfoEscenario* infoEscRed, Coordenada *posInicial);

	void cargarImagenes();

	int ChequeoDeBorde(int max, int input);

	InfoEscenario parsearConfig();

	InfoEscenario infoEscenarioDefault();
	//de prueba		!!!
	InfoEscenario OdioYAML();

	ContenedorDeRecursos *contenedor;

	BarraEstado *barraEstado;

public:
	Juego(Connection* lan, Coordenada* posInicial, InfoEscenario* infoEscRed);

	Connection* const getConnection();
	void olvidarConnection();
	bool esCliente();

	void setNombreJugador(string nom);
	string getNombreJugador();
	int getIDJugador();
	void cargarNumJugador();
	void cargarEnemigo(PosEntidad posEnt);

	Map<Entidad*, Sprite*>* getSpritesEntidades();
	vector<InfoEntidad> getInfoTiposEntidades();
	Entidad* getProtagonista();
	Sprite* getSpritePlayer();
	Sprite* getSpritePlayer(int idJug);
	Entidad* getEntidadDeSprite(Sprite* spr);
	vector<Sprite*> getSpritesProtagonistas();
	PosEntidad getPosEntDeProtagonista();
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

	void toggleEnemigo(int id_jug);

	void reiniciar();
	virtual ~Juego();
};

#endif /* MODELO_JUEGO_H_ */
