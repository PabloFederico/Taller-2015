/*
 * Juego.cpp
 *
 *  Created on: 6 de sept. de 2015
 *      Author: emanuel
 */

#include "../modelo/Juego.h"

#include "../utils/Yaml.h"


//Juego::Juego(Connection* lan = NULL, Coordenada* posInicial = NULL, InfoEscenario* infoEscRed = NULL):
//			connection(lan) {
Juego::Juego(){
	this->idJug = 1; /* esta hardcodeado para probar (sin cliente-servidor) */
	this->nombreJug = "Pepito";
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

	//if esCliente(), receive Escenario; ¡chequear disponibilidad de las entidades! rellenar con missing las faltantes.

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

	escenario->getCapa()->setRangoDeVision(configGame.rango_vision);
	this->barraEstado = new BarraEstado(configGame.ancho_pantalla, 150, jugador); // Podría ser proporcional al tamaño de la ventana (MC)

	this->contenedorSonidos = new ContenedorDeSonidos();
}

/********************************************************************************/
void Juego::cargaInicialDeRecursos() {
	// bieeen hardcodeado, de prueba
	// Actualizacion: esto ya no va. Borrar una vez terminado el tp. Lo dejo para pruebas. todo
	agregarRecurso(ORO, Coordenada(22,22));//
	agregarRecurso(COMIDA, Coordenada(20,22));//
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
}

/********************************************************************************/
ContenedorDeRecursos* Juego::getContenedorRecursos(){
	 return contenedor;
}
/********************************************************************************/

Sprite* Juego::getSpriteDeEntidad(Entidad* entidad){
	return contenedor->getSpriteDeEntidad(entidad);
}

/********************************************************************************/
// Devuelve puntero a Entidad buscada, sea propia o de adversario.
// En caso de no encontrarla, devuelve NULL.
Entidad* Juego::getEntidad(TipoEntidad tipo, int id_jug, int identificador) {
	if (id_jug == this->getIDJugador())
		return this->jugador->getEntidad(tipo, identificador);

	if (EsUnidad(tipo)) {
		for (vector<Unidad*>::iterator it = this->unidadesEnemigos->begin(); it < this->unidadesEnemigos->end(); ++it)
			if ((*it)->getIDJug() == id_jug && (*it)->get_identificador() == identificador)
				return (*it);
	} else if (EsEdificio(tipo)) {
		for (vector<Edificio*>::iterator it = this->edificiosEnemigos->begin(); it < this->edificiosEnemigos->end(); ++it)
			if ((*it)->getIDJug() == id_jug && (*it)->get_identificador() == identificador)
				return (*it);
	} else if (EsRecurso(tipo)) {
		return this->escenario->getRecurso(/*tipo, */identificador);
	}

	return NULL;	//	throw NoSeRecibio(); ?
}

