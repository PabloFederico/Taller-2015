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
	this->unidadesEnemigos = new vector<Unidad*>();
	this->edificiosEnemigos = new vector<Edificio*>();
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
	this->escenario = new Escenario(configGame.escenarios[0], this->fabricaDeEntidades, this->unidadesEnemigos, this->edificiosEnemigos);

	/* Las siguientes entidades son de prueba */
	Unidad* soldado = new Unidad(SOLDADO,1);
	Unidad* aldeano = new Unidad(ALDEANO,1);
	Unidad* arquero = new Unidad(ARQUERO,1);
	Unidad* aldeanoDesconocido = new Unidad(ALDEANO,2);
	Unidad* arqueroDesconocido = new Unidad(ARQUERO,2);
	soldado->setPosicion(Coordenada(0,5));
	aldeano->setPosicion(Coordenada(0,6));
	arquero->setPosicion(Coordenada(1,8));
	aldeanoDesconocido->setPosicion(Coordenada(14,14));
	arqueroDesconocido->setPosicion(Coordenada(0,0));

	jugador->agregarNuevaUnidad(soldado);
	jugador->agregarNuevaUnidad(aldeano);
	jugador->agregarNuevaUnidad(arquero);
	this->cargarEnemigo(aldeanoDesconocido);

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

/********************************************************************************/
ContenedorDeRecursos* Juego::getContenedorRecursos(){
	 return contenedor;
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
	for (vector<Unidad*>::iterator it = unidadesEnemigos->begin(); it < unidadesEnemigos->end(); ++it)
		if ((*it)->getIDJug() == id_jug && (*it)->get_identificador() == identificador)
			return this->contenedor->getSpriteDeEntidad(*it);
	throw NoSeRecibio();
	//return this->contenedor->getSpriteDeEntidad(protagonista);
}

/***************************************************/
vector<Sprite*> Juego::getSpritesUnidades() {
	vector<Sprite*> v;
	for (vector<Unidad*>::iterator it = unidadesEnemigos->begin(); it < unidadesEnemigos->end(); ++it)
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
	Entidad* recurso_a_agregar = new Entidad(recurso); // factory todo?
	recurso_a_agregar->setPosicion(coord);
	try {
		escenario->agregarEntidad(coord, recurso_a_agregar);
		this->contenedor->generarYGuardarSpriteEntidad(recurso_a_agregar, Coordenada(*cero_x, *cero_y), escenario);
	} catch ( FueraDeEscenario &e ) {
		delete recurso_a_agregar;
	}
}

/***************************************************/
void Juego::comenzarNuevaConstruccion(TipoEntidad tipoEdif, Coordenada coord, int id_jug = -1) {
	if (id_jug == -1)
		id_jug = this->idJug;
	if (this->escenario->coordEnEscenario(coord)) {
		Log::imprimirALog(ERR, "Se intentó construir fuera del Escenario.");
		return;
	}
	Construccion *construccion = new Construccion(tipoEdif, id_jug);
	construccion->setPosicion(coord);
	this->jugador->agregarNuevoEdificio(construccion, id_jug);
	this->escenario->agregarEntidad(coord, construccion);
	this->contenedor->generarYGuardarSpriteEntidad(construccion, Coordenada(*cero_x, *cero_y), this->escenario);
}

/***************************************************/
void Juego::cargarEnemigo(Entidad* enemigo) {
	if (enemigo->getIDJug() == getIDJugador())
		return;	// Si es propio, no hacer nada...
	try {
		if (enemigo->esUnidad()) {
			Unidad* enemigoUni = (Unidad*)enemigo;
			for (vector<Unidad*>::iterator it = unidadesEnemigos->begin(); it < unidadesEnemigos->end(); ++it) {
				if ((*it)->getIDJug() == enemigoUni->getIDJug() && (*it)->get_identificador() == enemigoUni->get_identificador()) {
					Camino camCorreccion;
					camCorreccion.agregar(enemigoUni->getPosicion());
					getSpritePlayer(enemigoUni->getIDJug(),enemigoUni->get_identificador())->setearNuevoCamino(camCorreccion, getCoordCeros());
					return;		// Si ya hay una entidad de ese jugador, corrije su posición nomás.
				}
			}
		} else if (enemigo->esEdificio()) {
			Edificio* enemigoEdi = (Edificio*)enemigo;
			for (vector<Edificio*>::iterator it = edificiosEnemigos->begin(); it < edificiosEnemigos->end(); ++it) {
				if ((*it)->getIDJug() == enemigoEdi->getIDJug() && (*it)->get_identificador() == enemigoEdi->get_identificador()) {
					///Esto es cualquier cosa. Si todo lo demás funciona correcto (en el setteo de escenario y etc), borrarlo.
						//Camino camCorreccion;
						//camCorreccion.agregar(enemigoEdi->getPosicion());
						//getSpritePlayer(enemigoEdi->getIDJug(),enemigoEdi->get_identificador())->setearNuevoCamino(camCorreccion, getCoordCeros());
					return;		// Si ya hay una entidad de ese jugador, corrije su posición nomás.
				}
			}
		} else {
			Log::imprimirALog(ERR, "Se intentó cargar enemigo ni edificio ni unidad (Juego::cargarEnemigo).");
			return;
		}

		/*Mismo comentario que arriba
		PosEntidad posEnt(enemigo->getPosicion().x, enemigo->getPosicion().y, enemigo);
		bool resPosicionar = false;
		int aux = posEnt.x;
		int *d = &posEnt.x;
		// bien cabeza para resolver situación inicial, se las arregla para el noventialgo % de los casos
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
		}*/
		//enemigo->setPosicion(Coordenada(posEnt.x,posEnt.y));
		if (enemigo->esUnidad())
			unidadesEnemigos->push_back((Unidad*)enemigo);
		else
			edificiosEnemigos->push_back((Edificio*)enemigo);

		//this->contenedor->generarYGuardarSpriteEntidad(enemigo, Coordenada(*cero_x, *cero_y), escenario);
	} catch ( NoSeRecibio &e ) {}
}

