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
#include <SDL2/SDL_mixer.h>

#include "../utils/Map.h"
#include "../utils/ContenedorDeRecursos.h"
#include "../utils/ContenedorDeSonidos.h"
#include "../utils/Exceptions.h"
#include "../modelo/Entidad.h"
#include "../modelo/BarraEstado.h"
#include "../modelo/Escenario.h"
//#include "../red/Connection.h"
//#include "../red/Client.h"
#include "../vista/Sprite.h"
#include "../modelo/Jugador.h"

class Juego {
private:
	int idJug;
	string nombreJug;

	Jugador* jugador;

	int *cero_x;
	int *cero_y;

	ConfiguracionJuego configGame;

	EntidadFactory *fabricaDeEntidades;
	
	Escenario *escenario;

	vector<Unidad*  >* unidadesEnemigos;
	vector<Edificio*>* edificiosEnemigos;

	ContenedorDeSonidos *contenedorSonidos;

	//Connection* connection;

	/* Carga la configuración inicial del juego a traves
	 * de un archivo de configuración */
	void cargarJuego();//InfoEscenario* infoEscRed, Coordenada *posInicial);

	void cargarImagenes();

	ContenedorDeRecursos *contenedor;

	BarraEstado *barraEstado;

	vector<Entidad*> revisarMuertos();

public:
	Juego();//Connection* lan, Coordenada* posInicial, InfoEscenario* infoEscRed);
	void cargaInicialDeRecursos();

	//Connection* const getConnection();
	//void setConnection(Connection* conn);
	//void olvidarConnection();
	//bool esCliente();

	Jugador* getJugador();

	void setNombreJugador(string nom);

	int getRangoDeVision();

	string getNombreJugador();
	int getIDJugador();
	void cargarNumJugador();
	void cargarEnemigo(Entidad* enemigo);

	Map<Entidad*, Sprite*>* getSpritesEntidades();
	vector<InfoEntidad> getInfoTiposEntidades();

	//Sprite* getSpritePlayer();
	Sprite* getSpritePlayer(int idJug, int identificador);

	Sprite* getSpriteDeEntidad(Entidad* entidad);

	Entidad* getEntidadDeSprite(Sprite* spr);

	vector<Sprite*> getSpritesUnidades();

	//PosEntidad getPosEntDeProtagonista();

	Mix_Chunk* getSonidoTipo(TipoSonido tipo);

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

	ContenedorDeRecursos* getContenedorRecursos();

	void comenzarNuevaConstruccion(TipoEntidad tipoEdif, Coordenada coord, int id_jug);
	void crearNuevaUnidad(TipoEntidad tipoUnid, Coordenada coord, int id_jug);
	void agregarRecurso(TipoEntidad recurso, Coordenada coord);

	void reemplazarEntidadPorRecurso(Entidad* entidad, Coordenada coord);

	void toggleEnemigo(int id_jug, int idUnidad);

	void continuar();

	void emitirSonido(Entidad* entidad);

	void reiniciar();
	virtual ~Juego();
};

#endif /* MODELO_JUEGO_H_ */
