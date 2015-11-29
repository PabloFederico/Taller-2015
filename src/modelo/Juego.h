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
#include "../red/Connection.h"
//#include "../red/Client.h"
#include "../red/Proxy.h"
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

	Connection* connection;

	/* Carga la configuración inicial del juego a traves
	 * de un archivo de configuración */
	void cargarJuego(ConfiguracionJuego* infoJuegoRed);
	void generarNuevasUnidadesYEdificiosIniciales();
	void cargarNumJugador();
	void cargarImagenes();

	ContenedorDeRecursos *contenedor;

	BarraEstado *barraEstado;

	vector<Entidad*> revisarMuertos();

	void verificacionDeEstadoYColisionesDeAtaques();

	void actualizarPosicionesAtaquesPorMoverCamara(int cant_x, int cant_y);

	Map<Entidad*,Sprite*> *mapAtaquesLargaDistancia; // Para flechas y ataques de larga distancia

public:
	Juego(Connection* lan, ConfiguracionJuego* infoJuegoRed);
	void cargaInicialDeRecursos();
	void envioInicialDeEntidadesPropias();

	Map<Entidad*,Sprite*> *getMapObjetosAtacantes();
	Connection* const getConnection();
	void setConnection(Connection* conn);
	void olvidarConnection();
	bool esCliente();
	void chat(std::string mensaje);

	Jugador* getJugador();

	void setNombreJugador(string nom);

	int getRangoDeVision();

	string getNombreJugador();
	int getIDJugador();
	void cargarEnemigo(Entidad* enemigo);

	vector<Sprite*> getSpritesEntidadesJugadores();
	Map<Entidad*, Sprite*>* getSpritesEntidades();
	vector<InfoEntidad> getInfoTiposEntidades();

	Entidad* getEntidad(TipoEntidad tipo, int id_jug, int identificador);
	Sprite* getSpriteDeEntidad(Entidad* entidad);
	Entidad* getEntidadDeSprite(Sprite* spr);

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

	Unidad* crearNuevaUnidad(TipoEntidad tipoUnid, Coordenada coord, int id_jug = -1, int id_unidad = 0);
	Construccion* comenzarNuevaConstruccion(TipoEntidad tipoEdif, Coordenada coord, int id_jug = -1, int id_edificio = 0);
	Edificio* crearNuevoEdificio(TipoEntidad tipoEdif, Coordenada coord, int id_jug = -1, int id_edificio = 0);
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

	void apagarEnemigo(int id_jug);
	void reiniciar();
	virtual ~Juego();
};

#endif /* MODELO_JUEGO_H_ */
