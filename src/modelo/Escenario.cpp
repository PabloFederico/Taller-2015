/*
 * Escenario.cpp
 *
 *  Created on: 2 de sept. de 2015
 *      Author: pgfederi
 */
#include "../modelo/Escenario.h"



Escenario::Escenario(InfoEscenario infoEsc, EntidadFactory *fabrica, vector<Unidad*>* unidadesMalvadas, vector<Edificio*>* edificiosMalvados):
		fabricaDeEntidades(fabrica), unidadesEnemigos(unidadesMalvadas), edificiosEnemigos(edificiosMalvados) {
	this->size_x = infoEsc.size_x;
	this->size_y = infoEsc.size_y;

	matriz_tiles = new Tile**[size_x];
	this->inicializarMatrizTiles();

	this->capa = new CapaFog(size_x,size_y);

	this->posicionesEntidades = new vector<Entidad*>();

	vector<PosTipoEntidad> vecPosTipoEntidades = infoEsc.getPosicionesEntidades();
	for (unsigned i = 0; i < vecPosTipoEntidades.size(); i++){
		int x = vecPosTipoEntidades[i].x;
		int y = vecPosTipoEntidades[i].y;
		TipoEntidad tipo = vecPosTipoEntidades[i].tipo;
		Entidad *entidad = this->fabricaDeEntidades->nuevaEntidad(tipo);
		Coordenada pos(x,y);
		entidad->setPosicion(pos);
		this->agregarEntidad(pos,entidad);	// Si no pudo, no reintento.
	}
	infoEsc.getPosicionesEntidades().clear();
	std::cout<<"se agregaron todas las entidades"<<std::endl;
/*
	this->protagonista = this->fabricaDeEntidades->nuevaEntidad(infoEsc.protagonista);
	Coordenada posProtag(infoEsc.posX_protagonista, infoEsc.posY_protagonista);
	bool resPosicionar = false;
	try {
		resPosicionar = this->agregarEntidad(posProtag, this->protagonista);
	} catch ( FueraDeEscenario &e ) {}
	while (resPosicionar == false) {	// Genera posiciones random hasta poder agregar al protagonista.
		try {
			Coordenada posProtag = generarCoordenadaRandom(size_x, 0, size_y, 0, 9);
			resPosicionar = this->agregarEntidad(posProtag, this->protagonista);
		} catch ( FueraDeEscenario &e ) {}
	}
*/
	this->tile_clic = NULL;
	this->entidadSeleccionada = NULL;
}

/********************************************************************************/
// No libera memoria de previa matriz/
void Escenario::inicializarMatrizTiles() {
	for (int i = 0; i < size_x; i++){
		matriz_tiles[i] = new Tile*[size_y];

		for (int j = 0; j < size_y; j++){
			matriz_tiles[i][j] = new Tile();
		}
	}
}

/********************************************************************************/
pair<int,int> Escenario::getDimension(){
	return std::make_pair(this->size_x,this->size_y);
}

/********************************************************************************/
vector<Entidad*>* Escenario::getVectorEntidades(){
	return this->posicionesEntidades;
}

/********************************************************************************/
void Escenario::actualizarPosicionParaEntidad(Coordenada c, Entidad* entidad){
	/* Si las coordenadas no son iguales, actualizar la coordenada de cualquier entidad */
	if (entidad->getPosicion() != c) {
		Tile* tile = getTile(entidad->getPosicion());
		try {
			if (tile != NULL)
				tile->eliminarEntidad(entidad);
		} catch ( NoSeRecibio &e ) {}

		/* agregamos la entidad a su nuevo tile */
		tile = getTile(c.x, c.y);
		if (tile != NULL)
			tile->agregarEntidad(entidad);
		entidad->setPosicion(c);
	}
}

/********************************************************************************/
void Escenario::actualizarPosicionEnemigo(Entidad* ent, Coordenada c) {
	/* Si las coordenadas no son iguales, actualizar la coordenada del enemigo */
	actualizarPosicionParaEntidad(c,ent);
	/*
	if (!coordEnEscenario(c))
		throw FueraDeEscenario();
	vector<PosEntidad>::iterator it;
	for (it = enemigos->begin(); it < enemigos->end(); ++it)
		if (it->entidad == ent)
			break;
	if (it != enemigos->end() && it->coord() != c) {
		try {
			Tile* tile = getTile(it->coord());
			if (!tile) return;
			try {
				tile->eliminarEntidad(ent);
			} catch ( NoSeRecibio &e ) {}
			tile = getTile(c.x, c.y);
			tile->agregarEntidad(ent);
			it->x = c.x;
			it->y = c.y;
		} catch ( NoSeRecibio &e ) {
			std::cerr << "No estaba ahí, campeón."<<std::endl;
		}
	}
	*/
}

