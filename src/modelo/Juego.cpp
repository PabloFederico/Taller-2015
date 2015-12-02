/*
 * Juego.cpp
 *
 *  Created on: 6 de sept. de 2015
 *      Author: emanuel
 */

#include "../modelo/Juego.h"

#include "../utils/Yaml.h"
#include "Flecha.h"


Juego::Juego(Connection* lan = NULL, ObjetivoEscenario modoDeJuego = MODO_DEFAULT, ConfiguracionJuego* infoJuegoRed = NULL):
			modo_de_juego(modoDeJuego), connection(lan) {
	this->idJug = 1;
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
	mapAtaquesLargaDistancia = new Map<Entidad*,Sprite*>();
	cargarNumJugador();
	cargarJuego(infoJuegoRed);
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
	if (esCliente())
		 this->idJug = connection->getIDJugador();
}

/********************************************************************************/
void Juego::cargarJuego(ConfiguracionJuego* infoJuegoRed = NULL) {
	if (infoJuegoRed == NULL)
		configGame = Yaml::cargarConfiguracionJuego(/*"config.yaml"*/);
	else configGame = *infoJuegoRed;

	if (esCliente()) {
		configGame.nombreJugador = NombreDeJug(this->idJug);
		generarNuevasUnidadesYEdificiosIniciales();
	}

	this->jugador = new Jugador(configGame.nombreJugador,idJug);
	this->fabricaDeEntidades = new EntidadFactory(configGame.entidades);
	this->escenario = new Escenario(configGame.escenarios[0], this->idJug);
	cargaInicialDeEntidades();

	escenario->getCapa()->setRangoDeVision(configGame.rango_vision);
	this->barraEstado = new BarraEstado(configGame.ancho_pantalla, 150, jugador); // Podría ser proporcional al tamaño de la ventana (MC)

	this->contenedorSonidos = new ContenedorDeSonidos();
}

/********************************************************************************/
void Juego::cargaInicialDeEntidades() {
//	bool todas = true;//
	InfoEscenario infoEsc = this->configGame.escenarios[0];
	vector<PosTipoEntidad> vec = infoEsc.getPosicionesEntidades();
	for (unsigned i = 0; i < vec.size(); i++) {
		TipoEntidad tipo = vec[i].tipo;
		Coordenada pos(vec[i].x,vec[i].y);

		Entidad *aux;
		if (EsUnidad(tipo))
			aux = crearNuevaUnidad(tipo, pos);
		else if (EsConstruccion(tipo))
			aux = comenzarNuevaConstruccion(tipo, pos);
		else if (EsEdificio(tipo))
			aux = crearNuevoEdificio(tipo, pos);
		else if (EsRecurso(tipo))
			aux = agregarRecurso(tipo, pos);
		else {								//Ninguno reintenta si no se pudo agregar
			try {
				aux = this->fabricaDeEntidades->nuevaEntidad(tipo);
				if (aux) this->escenario->agregarEntidad(pos, aux);
			} catch ( FueraDeEscenario &e ) {
				delete aux;
				aux = NULL;
			}
		}
//		if (!aux) {
//			std::cout << "No se pudo crear/agregar entidad de tipo "<<tipo<<std::endl;//
//			todas = false;//
//		} //else std::cout << "Creada entidad de tipo "<<tipo<<" en "<<pos.enc()<<std::endl;//
	}
	infoEsc.getPosicionesEntidades().clear();
//	if (todas)
//		std::cout<<"se agregaron todas las entidades"<<std::endl;//
}

