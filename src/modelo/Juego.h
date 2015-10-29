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
#include <unistd.h>

#include "../utils/Map.h"
#include "../utils/ContenedorDeRecursos.h"
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

	ConfiguracionJuego configGame;

	EntidadFactory *fabricaDeEntidades;
	
	Escenario *escenario;
	Entidad *protagonista;
	vector<PosEntidad>* enemigos;

	Connection* connection;

	/* Carga la configuración inicial del juego a traves
	 * de un archivo de configuración */
	void cargarJuego(InfoEscenario* infoEscRed, Coordenada *posInicial);

	void cargarImagenes();

	ContenedorDeRecursos *contenedor;

	BarraEstado *barraEstado;

public:
	Juego(Connection* lan, Coordenada* posInicial, InfoEscenario* infoEscRed);

	Connection* const getConnection();
	void olvidarConnection();
	bool esCliente();

	void setNombreJugador(string nom);

	int getRangoDeVision();

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

	void agregarRecurso(TipoEntidad recurso, Coordenada coord);

	void toggleEnemigo(int id_jug);

	void reiniciar();
	virtual ~Juego();
};

#endif /* MODELO_JUEGO_H_ */