/********************************************************************************/
Tile* Escenario::getTile(int x, int y){
	if (!coordEnEscenario(Coordenada(x,y)))
		return NULL;
	return this->matriz_tiles[x][y];
}

/********************************************************************************/
Tile* Escenario::getTile(Coordenada c) {
	if (!coordEnEscenario(c))
		return NULL;
	return this->matriz_tiles[c.x][c.y];
}

/********************************************************************************/
int Escenario::getIDJug(){
	return fabricaDeEntidades->getId_Jug();
}

/********************************************************************************/
bool Escenario::agregarEntidad(Coordenada pos, Entidad* entidad){
	if (!coordEnEscenario(pos))
		throw FueraDeEscenario();
	try {
		pair<int,int> dim = entidad->getTam();
		// Se fija que todos los tiles a ocupar estén vacíos antes de ocuparlos
		for (int j = 0; j < dim.second; j++)
			for (int i = 0; i < dim.first; i++){
				Tile* tile = this->matriz_tiles[pos.x+i][pos.y+j];
				if (!tile->estaLibre())
					throw TileEstaOcupado();
			}
		for (int j = 0; j < dim.second; j++){
			for (int i = 0; i < dim.first; i++){
				Tile* tile = this->matriz_tiles[pos.x+i][pos.y+j];
				if (i==round(dim.first/2.0)-1 && j==round(dim.second/2.0)-1)
					tile->agregarEntidad(entidad, true);
				else tile->agregarEntidad(entidad);
			}
		}
		// NOT TRUE ANYMORE: Los recursos se manejan por separado, no se agregan y se borran desde el tile.
		//if (!EsRecurso(entidad->getTipo())) {
			//PosEntidad posEntidad(pos, entidad);
			//this->posicionesEntidades->push_back(posEntidad);
			posicionesEntidades->push_back(entidad);
		//}
	} catch ( TileEstaOcupado &e ) {
		Log::imprimirALog(ERR,"Se intentó agregar una entidad en un tile ocupado");
		return false;
	}
	return true;
}

/********************************************************************************/
// Hace DELETE de la posición de memoria, por lo que el puntero pasado no se debe seguir usando!!!
void Escenario::quitarEntidad(Coordenada pos, Entidad* entidad) {
	// La quita del vector posicionesEntidades.
	std::vector<Entidad*>::iterator it = std::find(this->posicionesEntidades->begin(), this->posicionesEntidades->end(), entidad);
	if (it != this->posicionesEntidades->end()) {
		this->posicionesEntidades->erase(it);
	}
	// La quita de los Tile que ocupaba.
	pair<int,int> dim = entidad->getTam();
	for (int j = 0; j < dim.second; j++)
		for (int i = 0; i < dim.first; i++) {
			Tile* tile = getTile(pos.x+i, pos.y+j);
			try {
				if (tile) tile->eliminarEntidad(entidad);
			} catch ( NoSeRecibio &e ) {}
		}
}

/********************************************************************************/
bool Escenario::coordEnEscenario(Coordenada c) {
	if (c.x < 0 || c.y < 0 || c.x >= this->size_x || c.y >= this->size_y)
			return false;
	return true;
}

/********************************************************************************/
// Verifica que (x;y) corresponde a una posición en el escenario que está desocupada.
bool Escenario::tileEsOcupable(Coordenada c) {
	if (!coordEnEscenario(c))
		return false;
	return (this->matriz_tiles[c.x][c.y]->estaLibre());
}

/********************************************************************************/
Entidad* Escenario::obtenerEntidadOcupadoraEnTile(Tile* tile) {
	Entidad* ent = NULL;
	vector<Entidad*> entidades = tile->getEntidades();
	for (unsigned i = 0; i < entidades.size(); i++) {
		if (entidades[i]->ocupaSuTile()) {
			 ent = entidades[i];	// Si hay varias (que no debería), queda la última (lo cual es preferible)
		}
	}
	return ent;
}

