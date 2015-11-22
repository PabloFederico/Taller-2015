/*
 * Juego.cpp
 *
 *  Created on: 6 de sept. de 2015
 *      Author: emanuel
 */

#include "../modelo/Juego.h"
#include "../modelo/Yaml.h"


//Juego::Juego(Connection* lan = NULL, Coordenada* posInicial = NULL, InfoEscenario* infoEscRed = NULL):
//			connection(lan) {
Juego::Juego(){
	this->idJug = 1; /* esta hardcodeado para probar (sin cliente-servidor) */
	this->nombreJug = "JugadorGenerico";
	this->cero_x = NULL;
	this->cero_y = NULL;
	this->contenedor = NULL;
	this->barraEstado = NULL;
	this->escenario = NULL;
	this->fabricaDeEntidades = NULL;
	this->enemigos = new vector<Unidad*>();
	this->jugador = NULL;
	this->contenedorSonidos = NULL;
	this->cargarNumJugador();
	this->cargarJuego();//infoEscRed, posInicial);

}

/********************************************************************************/
vector<InfoEntidad> Juego::getInfoTiposEntidades(){
	return configGame.entidades;
}

/********************************************************************************/
Jugador* Juego::getJugador(){
	return jugador;
}
/********************************************************************************/
void Juego::setNombreJugador(string nom) {
	this->nombreJug = nom;
	configGame.nombreJugador = nom;
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
int Juego::getIDJugador() {
	return this->idJug;
}

/********************************************************************************/
void Juego::cargarNumJugador() {
	/*
	if (connection != NULL)
		 this->idJug = connection->getIDJugador();
	else this->idJug = 0;
	*/
}

/********************************************************************************/
void Juego::cargarJuego(){//InfoEscenario* infoEscRed = NULL, Coordenada *posInicial = NULL) {
	//----------------------------------------------------------------------------------------!!
	configGame = Yaml::cargarConfiguracionJuego("config.yaml");
	// !!! Para Guido, comentar la línea de arriba y descomentar la de abajo.
	//configGame = Yaml::OdioYAML();
	//----------------------------------------------------------------------------------------!!

	//if esCliente(), receive Escenario; chequear disponibilidad de las entidades, rellenar con missing las faltantes.

	// Acá me imagino la posibilidad de un selector de escenarios.
	/*if (esCliente()) {
		configGame.vel_personaje = 50; // Misma velocidad para todos.
		// Habría que saber en que escenario estamos
		if (posInicial != NULL)
			configGame.escenarios[0].setPosProtag(*posInicial);
	}*/
	this->jugador = new Jugador(configGame.nombreJugador,idJug);
	this->fabricaDeEntidades = new EntidadFactory(this->idJug, configGame.entidades);
	this->escenario = new Escenario(configGame.escenarios[0], this->fabricaDeEntidades, this->enemigos);

	/* Las siguientes entidades son de prueba*/	
	Unidad* soldado = new Unidad(SOLDADO,1);
	Unidad* aldeano = new Unidad(ALDEANO,1);
	Unidad* arquero = new Unidad(ARQUERO,1);
	Unidad* aldeanoDesconocido = new Unidad(ALDEANO,2);
	Unidad* arqueroDesconocido = new Unidad(ARQUERO,2);
	soldado->setPosicion(Coordenada(20,20));
	aldeano->setPosicion(Coordenada(17,17));
	arquero->setPosicion(Coordenada(16,16));
	aldeanoDesconocido->setPosicion(Coordenada(14,14));
	arqueroDesconocido->setPosicion(Coordenada(0,0));

	jugador->agregarNuevaUnidad(soldado);
	jugador->agregarNuevaUnidad(aldeano);
	jugador->agregarNuevaUnidad(arquero);

	escenario->agregarEntidad(soldado->getPosicion(),soldado);
	escenario->agregarEntidad(aldeano->getPosicion(),aldeano);
	escenario->agregarEntidad(arquero->getPosicion(),arquero);
	escenario->agregarEntidad(aldeanoDesconocido->getPosicion(),aldeanoDesconocido);
	escenario->agregarEntidad(arqueroDesconocido->getPosicion(),arqueroDesconocido);

	escenario->getCapa()->setRangoDeVision(configGame.rango_vision);
	//this->protagonista = this->escenario->getProtagonista();
	this->barraEstado = new BarraEstado(configGame.ancho_pantalla, 150, jugador); // Podría ser proporcional al tamaño de la ventana (MC)

	this->contenedorSonidos = new ContenedorDeSonidos();
}

/********************************************************************************/
void Juego::cargaInicialDeRecursos() {
	// bieeen hardcodeado, de prueba
	agregarRecurso(ORO, Coordenada(22,22));
	agregarRecurso(COMIDA, Coordenada(20,22));
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
//Sprite* Juego::getSpritePlayer(){
//	return this->contenedor->getSpriteDeEntidad(protagonista);
//}

Sprite* Juego::getSpriteDeEntidad(Entidad* entidad){
	return contenedor->getSpriteDeEntidad(entidad);
}
/***************************************************/
// Devuelve Sprite protagonista de jugador idJug; en caso de no encontrarlo devuelve el propio.
// Solo para una entidad de adversario; no chequea tipo.
Sprite* Juego::getSpritePlayer(int id_jug, int identificador) {
	for (vector<Unidad*>::iterator it = enemigos->begin(); it < enemigos->end(); ++it)
		if ((*it)->getIDJug() == id_jug && (*it)->get_identificador() == identificador)
			return this->contenedor->getSpriteDeEntidad(*it);
	throw NoSeRecibio();
	//return this->contenedor->getSpriteDeEntidad(protagonista);
}

/***************************************************/
vector<Sprite*> Juego::getSpritesProtagonistas() {
	vector<Sprite*> v;
	for (vector<Unidad*>::iterator it = enemigos->begin(); it < enemigos->end(); ++it)
		v.push_back(this->contenedor->getSpriteDeEntidad(*it)); // ajenos

	vector<Unidad*> unidades = jugador->getUnidades();
	for (unsigned i = 0; i < unidades.size(); i++)
		v.push_back(contenedor->getSpriteDeEntidad(unidades[i])); // propio
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
Mix_Chunk* Juego::getSonidoTipo(TipoSonido tipo){
	return this->contenedorSonidos->getSonidoTipo(tipo);
}


/***************************************************/
//bool Juego::esCliente() {
//	return (this->connection != NULL);
//}

/***************************************************/
//Connection* const Juego::getConnection() {
//	return this->connection;
//}

/***************************************************/
//void Juego::setConnection(Connection* conn) {
//	this->connection = conn;
//}

/***************************************************/
//void Juego::olvidarConnection() {
//	this->connection->finalizar();
//}

/***************************************************/
//PosEntidad Juego::getPosEntDeProtagonista() {
//	return PosEntidad(this->escenario->getPosProtagonista(), this->protagonista);
//}

/***************************************************/
void Juego::agregarRecurso(TipoEntidad recurso, Coordenada coord) {
	Entidad* recurso_a_agregar = new Entidad(recurso); // AGHH MI FACTORYY TODO
	recurso_a_agregar->setPosicion(coord);
	try {
		escenario->agregarEntidad(coord, recurso_a_agregar);
		this->contenedor->generarYGuardarSpriteEntidad(recurso_a_agregar, Coordenada(*cero_x, *cero_y), escenario);
	} catch ( FueraDeEscenario &e ) {}
}

/***************************************************/
void Juego::cargarEnemigo(Unidad* enemigo) {
	if (enemigo->getIDJug() == getIDJugador())
		return;	// Si es el propio, no hacer nada...
	try {
		for (vector<Unidad*>::iterator it = enemigos->begin(); it < enemigos->end(); ++it)
			if ((*it)->getIDJug() == enemigo->getIDJug() && (*it)->get_identificador() == enemigo->get_identificador()) {
				Camino camCorreccion;
				camCorreccion.agregar(enemigo->getPosicion());
				getSpritePlayer(enemigo->getIDJug(),enemigo->get_identificador())->setearNuevoCamino(camCorreccion, getCoordCeros());
				return;		// Si ya hay una entidad de ese jugador, corrije su posición nomás.
			}

		PosEntidad posEnt(enemigo->getPosicion().x, enemigo->getPosicion().y, enemigo);
		bool resPosicionar = false;
		int aux = posEnt.x;
		int *d = &posEnt.x;
		// cabeza para resolver situación inicial, se las arregla para el 9# % de los casos
		while (!resPosicionar) {
			try {
				resPosicionar = escenario->agregarEntidad(posEnt.coord(), enemigo);
				if (!resPosicionar)
					*d += 1;
			} catch ( FueraDeEscenario &e ) {
				if (*d == aux) throw FueraDeEscenario();
				*d = aux;
				d = &posEnt.y;
			}
		}
		enemigo->setPosicion(Coordenada(posEnt.x,posEnt.y));
		enemigos->push_back(enemigo);
		this->contenedor->generarYGuardarSpriteEntidad(enemigo, Coordenada(*cero_x, *cero_y), escenario);
	} catch ( NoSeRecibio &e ) {}
}

/***************************************************/
void Juego::toggleEnemigo(int id_jug) {
	Unidad* entidadEnemigo = NULL;
	for (vector<Unidad*>::iterator it = enemigos->begin(); it < enemigos->end(); ++it)
		if ((*it)->getIDJug() == id_jug) {
			entidadEnemigo = *it;
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
void Juego::continuar() {	//Modularizar si se pasa a usar threads
	this->jugador->interaccionesDeUnidades();
}

/***************************************************/
Juego::~Juego() {
	this->enemigos->clear();
	delete this->escenario;
	delete this->contenedor;
	delete this->barraEstado;
	delete this->contenedorSonidos;
	delete jugador;
}