/********************************************************************************/
// DESCARTA todas las unidades y edificios del configGame y crea propias, con posiciones rándom.
void Juego::generarNuevasUnidadesYEdificiosIniciales() {
	vector<PosTipoEntidad>* vecIni = &configGame.escenarios[0].posTipoEntidades;
	// primero, borrar las unidades y edificios que vienen
	for (vector<PosTipoEntidad>::iterator it = vecIni->begin(); it < vecIni->end(); ) {
		if (EsUnidad(it->tipo) || EsEdificio(it->tipo)) {
			vecIni->erase(it);
			//it = vecIni->begin(); //por las
		} else ++it;
	}

	Coordenada c_uni, c_cc;
	int m = 8; // margen de distancia al CC
	int size_x = configGame.escenarios[0].size_x - 1;
	int size_y = configGame.escenarios[0].size_y - 1;

	// segundo, creo: centro cívico y 3 aldeanos; con posiciones rándom
	do { c_cc = Calculador::generarPosRandom(size_x-4, 0, size_y-4, 0);
	} while (configGame.escenarios[0].posicionYaOcupada(c_cc));
	configGame.escenarios[0].agregarEntidad(c_cc, CENTRO_CIVICO);

	do { c_uni = Calculador::generarPosRandomDentroDeEscenarioConLimites(size_x, c_cc.x+m, c_cc.x-m, size_y, c_cc.y+m, c_cc.y-m);
	} while (configGame.escenarios[0].posicionYaOcupada(c_uni));
	configGame.escenarios[0].agregarEntidad(c_uni, ALDEANO);

	do { c_uni = Calculador::generarPosRandomDentroDeEscenarioConLimites(size_x, c_cc.x+m, c_cc.x-m, size_y, c_cc.y+m, c_cc.y-m);
	} while (configGame.escenarios[0].posicionYaOcupada(c_uni));
	configGame.escenarios[0].agregarEntidad(c_uni, ALDEANO);

	do { c_uni = Calculador::generarPosRandomDentroDeEscenarioConLimites(size_x, c_cc.x+m, c_cc.x-m, size_y, c_cc.y+m, c_cc.y-m);
	} while (configGame.escenarios[0].posicionYaOcupada(c_uni));
	configGame.escenarios[0].agregarEntidad(c_uni, ALDEANO);

	switch (modo_de_juego) {
	case CAPTURAR_BANDERA:
			do { c_uni = Calculador::generarPosRandomDentroDeEscenarioConLimites(size_x, c_cc.x+m, c_cc.x-m, size_y, c_cc.y+m, c_cc.y-m);
			} while (configGame.escenarios[0].posicionYaOcupada(c_uni));
			configGame.escenarios[0].agregarEntidad(c_uni, BANDERA);
		break;
	case PARTIDA_REGICIDA:
			do { c_uni = Calculador::generarPosRandomDentroDeEscenarioConLimites(size_x, c_cc.x+m, c_cc.x-m, size_y, c_cc.y+m, c_cc.y-m);
			} while (configGame.escenarios[0].posicionYaOcupada(c_uni));
			configGame.escenarios[0].agregarEntidad(c_uni, REY);
		break;
	default: break;
	}
}

/********************************************************************************/
//void Juego::cargaInicialDeRecursos() {
//	// bieeen hardcodeado, de prueba
//	// Actualizacion: esto ya no va. Borrar una vez terminado el tp. Lo dejo para pruebas. todo
//	agregarRecurso(ORO, Coordenada(22,22));//
//	agregarRecurso(COMIDA, Coordenada(20,22));//
//}