/********************************************************************************/
void Escenario::setearTileClic(Tile* tile, Coordenada c_tile){
	this->tile_clic = tile;					// Por qué no dejar el valor anterior en caso de NULL? (MC)
	if (tile == NULL) entidadSeleccionada = NULL;
	if ((tile != NULL) && (capa->getEstadoTile(c_tile.x, c_tile.y) == ESTADO_COLOR)) {
		 Entidad *entAux = obtenerEntidadOcupadoraEnTile(tile);
		// if (entAux != NULL)
			 this->entidadSeleccionada = entAux;
	}
}

/********************************************************************************/
Tile* Escenario::getTileClic(){
	return this->tile_clic;
}

Entidad* Escenario::getEntidadSeleccionada(){
	return this->entidadSeleccionada;
}

void Escenario::setearCoordTileClic(Coordenada c){
	this->c_tile_clic = c;
}

Coordenada Escenario::getCoordTileClic(){
	return c_tile_clic;
}

/********************************************************************************/
CapaFog* Escenario::getCapa() {
	return this->capa;
}

Coordenada Escenario::generarCoordenadaRandom(int size_x_final, int size_x_inicial, int size_y_final, int size_y_inicial, int seed = 0) {
	int x_rand, y_rand;
	srand(((int)time(0)) * SDL_GetTicks());
	x_rand = (rand() % (size_x_final - size_x_inicial)) + size_x_inicial;
	y_rand = (rand() % (size_y_final - size_y_inicial)) + size_y_inicial;

	return Coordenada(x_rand,y_rand);}


void Escenario::quitarRecurso(Coordenada c,Entidad* entidad){
	Entidad* aux = entidad;
	quitarEntidad(c,entidad);
	aux->~Entidad();
}
/********************************************************************************/
bool Escenario::tieneRecuadroSeleccion(){
	return (c_tile_ini_recuadro != c_tile_fin_recuadro);
}

/********************************************************************************/
void Escenario::agregarCoordenadasRecuadroSeleccion(Coordenada c_inicial, Coordenada c_final){
	c_tile_ini_recuadro = c_inicial;
	c_tile_fin_recuadro = c_final;
}

/********************************************************************************/
pair<Coordenada,Coordenada> Escenario::getCoordenadasRecuadro(){
	if (c_tile_ini_recuadro.y > c_tile_fin_recuadro.y || c_tile_ini_recuadro.x > c_tile_fin_recuadro.x){
		Coordenada aux = c_tile_fin_recuadro;
		c_tile_fin_recuadro = c_tile_ini_recuadro;
		c_tile_ini_recuadro = aux;
	}
	return make_pair(c_tile_ini_recuadro,c_tile_fin_recuadro);
}

/********************************************************************************/
void Escenario::quitarRecuadroSeleccion(){
	c_tile_ini_recuadro = c_tile_fin_recuadro;
}

/********************************************************************************/
vector<Entidad*> Escenario::revisarMuertosDeNadie() {
	vector<Entidad*> cuerpos;
	for (std::vector<Entidad*>::iterator entIt = this->posicionesEntidades->begin(); entIt < this->posicionesEntidades->end(); ++entIt) {
		if (!(*entIt)->sigueViva() && (*entIt)->getIDJug() == 0) {
			cuerpos.push_back(*entIt);
			//this->posicionesEntidades->erase(entIt);		//Innecesarias porque se borra en quitarEntidad
			//entIt = this->posicionesEntidades->begin(); //por las dudas
		}
	}
	return cuerpos;
}

/********************************************************************************/
Escenario::~Escenario() {
	for (unsigned i = 0; i < posicionesEntidades->size(); i++){
		Entidad* entidad = (*posicionesEntidades)[i];
		delete entidad; // meter en quitarEntidad y reemplazar acá?
	}

	this->posicionesEntidades->clear();
	delete this->posicionesEntidades;

	delete fabricaDeEntidades;

	delete this->capa;

	for (int i = 0; i < size_x; i++){
		for (int j = 0; j < size_y; j++){
			delete matriz_tiles[i][j];
		}
		delete[] matriz_tiles[i];
	}
	delete[] matriz_tiles;
}

