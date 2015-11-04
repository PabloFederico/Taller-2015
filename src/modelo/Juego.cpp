/*
 * Juego.cpp
 *
 *  Created on: 6 de sept. de 2015
 *      Author: emanuel
 */

#include "../modelo/Juego.h"
#include "../modelo/Yaml.h"


Juego::Juego(Connection* lan = NULL, Coordenada* posInicial = NULL, InfoEscenario* infoEscRed = NULL):
			connection(lan) {
	this->idJug = 0;
	this->nombreJug = "JugadorGenerico";
	this->cero_x = NULL;
	this->cero_y = NULL;
	this->contenedor = NULL;
	this->barraEstado = NULL;
	this->escenario = NULL;
	this->fabricaDeEntidades = NULL;
	this->protagonista = NULL;
	this->enemigos = new vector<PosEntidad>;

	this->cargarNumJugador();
	this->cargarJuego(infoEscRed, posInicial);
}

/********************************************************************************/
vector<InfoEntidad> Juego::getInfoTiposEntidades(){
	return configGame.entidades;
}

/********************************************************************************/
void Juego::setNombreJugador(string nom) {
	this->nombreJug = nom;
}

/********************************************************************************/
string Juego::getNombreJugador(){
	return configGame.nombreJugador;
}

/********************************************************************************/
Escenario* Juego::getEscenario(){
	return this->escenario;
}

/********************************************************************************/
Entidad* Juego::getProtagonista(){
	return this->protagonista;
}

/********************************************************************************/
int Juego::getIDJugador() {
	return this->idJug;
}

/********************************************************************************/
void Juego::cargarNumJugador() {
	if (connection != NULL)
		 this->idJug = connection->getIDJugador();
	else this->idJug = 0;
}

/********************************************************************************/
void Juego::cargarJuego(InfoEscenario* infoEscRed = NULL, Coordenada *posInicial = NULL) {
	//----------------------------------------------------------------------------------------!!
	configGame = Yaml::cargarConfiguracionJuego("config.yaml");
	// !!! Para Guido, comentar la línea de arriba y descomentar la de abajo.
	//configGame = Yaml::OdioYAML();
	//----------------------------------------------------------------------------------------!!

	//if esCliente(), receive Escenario; chequear disponibilidad de las entidades, rellenar con missing las faltantes.

	// Acá me imagino la posibilidad de un selector de escenarios.
	if (esCliente()) {
		configGame.vel_personaje = 50; // Misma velocidad para todos.
		// Habría que saber en que escenario estamos
		if (posInicial != NULL)
			configGame.escenarios[0].setPosProtag(*posInicial);
	}
	this->fabricaDeEntidades = new EntidadFactory(this->idJug, configGame.entidades);
	this->escenario = new Escenario(configGame.escenarios[0], this->fabricaDeEntidades, this->enemigos);
	escenario->getCapa()->setRangoDeVision(configGame.rango_vision);
	this->protagonista = this->escenario->getProtagonista();
	this->barraEstado = new BarraEstado(configGame.ancho_pantalla, 150);
}

/********************************************************************************/
int Juego::getRangoDeVision(){
	return configGame.rango_vision;
}

/********************************************************************************/
pair<int,int> Juego::getDimensionVentana(){
	return make_pair(configGame.ancho_pantalla,configGame.alto_pantalla);
}

/********************************************************************************/
int Juego::getMargenScroll(){
	return configGame.margen_scroll;
}

/********************************************************************************/
Coordenada Juego::getCoordCeros() {
	return Coordenada(*getCeros().first + DISTANCIA_ENTRE_X, *getCeros().second);
}

/***************************************************/
int Juego::getVelocidad(){
	return configGame.vel_personaje;
}

/***************************************************/
void Juego::agregarContenedorDeRecursos(ContenedorDeRecursos *container){
	this->contenedor = container;
	this->contenedor->cargarImagenesUtil();
	this->contenedor->cargarImagenesRecursos();
}

/***************************************************/
Sprite* Juego::getSpritePlayer(){
	return this->contenedor->getSpriteDeEntidad(protagonista);
}

/***************************************************/
// Devuelve Sprite protagonista de jugador idJug; en caso de no encontrarlo devuelve el propio.
// Solo para una entidad de adversario; no chequea tipo.
Sprite* Juego::getSpritePlayer(int id_jug) {
	for (vector<PosEntidad>::iterator it = enemigos->begin(); it < enemigos->end(); ++it)
		if (it->entidad->getIDJug() == id_jug)
			return this->contenedor->getSpriteDeEntidad(it->entidad);
	throw NoSeRecibio();
	//return this->contenedor->getSpriteDeEntidad(protagonista);
}