/********************************************************************************/
void Juego::envioInicialDeEntidadesPropias() {
	if (!esCliente()) return;

	vector<Edificio*> v_edif = this->jugador->getEdificios();
	for (vector<Edificio*>::iterator it1 = v_edif.begin(); it1 < v_edif.end(); ++it1) {
		Proxy::enviar(this->connection, **it1);
	}
	vector<Unidad*> v_unid = this->jugador->getUnidades();
	for (vector<Unidad*>::iterator it2 = v_unid.begin(); it2 < v_unid.end(); ++it2) {
		Proxy::enviar(this->connection, **it2);
	}
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
vector<Sprite*> Juego::getSpritesEntidadesJugadores() {
	vector<Sprite*> v;
	for (vector<Unidad*>::iterator it = unidadesEnemigos->begin(); it < unidadesEnemigos->end(); ++it)
		v.push_back(this->contenedor->getSpriteDeEntidad(*it)); // ajenas

	vector<Unidad*> unidadesPropias = jugador->getUnidades();
	for (unsigned i = 0; i < unidadesPropias.size(); i++)
		v.push_back(contenedor->getSpriteDeEntidad(unidadesPropias[i])); // propias

	// Se cargar los edificios propios
	vector<Edificio*> edificiosPropios = jugador->getEdificios();
	for (unsigned i = 0; i < edificiosPropios.size(); i++)
		v.push_back(contenedor->getSpriteDeEntidad(edificiosPropios[i]));

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
	actualizarPosicionesAtaquesPorMoverCamara(cant_x,cant_y);
}

/***************************************************/
void Juego::actualizarPosicionesAtaquesPorMoverCamara(int cant_x, int cant_y){
	map<Entidad*,Sprite*>::iterator p = mapAtaquesLargaDistancia->begin();
	while (p != mapAtaquesLargaDistancia->end()){
		Sprite* spriteObjetoAtaque = p->second;
		spriteObjetoAtaque->mover(cant_x,cant_y);
		p++;
	}
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
bool Juego::esCliente() {
	return (this->connection != NULL);
}

/***************************************************/
Connection* const Juego::getConnection() {
	if (!this->connection)
		throw Disconnected();
	return this->connection;
}

/***************************************************/
void Juego::setConnection(Connection* conn) {
	this->connection = conn;
}

/***************************************************/
void Juego::olvidarConnection() {
	std::cout << "~~~DESCONECTADO~~~"<<std::endl;
	this->connection->finalizar();
	this->connection = NULL;
	throw FinJuego();
}

/***************************************************/
void Juego::chat(std::string mensaje) {
	Proxy::enviar(this->connection, mensaje);
}


/***************************************************/
void Juego::cargarEnemigo(Entidad* enemigo) {
	if (enemigo->getIDJug() == this->getIDJugador())
		return;	// Si es propio, no hacer nada.
	if (enemigo->esUnidad())
		unidadesEnemigos->push_back((Unidad*)enemigo);
	else if (enemigo->esEdificio())
		edificiosEnemigos->push_back((Edificio*)enemigo);
	else std::cout << "entidad enemiga ni edificio ni unidad no agregada";//
}

/***************************************************/
/* FUNCIONES DE CREACIÓN. Para hacer una entidad local nueva, ignorar los últimos 2 argumentos. */

Unidad* Juego::crearNuevaUnidad(TipoEntidad tipoUnid, Coordenada coord, int id_jug, int id_unidad) {
	if (id_jug == -1)
		id_jug = this->idJug;
	if (!this->escenario->coordEnEscenario(coord)) {
		Log::imprimirALog(ERR, "Se intentó posicionar fuera del escenario ("+coord.enc()+")");
		return NULL;
	}

	Unidad *unidad = (Unidad*)this->fabricaDeEntidades->nuevaEntidad(tipoUnid, id_jug, id_unidad);
	unidad->setPosicion(coord);

	if (!this->escenario->agregarEntidad(coord, unidad)) {
		delete unidad;
		return NULL;
	}
	if (id_jug == this->idJug)
		this->jugador->agregarNuevaUnidad(unidad);
	else
		cargarEnemigo(unidad);
	if (contenedor) {
		this->contenedor->generarYGuardarSpriteEntidad(unidad, Coordenada(*cero_x, *cero_y), this->escenario);
		if (esCliente() && unidad->perteneceAJugador(this->idJug))
			Proxy::enviar(this->connection, *unidad);
	};//
	return unidad;
}

/***************************************************/
Construccion* Juego::comenzarNuevaConstruccion(TipoEntidad tipoConstr, Coordenada coord, int id_jug, int id_edificio) {
	if (id_jug == -1)
		id_jug = this->idJug;
	if (!this->escenario->coordEnEscenario(coord)) {
		Log::imprimirALog(ERR, "Se intentó construir fuera del escenario ("+coord.enc()+")");
		return NULL;
	}
	if (!EsConstruccion(tipoConstr))
		tipoConstr = TipoConstruccion(tipoConstr);

	Construccion *construccion = (Construccion*)fabricaDeEntidades->nuevaEntidad(tipoConstr, id_jug, id_edificio);
	construccion->setPosicion(coord);
	construccion->setTam(4,4); // puro hardcodeo y rocknroll

	if (!this->escenario->agregarEntidad(coord, construccion)) {
		delete construccion;
		return NULL;
	}
	if (id_jug == this->idJug)
		this->jugador->agregarNuevoEdificio(construccion);
	else
		cargarEnemigo(construccion);
	if (contenedor) {
		this->contenedor->generarYGuardarSpriteEntidad(construccion, Coordenada(*cero_x, *cero_y), this->escenario);
		if (esCliente() && construccion->perteneceAJugador(this->idJug))
			Proxy::enviar(this->connection, *construccion);
	}
	return construccion;
}

/***************************************************/
// Únicamente para el estado inicial y pruebas.
Edificio* Juego::crearNuevoEdificio(TipoEntidad tipoEdif, Coordenada coord, int id_jug, int id_edificio) {
	if (id_jug == -1)
		id_jug = this->idJug;
	if (!this->escenario->coordEnEscenario(coord)) {
		Log::imprimirALog(ERR, "Se intentó posicionar fuera del escenario ("+coord.enc()+")");
		return NULL;
	}

	Edificio *edificio = (Edificio*)fabricaDeEntidades->nuevaEntidad(tipoEdif, id_jug, id_edificio);
	edificio->setPosicion(coord);

	if (!this->escenario->agregarEntidad(coord, edificio)) {
		delete edificio;
		return NULL;
	}
	if (id_jug == this->idJug)
		this->jugador->agregarNuevoEdificio(edificio);
	else
		cargarEnemigo(edificio);
	if (contenedor) {
		this->contenedor->generarYGuardarSpriteEntidad(edificio, Coordenada(*cero_x, *cero_y), this->escenario);
		if (esCliente() && edificio->perteneceAJugador(this->idJug))
			Proxy::enviar(this->connection, *edificio);
	}
	return edificio;
}	// todo falta algo q acomode por posiciones ocupadas; lo mismo para unidades y recursos


/***************************************************/
// Pasar el id_recurso que dicte el Server. En caso de jugar offline, ignorarlo.
Entidad* Juego::agregarRecurso(TipoEntidad tipoRecurso, Coordenada coord, int id_recurso) {
	Entidad* recurso_a_agregar = fabricaDeEntidades->nuevaEntidad(tipoRecurso, 0, id_recurso);
	recurso_a_agregar->setPosicion(coord);
	try {
		if (!escenario->agregarEntidad(coord, recurso_a_agregar))
			throw FueraDeEscenario();
		this->contenedor->generarYGuardarSpriteEntidad(recurso_a_agregar, Coordenada(*cero_x, *cero_y), escenario);
	} catch ( FueraDeEscenario &e ) {
		delete recurso_a_agregar;
		return NULL;
	}
	return recurso_a_agregar;
}


/********************************************************************************/
bool Juego::crearNuevaUnidadApartirDeEdificioSeleccionado(TipoEntidad tipoEntidadACrear){
	Edificio* edificio = jugador->getEdificioSeleccionado();
	if (edificio == NULL) return false;
	if (jugador->tieneRecursosParaCrearUnidad(tipoEntidadACrear)){
		Coordenada c = Calculador::obtenerCoordenadaLibreCercaDeEdificio(edificio,escenario);
		if (!escenario->coordEnEscenario(c)) return false;

		if (crearNuevaUnidad(tipoEntidadACrear, c, this->getIDJugador())) {
			jugador->descontarRecursosPorCrearUnidad(tipoEntidadACrear);
			return true;
		}
		return false;
	} else return false;
}

/********************************************************************************/
Edificio* Juego::terminarConstruccion(ConstruccionTermino c) {
	Entidad *construc = this->getEntidad(c.tipoEdif, c.idJug, c.dni);
	if (!construc) return NULL;

	try {
		construc->morir();
	} catch ( FinJuego &e ) {}
	this->escenario->quitarEntidad(construc);
	this->contenedor->borrarSpriteDeEntidad(construc);

	Edificio *nuevoEdificio = (Edificio*)fabricaDeEntidades->nuevaEntidad(c.tipoEdif, c.idJug, c.dni);
	nuevoEdificio->set_identificador(c.dni);
	nuevoEdificio->set_id_jugador(c.idJug);
	nuevoEdificio->setPosicion(Coordenada(c.x,c.y));
	nuevoEdificio->sufrirGolpe(50 - c.vidaRestante); // hardcodeo de vida inicial de una construcción

	if (c.idJug == this->getIDJugador())
		this->jugador->guardarConstruccionTerminada(nuevoEdificio);
	else {
		for (vector<Edificio*>::iterator it = this->edificiosEnemigos->begin(); it < this->edificiosEnemigos->end(); ++it)
			if ((*it)->get_identificador() == c.dni && (*it)->perteneceAJugador(c.idJug)) {
				this->edificiosEnemigos->erase(it);
				break;
			}
		this->edificiosEnemigos->push_back(nuevoEdificio);
	}
	delete construc;
	return nuevoEdificio;
}

/********************************************************************************/

vector<Entidad*> Juego::revisarMuertos() {
	// Propios
	vector<Entidad*> funeral = this->jugador->revisarMuertosPropios();
	// Enemigos
	for (std::vector<Unidad*>::iterator uniIt = this->unidadesEnemigos->begin(); uniIt < this->unidadesEnemigos->end(); ) {
//		if (!(*uniIt)->sigueViva()) {
		if ((*uniIt)->getEstado() == MUERTO) {
			Unidad* moribundo = *uniIt;
			unidadesEnemigos->erase(uniIt);
			funeral.push_back(moribundo);
		} else ++uniIt;
	}
	for (std::vector<Edificio*>::iterator ediIt = this->edificiosEnemigos->begin(); ediIt < this->edificiosEnemigos->end(); ) {
		if (!(*ediIt)->sigueViva()) {
			Edificio* dilapidado = *ediIt;
			edificiosEnemigos->erase(ediIt);
			funeral.push_back(dilapidado);
		} else ++ediIt;
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

	ejecutor->olvidarInteraccion();
	getSpriteDeEntidad(ejecutor)->activarMovimiento(false);
	iniciarInteraccionEntre(ejecutor, receptor);
}

/********************************************************************************/
void Juego::interaccionesDeUnidades() {
	vector<Unidad*> all_unidades = this->jugador->getUnidades();
	for(vector<Unidad*>::iterator it = this->unidadesEnemigos->begin(); it < this->unidadesEnemigos->end(); ++it)
		all_unidades.push_back(*it);

	for (std::vector<Unidad*>::iterator uniIt = all_unidades.begin(); uniIt < all_unidades.end(); ++uniIt) {
		try {
			(*uniIt)->interactuar();
			// Analizamos si se realizo un ataque de largo alcance (ARQUERO) [HORRIBLE, pero bue...]
			if ((*uniIt)->getTipo() == ARQUERO && (*uniIt)->emitioAtaque()){
				//std::cout <<"Arquero emitiendo flecha\n";
				Sprite* spriteArquero = contenedor->getSpriteDeEntidad((*uniIt));
				Flecha* flecha = new Flecha((*uniIt),spriteArquero->getDireccion());
				Coordenada coord_ceros(*cero_x,*cero_y);
				// El sprite de ataque no se guarda en Contenedor De Recursos
				// sino temporalmente en Juego
				Sprite* spriteFlecha = contenedor->generarSpriteDeAtaque(flecha,spriteArquero->getDireccion(),coord_ceros,escenario);
				spriteFlecha->activarMovimiento(true);
				mapAtaquesLargaDistancia->insert(flecha,spriteFlecha);
			}

		} catch ( UnidadDebeAcercarse &u ) {
			Sprite *sprite = this->contenedor->getSpriteDeEntidad(*uniIt);
			if (!sprite->estaEnMovimiento() || !sprite->quedaCaminoPorRecorrer()) {
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
			if (this->escenario->agregarEntidad(Coordenada(c.x,c.y), edif))
				this->contenedor->generarYGuardarSpriteEntidad(edif, Coordenada(*cero_x,*cero_y), escenario);
			else {
				Log::imprimirALog(ERR, "Error al terminar una construcción");
				delete edif;
			}

		} catch ( FinJuego &e ) {
			if (this->modo_de_juego == e.objetivo && modo_de_juego == CAPTURAR_BANDERA) {
				Proxy::enviarConversion(this->connection, this->getIDJugador(), (*uniIt)->getIDJug());
				conversionDeEnemigo(this->getIDJugador(), (*uniIt)->getIDJug());//
			}
		}
	}
}

/********************************************************************************/
void Juego::verificacionDeEstadoYColisionesDeAtaques(){
	map<Entidad*,Sprite*>::iterator itAtaque = mapAtaquesLargaDistancia->begin();
	while (itAtaque != mapAtaquesLargaDistancia->end()){
		Entidad* objetoAtaque = itAtaque->first;
		Sprite* spriteObjetoAtaque = itAtaque->second;
		Coordenada c_ataque = itAtaque->first->getPosicion();
		// Analizar si tile_actual es NULL
		Tile* tile_actual = escenario->getTile(c_ataque);
		if (tile_actual == NULL){
			mapAtaquesLargaDistancia->erase(objetoAtaque);
			delete spriteObjetoAtaque;
			delete objetoAtaque;
			itAtaque++;
			continue;
		}
		Entidad* receptor = escenario->obtenerEntidadOcupadoraEnTile(tile_actual);
		if (receptor != NULL){
			Sprite* spriteReceptor = contenedor->getSpriteDeEntidad(receptor);
			bool hayColision = spriteObjetoAtaque->checkColision(spriteReceptor);
			// El casteo a tipo flecha es temporal, podemos mejorarlo con una clase ObjetoAtaque (que herede de Entidad)
			if (hayColision || ((Flecha*)objetoAtaque)->llegoASuLimite()){

				if (hayColision && receptor->getIDJug() != idJug){
					((Flecha*)objetoAtaque)->lastimar(receptor);
					std::cout <<" colisión detectada en x : "<<c_ataque.x<<"  y : "<<c_ataque.y<<"\n";
				}
				mapAtaquesLargaDistancia->erase(objetoAtaque);
				delete spriteObjetoAtaque;
				delete objetoAtaque;
			}
		}
		itAtaque++;

	}
}

Map<Entidad*,Sprite*>* Juego::getMapObjetosAtacantes(){
	return mapAtaquesLargaDistancia;
}

/********************************************************************************/
void Juego::continuar() {
	verificacionDeEstadoYColisionesDeAtaques();

	interaccionesDeUnidades();

	// Limpiar muertes
	vector<Entidad*> funeral = revisarMuertos(); // Esto abarca todas las entidades.
	for (vector<Entidad*>::iterator it = funeral.begin(); it < funeral.end(); ++it) {
		Entidad* muerto = *it;
		this->emitirSonido(muerto);
		try {
			muerto->morir();
		} catch ( FinJuego &e ) {
			if (muerto->perteneceAJugador(this->getIDJugador()))
				verificarObjetivoPartida(e.objetivo);
		}

		if (muerto == escenario->getEntidadSeleccionada())
			escenario->setearTileClic(NULL, Coordenada(0,0));
		this->jugador->limpiarSeleccionDeUnidadMuerta((Unidad*)muerto);

		this->escenario->quitarEntidad(muerto);
		this->contenedor->borrarSpriteDeEntidad(muerto);

		ejecutoresOlvidarInteraccionCon(muerto);
		//Si la entidad muerta se convierte en un recurso, lo colocamos:
		reemplazarEntidadPorRecurso(muerto);

		delete muerto;
	}

	if (this->jugador->noTieneEntidades())
		Proxy::enviar(this->connection, this->getIDJugador());
}

/***************************************************/

void Juego::emitirSonido(Entidad* entidad){
	Mix_Chunk *chunk;
	//FALTAN CARGAR.
	switch (entidad->getTipo()){
		case REY:
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
void Juego::apagarEnemigo(int id_jugador) {
	if (id_jugador == this->getIDJugador())
		anunciarGanador(-1);
	else {
		for (vector<Unidad*>::iterator it = this->unidadesEnemigos->begin(); it < this->unidadesEnemigos->end(); ++it)
			if ((*it)->perteneceAJugador(id_jugador)) {
				try {
					(*it)->sufrirGolpe( (*it)->getVidaRestante() );
				} catch ( EntidadMurio &e ) {
					ejecutoresOlvidarInteraccionCon(*it);
				}
			}
	}
}

/***************************************************/
// Solo unidades, parece ser lo que dice el enunciado.
// TODO: probar para dos id != al propio
void Juego::conversionDeEnemigo(int id_conversor, int id_convertido) {
	if (id_convertido == this->getIDJugador()) {
		anunciarGanador(-1);	// si te convierten, simplemente perdés.
		return;
	}
	vector<Unidad*> unidadesConvertidas;
	int max_dni = 0;
	if (id_conversor == this->getIDJugador())
		max_dni = floor(jugador->getUltimoDNIdeUnidadAsignado() / 300.0) + 300;
	// Recolecta las unidades a convertir. En caso de conversión propia, la carga inmediatamente.
	for (vector<Unidad*>::iterator it = this->unidadesEnemigos->begin(); it < this->unidadesEnemigos->end(); ) {
		Unidad* uni = *it;
		if (uni->perteneceAJugador(id_conversor)) {
			max_dni = (max_dni > uni->get_identificador()) ? max_dni : uni->get_identificador();
		}
		else if (uni->perteneceAJugador(id_convertido)) {
			if (id_conversor == this->getIDJugador()) {
				uni->set_identificador(++max_dni);
				this->jugador->agregarNuevaUnidad(*it);
			} else {
				unidadesConvertidas.push_back(uni);
			}
			this->unidadesEnemigos->erase(it);
			continue;
		}
		++it;
	}
	// Caso contrario, se consiguió el mayor dni de las unidades del jugador convertido
	//  y se obtiene un dni seguro desde el cual "subir".
	max_dni = floor(max_dni / 300.0) + 300;
	for (vector<Unidad*>::iterator it = unidadesConvertidas.begin(); it < unidadesConvertidas.end(); ++it) {
		Unidad* uni = *it;
		uni->olvidarInteraccion();
		uni->set_id_jugador(id_conversor);
		uni->set_identificador(++max_dni);
		cargarEnemigo(uni);
	}
	unidadesConvertidas.clear();
}
// Explicación de DNI: en caso de que la última unidad haya sido asesinada, distintos jugadores asignarían
// distintos dni a las nuevas unidades convertidas. Ergo, se saltea a un número mayor para casi garantizar compatibilidad.

/***************************************************/

void Juego::verificarObjetivoPartida(ObjetivoEscenario oe) {
	if (!esCliente() || modo_de_juego == MODO_DEFAULT || oe != this->modo_de_juego)
		return;
	switch (modo_de_juego) {
	case DESTRUIR_CENTRO_CIVICO:
	case PARTIDA_REGICIDA:
		Proxy::enviar(this->connection, this->getIDJugador());
		this->olvidarConnection();
		break;
	//case CAPTURAR_BANDERA:	SE HACE DESDE EL CAPTURADOR
	default: break;
	}
}

void Juego::anunciarGanador(int id_ganador) {
	if (id_ganador == this->idJug) {
		std::cout << "HAS GANADO, EREIS UN CAMPEÓN"<<std::endl;
	} else if (id_ganador == 0) {
		std::cout << "Sos tan malo que perdiste contra vos mismo."<<std::endl;//
	} else {
		std::cout << "Mejor suerte la próxima"<<std::endl;
	}
	sleep(5);
	olvidarConnection(); // Esto en sí termina el juego
}

/***************************************************/

Juego::~Juego() {
	this->unidadesEnemigos->clear();	// escenario->posicionesEntidades posee todas
	this->edificiosEnemigos->clear();	//  las entidades y se ocupa de eliminarlas

	delete this->escenario;
	delete this->contenedor;
	delete this->barraEstado;
	delete this->contenedorSonidos;
	delete this->fabricaDeEntidades;

	map<Entidad*,Sprite*>::iterator p = mapAtaquesLargaDistancia->begin();
	while (p != mapAtaquesLargaDistancia->end()){
		Entidad* objetoAtaque = p->first;
		Sprite* spriteObjetoAtaque = p->second;
		p++;
		delete objetoAtaque;
		delete spriteObjetoAtaque;
	}
	delete mapAtaquesLargaDistancia;

	delete jugador;
}
