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
#include "../modelo/Castillo.h"
#include "../modelo/ConfigDefault.h"
#include "../utils/ContenedorDeRecursos.h"
#include "../modelo/Entidad.h"
#include "../modelo/BarraEstado.h"
#include "../modelo/Escenario.h"
#include "../modelo/Exceptions.h"
#include "../red/Connection.h"
#include "../red/Server.h"
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

	Connection* connection;

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

	BarraEstado *barraEstado;

public:
	Juego(Connection* lan);
	int getVelocidad();
	pair<int,int> dimensionVentana();

	Escenario* getEscenario();

	bool esCliente();

	vector<InfoEntidad> getInfoTiposEntidades();
	//map<std::string,VistaEntidad*>* getMapEntidades();

	Camino recibirCamino(); //cambiar nombre por uno mejor
	void enviar(Camino cam);

	Entidad* getProtagonista();

	int getMargenScroll();

	Coordenada getCoordCeros();

	void agregarContenedorDeRecursos(ContenedorDeRecursos *container);

	Map<Entidad*, Sprite*>* getSpritesEntidades();

	Sprite* getSpritePlayer();

	void setCeros(int *x, int *y);

	std::pair<int*,int*> getCeros();

	void reiniciar();

	void actualizarPosicionesEntidades(int cant_x, int cant_y);

	/* El nombre del método lo dice \TODO\ */
	void generarRecursosAleatoriosParaElEscenario();

	BarraEstado* getBarraEstado();

	Connection* const getConnection();

	void multiplayer();

	virtual ~Juego();
};

#endif /* MODELO_JUEGO_H_ */