/***************************************************/
void Juego::toggleEnemigo(int id_jug, int idUnidad) {
	Unidad* entidadEnemigo = NULL;
	for (vector<Unidad*>::iterator it = unidadesEnemigos->begin(); it < unidadesEnemigos->end(); ++it)
		if ((*it)->getIDJug() == id_jug && (*it)->get_identificador() == idUnidad) {
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
vector<Entidad*> Juego::revisarMuertos() {
	// Propios
	vector<Entidad*> funeral = this->jugador->revisarMuertosPropios();
	// Enemigos
	for (std::vector<Unidad*>::iterator uniIt = this->unidadesEnemigos->begin(); uniIt < this->unidadesEnemigos->end(); ++uniIt) {
		if (!(*uniIt)->sigueViva()) {
			Unidad* moribundo = *uniIt;

			this->jugador->limpiarRastrosDeUnidadMuerta(moribundo);
			unidadesEnemigos->erase(uniIt);
			uniIt = this->unidadesEnemigos->begin(); //por las dudas

			funeral.push_back(moribundo);
		}
	}
	for (std::vector<Edificio*>::iterator ediIt = this->edificiosEnemigos->begin(); ediIt < this->edificiosEnemigos->end(); ++ediIt) {
		if (!(*ediIt)->sigueViva()) {
			Edificio* dilapidado = *ediIt;

			edificiosEnemigos->erase(ediIt);
			ediIt = this->edificiosEnemigos->begin(); //por las dudas

			funeral.push_back(dilapidado);
		}
	}
	vector<Entidad*> basurero = this->escenario->revisarMuertosDeNadie();
	for (std::vector<Entidad*>::iterator entIt = basurero.begin(); entIt < basurero.end(); ++entIt)
		funeral.push_back(*entIt);

	return funeral;
}

/***************************************************/
void Juego::continuar() {	// Modularizar si se pasa a usar threads
	this->jugador->interaccionesDeUnidades(this->escenario, this->contenedor, Coordenada(*cero_x, *cero_y));

	// Limpiar muertes
	vector<Entidad*> funeral = revisarMuertos();
	for (vector<Entidad*>::iterator it = funeral.begin(); it < funeral.end(); ++it) {
		Entidad* muerto = *it;
		muerto->morir();

		this->emitirSonido(muerto);

		std::cout << muerto->enc()<<" sos un muerto"<<std::endl;//
		this->escenario->quitarEntidad(muerto->getPosicion(), muerto);
		this->contenedor->borrarSpriteDeEntidad(muerto);
		//Falta algo todo?

		delete muerto;
	}
}

/***************************************************/

void Juego::emitirSonido(Entidad* entidad){
	//FALTAN CARGAR.
	switch (entidad->getTipo()){
		case SOLDADO:
		case ALDEANO:
			Mix_PlayChannel(1, this->contenedorSonidos->getSonidoTipo(MORIR_HUMANO), 0);
			break;
		case ANIMAL:
			Mix_PlayChannel(1, this->contenedorSonidos->getSonidoTipo(MORIR_ANIMAL), 0);
			break;
		case EDIFICIO:
		case ORO:
			Mix_PlayChannel(1, this->contenedorSonidos->getSonidoTipo(DESTRUIR), 0);
			break;
		default :
			break;
	}
}

/***************************************************/
Juego::~Juego() {
	this->unidadesEnemigos->clear();
	this->edificiosEnemigos->clear();
	delete this->escenario;
	delete this->contenedor;
	delete this->barraEstado;
	delete this->contenedorSonidos;
	delete jugador;
}