/********************************************************************************/
vector<Sprite*> Juego::getSpritesUnidades() {
	vector<Sprite*> v;
	for (vector<Unidad*>::iterator it = unidadesEnemigos->begin(); it < unidadesEnemigos->end(); ++it)
		v.push_back(this->contenedor->getSpriteDeEntidad(*it)); // ajenas

	vector<Unidad*> unidadesPropias = jugador->getUnidades();
	for (unsigned i = 0; i < unidadesPropias.size(); i++)
		v.push_back(contenedor->getSpriteDeEntidad(unidadesPropias[i])); // propias
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
void Juego::reiniciar(){	// TODO revisar? O desactivar la posibilidad cuando esCliente() == true?
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
//	this->connection->finalizar();	imprimir que se perdió la conexión y cerrar?
//}

/***************************************************/
//PosEntidad Juego::getPosEntDeProtagonista() {
//	return PosEntidad(this->escenario->getPosProtagonista(), this->protagonista);
//}

/***************************************************/
void Juego::cargarEnemigo(Entidad* enemigo) {
	if (enemigo->getIDJug() == this->getIDJugador())
		return;	// Si es propio, no hacer nada.
	if (enemigo->esUnidad())
		unidadesEnemigos->push_back((Unidad*)enemigo);
	else if (enemigo->esEdificio())
		edificiosEnemigos->push_back((Edificio*)enemigo);
}

/***************************************************/
// FUNCIONES DE CREACIÓN. Para hacer una entidad local nueva, ignorar los últimos 2 argumentos.

Unidad* Juego::crearNuevaUnidad(TipoEntidad tipoUnid, Coordenada coord, int id_jug, int id_unidad) {
	if (id_jug == -1)
		id_jug = this->idJug;
	if (!this->escenario->coordEnEscenario(coord)) {
		Log::imprimirALog(ERR, "Se intentó posicionar fuera del escenario.");
		return NULL;
	}

	Unidad *unidad = new Unidad(tipoUnid, id_jug, id_unidad);	// Factory todo?
	unidad->setPosicion(coord);

	if (id_jug == this->idJug)
		this->jugador->agregarNuevaUnidad(unidad);
	else
		cargarEnemigo(unidad);
	this->contenedor->generarYGuardarSpriteEntidad(unidad, Coordenada(*cero_x, *cero_y), this->escenario); // COMENTADA SI SE CORRE ANTES QUE VENTANAJUEGO TODO
	this->escenario->agregarEntidad(coord, unidad);
	return unidad;
}

/***************************************************/
Construccion* Juego::comenzarNuevaConstruccion(TipoEntidad tipoEdif, Coordenada coord, int id_jug, int id_edificio) {
	if (id_jug == -1)
		id_jug = this->idJug;
	if (!this->escenario->coordEnEscenario(coord)) {
		Log::imprimirALog(ERR, "Se intentó construir fuera del Escenario.");
		return NULL;
	}

	Construccion *construccion = new Construccion(tipoEdif, id_jug, id_edificio);	// Factory todo?
	construccion->setPosicion(coord);

	if (id_jug == this->idJug)
		this->jugador->agregarNuevoEdificio(construccion);
	else
		cargarEnemigo(construccion);
	this->contenedor->generarYGuardarSpriteEntidad(construccion, Coordenada(*cero_x, *cero_y), this->escenario);	// COMENTADA SI SE CORRE ANTES QUE VENTANAJUEGO TODO
	this->escenario->agregarEntidad(coord, construccion);
	return construccion;
}

/***************************************************/
// Únicamente para el estado inicial y pruebas.
Edificio* Juego::crearNuevoEdificio(TipoEntidad tipoEdif, Coordenada coord, int id_jug, int id_edificio) {
	if (id_jug == -1)
		id_jug = this->idJug;
	if (!this->escenario->coordEnEscenario(coord)) {
		Log::imprimirALog(ERR, "Se intentó posicionar fuera del escenario.");
		return NULL;
	}

	Edificio *edificio = new Edificio(tipoEdif, id_jug, id_edificio);	// Factory todo?
	edificio->setPosicion(coord);

	if (id_jug == this->idJug)
		this->jugador->agregarNuevoEdificio(edificio);
	else
		cargarEnemigo(edificio);
	this->contenedor->generarYGuardarSpriteEntidad(edificio, Coordenada(*cero_x, *cero_y), this->escenario); // COMENTADA SI SE CORRE ANTES QUE VENTANAJUEGO TODO
	this->escenario->agregarEntidad(coord, edificio);
	return edificio;
}

/***************************************************/
void Juego::crearNuevaUnidadApartirDeEdificioSeleccionado(TipoEntidad tipoEntidadACrear){
	Edificio* edificio = jugador->getEdificioSeleccionado();
	if (edificio == NULL) return;
	if (jugador->getRecursosDisponibles() >= edificio->getCostoPorUnidad()){
		Coordenada c = Calculador::obtenerCoordenadaLibreCercaDeEdificio(edificio,escenario);
		if (!escenario->coordEnEscenario(c)) return;
		//TODO mandarle una señal al Servidor por creación de nueva unidad
//		Unidad* nuevaUnidad = new Unidad(tipoEntidadACrear,this->idJug);
		/*Unidad* nuevaUnidad = */crearNuevaUnidad(tipoEntidadACrear, c, this->getIDJugador());
//		nuevaUnidad->setPosicion(c);
//		escenario->agregarEntidad(c,nuevaUnidad);
		//contenedor->generarYGuardarSpriteEntidad(nuevaUnidad,Coordenada(*cero_x,*cero_y),escenario);	// LO DEJO ACÁ PQ EN crearNuevaUnidad SIGUE COMENTADO TODO
		jugador->descontarRecursos(edificio->getCostoPorUnidad());
//		jugador->agregarNuevaUnidad(nuevaUnidad);
		std::cout <<"creando nueva unidad tipo "<<tipoEntidadACrear<<" en : "<<c.x<<","<<c.y<<"\n";
	}
}

/***************************************************/
// Pasar el id_recurso que dicte el Server. En caso de jugar offline, ignorarlo.
Entidad* Juego::agregarRecurso(TipoEntidad recurso, Coordenada coord, int id_recurso) {
	Entidad* recurso_a_agregar = new Entidad(recurso, 0, id_recurso); // factory todo?
	recurso_a_agregar->setPosicion(coord);
	try {
		this->contenedor->generarYGuardarSpriteEntidad(recurso_a_agregar, Coordenada(*cero_x, *cero_y), escenario);
		escenario->agregarEntidad(coord, recurso_a_agregar);
	} catch ( FueraDeEscenario &e ) {
		delete recurso_a_agregar;
		return NULL;
	}
	return recurso_a_agregar;
}

/***************************************************
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

********************************************************************************/
Edificio* Juego::terminarConstruccion(ConstruccionTermino c) {
	Entidad *construc = this->getEntidad(c.tipoEdif, c.idJug, c.dni);
	if (!construc) return NULL;

	construc->morir();
	std::cout << construc->enc()<<" construccion terminada"<<std::endl;//
	this->escenario->quitarEntidad(construc);
	this->contenedor->borrarSpriteDeEntidad(construc);
	delete construc;

	Edificio *nuevoEdificio = new Edificio(c.tipoEdif, c.idJug, c.dni);
	nuevoEdificio->set_identificador(c.dni);
	nuevoEdificio->set_id_jugador(c.idJug);
	nuevoEdificio->setPosicion(Coordenada(c.x,c.y));
	nuevoEdificio->sufrirGolpe(100 - c.vidaRestante); // hardcodeo de vida inicial de construcción

	if (c.idJug == this->getIDJugador())
		this->jugador->guardarConstruccionTerminada(nuevoEdificio);
	else {
		vector<Edificio*>::iterator it;
		for (it = this->edificiosEnemigos->begin(); it < this->edificiosEnemigos->end(); ++it)
			if ((*it)->get_identificador() == c.dni && (*it)->perteneceAJugador(c.idJug))
				break;
		if (it != this->edificiosEnemigos->end())
			this->edificiosEnemigos->erase(it);
		this->edificiosEnemigos->push_back(nuevoEdificio);
	}
	return nuevoEdificio;
}

/********************************************************************************/

vector<Entidad*> Juego::revisarMuertos() {
	// Propios
	vector<Entidad*> funeral = this->jugador->revisarMuertosPropios();
	// Enemigos
	for (std::vector<Unidad*>::iterator uniIt = this->unidadesEnemigos->begin(); uniIt < this->unidadesEnemigos->end(); ++uniIt) {
		//if (!(*uniIt)->sigueViva()) {
		if ((*uniIt)->getEstado() == MUERTO) {
			Unidad* moribundo = *uniIt;

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

/********************************************************************************/

void Juego::iniciarInteraccionEntre(Entidad* ejecutor, Entidad* receptor) {
	ejecutor->interactuarCon(receptor);
}

void Juego::iniciarInteraccionEntre(TipoEntidad tipo_ejecutor, int idJug_ejecutor, int dni_ejecutor,
									TipoEntidad tipo_receptor, int idJug_receptor, int dni_receptor) {
	Entidad *ejecutor = getEntidad(tipo_ejecutor, idJug_ejecutor, dni_ejecutor);
	Entidad *receptor = getEntidad(tipo_receptor, idJug_receptor, dni_receptor);
	if (!ejecutor || !receptor)
		throw NoExiste();

	ejecutor->interactuarCon(receptor);
}

/********************************************************************************/
void Juego::interaccionesDeUnidades() {
	vector<Unidad*> all_unidades = this->jugador->getUnidades();
	for(vector<Unidad*>::iterator it = this->unidadesEnemigos->begin(); it < this->unidadesEnemigos->end(); ++it)
		all_unidades.push_back(*it);

	for (std::vector<Unidad*>::iterator uniIt = all_unidades.begin(); uniIt < all_unidades.end(); ++uniIt) {
		try {
			(*uniIt)->interactuar();

		} catch ( UnidadDebeAcercarse &u ) {
			Sprite *sprite = this->contenedor->getSpriteDeEntidad(*uniIt);
			if (!sprite->estaEnMovimiento()) {
				Coordenada coord_ceros = getCoordCeros();
				Coordenada pos_tile_sprite = Calculador::tileParaPixel(sprite->getPosPies(), coord_ceros);
				Camino cam = Calculador::obtenerCaminoMinParaAcercarse(escenario, pos_tile_sprite, Coordenada(u.x,u.y), coord_ceros, (*uniIt)->getRangoAccion());
				sprite->setearNuevoCamino(cam, coord_ceros);
			}

		} catch ( Recoleccion &r ) {
			if ((*uniIt)->perteneceAJugador(this->getIDJugador()))
				this->jugador->agregarRecursoEconomico(r.tipo, r.cant);

		} catch ( ConstruccionTermino &c ) {
			Edificio *edif = terminarConstruccion(c);
			this->contenedor->generarYGuardarSpriteEntidad(edif, getCoordCeros(), escenario);
			this->escenario->agregarEntidad(Coordenada(c.x,c.y), edif);
		}
	}
}

/********************************************************************************/
void Juego::continuar() {
	interaccionesDeUnidades();

	// Limpiar muertes
	vector<Entidad*> funeral = revisarMuertos(); // Esto abarca todas las entidades.
	for (vector<Entidad*>::iterator it = funeral.begin(); it < funeral.end(); ++it) {
		Entidad* muerto = *it;
		this->emitirSonido(muerto);
		muerto->morir();
		std::cout << muerto->enc()<<" sos un muerto"<<std::endl;//

		if (muerto == escenario->getEntidadSeleccionada())
			escenario->setearTileClic(NULL, Coordenada(0,0));
		this->jugador->limpiarSeleccionDeUnidadMuerta((Unidad*)muerto);

		this->escenario->quitarEntidad(muerto);
		this->contenedor->borrarSpriteDeEntidad(muerto);

		ejecutoresOlvidarInteraccionCon(muerto);
		//Si la entidad muerta se convierte en recurso, lo colocamos:
		reemplazarEntidadPorRecurso(muerto);

		delete muerto;
	}
}

/***************************************************/

void Juego::emitirSonido(Entidad* entidad){
	Mix_Chunk *chunk;
	//FALTAN CARGAR.
	switch (entidad->getTipo()){
		case SOLDADO:
		case ALDEANO:
			chunk = this->contenedorSonidos->getSonidoTipo(MORIR_HUMANO);
			break;
		case ANIMAL:
			chunk = this->contenedorSonidos->getSonidoTipo(MORIR_ANIMAL);
			break;
		case ARBOL:
			chunk = this->contenedorSonidos->getSonidoTipo(TALAR);
			break;
		case MINA_ORO:
		case ORO:
			chunk = this->contenedorSonidos->getSonidoTipo(OBTENER_ORO);
			break;
		case COMIDA:
			chunk = this->contenedorSonidos->getSonidoTipo(COMER);
			break;
		case MINA_PIEDRA:
		case PIEDRA:
			chunk = this->contenedorSonidos->getSonidoTipo(OBTENER_PIEDRA);
			break;
		default :
			return;
	}
	Mix_PlayChannel(1, chunk, 0);
}

/***************************************************/

void Juego::ejecutoresOlvidarInteraccionCon(Entidad* muerto) {
	this->jugador->ejecutoresOlvidarInteraccionCon(muerto);
	for (std::vector<Unidad*>::iterator uniIt = this->unidadesEnemigos->begin(); uniIt < this->unidadesEnemigos->end(); ++uniIt)
		(*uniIt)->olvidarReceptorSiEs(muerto);
}

void Juego::reemplazarEntidadPorRecurso(Entidad* entidad){
	Coordenada coord = entidad->getPosicion();
	//FALTAN CARGAR.
	switch (entidad->getTipo()){
		case ARBOL:
			this->agregarRecurso(MADERA, coord);
			break;
		case ANIMAL:
			this->agregarRecurso(COMIDA, coord);
			break;
		case MINA_PIEDRA:
			this->agregarRecurso(PIEDRA, coord);
			break;
		case MINA_ORO:
			this->agregarRecurso(ORO, coord);
			break;
		default :
			break;
	}
}

/***************************************************/
Juego::~Juego() {
	this->unidadesEnemigos->clear(); // se deberían borrar! no?
	this->edificiosEnemigos->clear();
	delete this->escenario;
	delete this->contenedor;
	delete this->barraEstado;
	delete this->contenedorSonidos;
	delete jugador;
}
