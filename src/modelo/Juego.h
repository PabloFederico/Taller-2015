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

	vector<Unidad*>*   unidadesEnemigos;
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

	vector<Sprite*> getSpritesUnidades();
	Map<Entidad*, Sprite*>* getSpritesEntidades();
	vector<InfoEntidad> getInfoTiposEntidades();

	Entidad* getEntidad(TipoEntidad tipo, int id_jug, int identificador);
	Sprite* getSpriteDeEntidad(Entidad* entidad);
	Entidad* getEntidadDeSprite(Sprite* spr);

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

	Unidad* crearNuevaUnidad(TipoEntidad tipoUnid, Coordenada coord, int id_jug, int id_unidad = 0);
	Construccion* comenzarNuevaConstruccion(TipoEntidad tipoEdif, Coordenada coord, int id_jug, int id_edificio = 0);
	Edificio* crearNuevoEdificio(TipoEntidad tipoEdif, Coordenada coord, int id_jug, int id_edificio = 0);
	Entidad* agregarRecurso(TipoEntidad recurso, Coordenada coord, int id_recurso = 0);

	void iniciarInteraccionEntre(Entidad* ejecutor, Entidad* receptor);
	void iniciarInteraccionEntre(TipoEntidad tipo_ejecutor, int idJug_ejecutor, int dni_ejecutor,
								 TipoEntidad tipo_receptor, int idJug_receptor, int dni_receptor);

	void ejecutoresOlvidarInteraccionCon(Entidad* muerto);
	void reemplazarEntidadPorRecurso(Entidad* entidad);

	Edificio* terminarConstruccion(ConstruccionTermino c);
	void interaccionesDeUnidades();
	void continuar();

	void emitirSonido(Entidad* entidad);

	void crearNuevaUnidadApartirDeEdificioSeleccionado(TipoEntidad tipoEntidadACrear);

	//void toggleEnemigo(int id_jug, int idUnidad);
	void reiniciar();
	virtual ~Juego();
};

#endif /* MODELO_JUEGO_H_ */