/***************************************************/
vector<Sprite*> Juego::getSpritesProtagonistas() {
	vector<Sprite*> v;
	for (vector<PosEntidad>::iterator it = enemigos->begin(); it < enemigos->end(); ++it)
		v.push_back(this->contenedor->getSpriteDeEntidad(it->entidad)); // ajenos
	v.push_back(this->getSpritePlayer()); // propio
	return v;
}

/***************************************************/
void Juego::setCeros(int *x, int *y){
	this->cero_x = x;
	this->cero_y = y;
}

/***************************************************/
std::pair<int*,int*> Juego::getCeros(){
	return std::make_pair(cero_x,cero_y);
}

/***************************************************/
void Juego::reiniciar(){
	delete this->escenario;
	delete this->contenedor;
	delete this->barraEstado;
	//cargarJuego(NULL);
}

/***************************************************/
void Juego::actualizarPosicionesEntidades(int cant_x, int cant_y){
	this->contenedor->actualizarPosicionesEntidades(cant_x,cant_y);
}

/***************************************************/
Map<Entidad*, Sprite*>* Juego::getSpritesEntidades(){
	return this->contenedor->getMapaSpritesEntidades();
}

/***************************************************/
BarraEstado* Juego::getBarraEstado(){
	return barraEstado;
}

/***************************************************/
bool Juego::esCliente() {
	return (this->connection != NULL);
}

/***************************************************/
Connection* const Juego::getConnection() {
	return this->connection;
}

/***************************************************/
void Juego::setConnection(Connection* conn) {
	this->connection = conn;
}

/***************************************************/
void Juego::olvidarConnection() {
	delete this->connection;
}

/***************************************************/
PosEntidad Juego::getPosEntDeProtagonista() {
	return PosEntidad(this->escenario->getPosProtagonista(), this->protagonista);
}

/***************************************************/
void Juego::agregarRecurso(TipoEntidad recurso, Coordenada coord) {
	Entidad* recurso_a_agregar = new Entidad(recurso); // AGHH MI FACTORYY
	//printf("GENERO UN RECURSO NUEVO %s\n",recurso->getInfo().c_str());
	try {
		escenario->agregarEntidad(coord, recurso_a_agregar);
		//printf("TERMINO EL agregarRecurso, ME VOY \n");
		this->contenedor->generarYGuardarSpriteEntidad(PosEntidad(coord, recurso_a_agregar), Coordenada(*cero_x, *cero_y), escenario);
	} catch ( FueraDeEscenario &e ) {}
}

/***************************************************/
void Juego::cargarEnemigo(PosEntidad posEnt) {
	if (posEnt.entidad->getIDJug() == getIDJugador())
		return;	// Si es el propio, no hacer nada...
	try {
		for (vector<PosEntidad>::iterator it = enemigos->begin(); it < enemigos->end(); ++it)
			if (it->entidad->getIDJug() == posEnt.entidad->getIDJug()) {
				Camino camCorreccion;
				camCorreccion.agregar(posEnt.coord());
				getSpritePlayer(posEnt.entidad->getIDJug())->setearNuevoCamino(camCorreccion, getCoordCeros());
				return;		// Si ya hay una entidad de ese jugador, corrije su posición nomás.
			}

		bool resPosicionar = false;
		int aux = posEnt.x;
		int *d = &posEnt.x;
		// cabeza para resolver situación inicial, se las arregla para el 9# % de los casos
		while (!resPosicionar) {
			try {
				resPosicionar = escenario->agregarEntidad(posEnt.coord(), posEnt.entidad);
				if (!resPosicionar)
					*d += 1;
			} catch ( FueraDeEscenario &e ) {
				if (*d == aux) throw FueraDeEscenario();
				*d = aux;
				d = &posEnt.y;
			}
		}

		enemigos->push_back(posEnt);
		this->contenedor->generarYGuardarSpriteEntidad(posEnt, Coordenada(*cero_x, *cero_y), escenario);
	} catch ( NoSeRecibio &e ) {}
}

/***************************************************/
void Juego::toggleEnemigo(int id_jug) {
	Entidad* entidadEnemigo = NULL;
	for (vector<PosEntidad>::iterator it = enemigos->begin(); it < enemigos->end(); ++it)
		if (it->entidad->getIDJug() == id_jug) {
			entidadEnemigo = it->entidad;
			if (entidadEnemigo->estaPetrificado()) {
				// Si está congelado, lo pasamos a color
				entidadEnemigo->despetrificar();
				std::cout << id_jug<<" despetrificado"<<std::endl;//
			} else {
				// Si no, lo pasamos a gris

				entidadEnemigo->petrificar();
				std::cout << id_jug<<" petrificado"<<std::endl;//
			}
			contenedor->getSpriteDeEntidad(entidadEnemigo)->cambiarTexture();
			break;
		}
}

/***************************************************/
Juego::~Juego() {
	this->enemigos->clear();
	delete this->escenario;
	delete this->contenedor;
	delete this->barraEstado;
}
