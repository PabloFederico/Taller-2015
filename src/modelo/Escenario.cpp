/*
 * Escenario.cpp
 *
 *  Created on: 2 de sept. de 2015
 *      Author: pgfederi
 */
#include "../modelo/Escenario.h"
//#include "../modelo/Log.h"

Escenario::Escenario(InfoEscenario infoEsc, EntidadFactory *fabrica): fabricaDeEntidades(fabrica) {
	this->size_x = infoEsc.size_x;
	this->size_y = infoEsc.size_y;

	this->matriz_tiles = new Tile**[size_x];
	for (int i = 0; i < size_x; i++){
		this->matriz_tiles[i] = new Tile*[size_y];

		for (int j = 0; j < size_y; j++){
			this->matriz_tiles[i][j] = new Tile();
		}
	}

	this->capa = new CapaFog(size_x,size_y);

	this->posicionesEntidades = new vector<PosEntidad>();

	vector<PosTipoEntidad> vecPosTipoEntidades = infoEsc.getPosicionesEntidades();
	for (unsigned i = 0; i < vecPosTipoEntidades.size(); i++){
		int x = vecPosTipoEntidades[i].x;
		int y = vecPosTipoEntidades[i].y;
		TipoEntidad tipo = vecPosTipoEntidades[i].tipo;
		Entidad *entidad = this->fabricaDeEntidades->nuevaEntidad(tipo);
		Coordenada pos(x,y);
		this->agregarEntidad(pos,entidad);
	}

	infoEsc.getPosicionesEntidades().clear();

	this->protagonista = this->fabricaDeEntidades->nuevaEntidad(infoEsc.protagonista);
	Coordenada pos(infoEsc.posX_protagonista, infoEsc.posY_protagonista);
	this->agregarEntidad(pos, this->protagonista);
	this->c_protagonista = pos;
	this->tile_clic = NULL;
	this->entidadSeleccionada = NULL;
	//PosEntidad posEntidad(pos.x, pos.y, this->protagonista);
	//this->posicionesEntidades->push_back(posEntidad);
}

/********************************************************************************/
Entidad* Escenario::getProtagonista(){
	return this->protagonista;
}

/********************************************************************************/
pair<int,int> Escenario::getDimension(){
	return std::make_pair(this->size_x,this->size_y);
}

/********************************************************************************/
vector<PosEntidad>* Escenario::getVectorEntidades(){
	return this->posicionesEntidades;
}

/********************************************************************************/
void Escenario::actualizarPosicionProtagonista(Coordenada c){
	/* Si las coordenadas no son iguales, actualizar la coordenada del protagonista */
	if (!(c_protagonista == c)){
		Tile* tile = getTile(c_protagonista.x, c_protagonista.y);
		tile->eliminarEntidad(protagonista);

		/* agregamos al protagonista a su nuevo tile */
		tile = getTile(c.x, c.y);
		tile->agregarEntidad(protagonista);
		c_protagonista = c;
	}
}
/********************************************************************************/
Tile* Escenario::getTile(int x, int y){
	if (x < 0 || y < 0 || x >= this->size_x || y >= this->size_y)
		return NULL;
	return this->matriz_tiles[x][y];
}
/********************************************************************************/
void Escenario::agregarEntidad(Coordenada pos, Entidad* entidad){
	try {
		pair<int,int> dim = entidad->getTam();
		for (int j = 0; j < dim.second; j++)
			for (int i = 0; i < dim.first; i++){
				// pregunta si ese tile está vacío
				Tile* tile = matriz_tiles[pos.x+i][pos.y+j];
				if (!tile->estaLibre())
					throw TileEstaOcupado();
			}
		for (int j = 0; j < dim.second; j++){
			for (int i = 0; i < dim.first; i++){
				Tile* tile = this->matriz_tiles[pos.x+i][pos.y+j];
				tile->agregarEntidad(entidad);
			}
		}
		PosEntidad posEntidad(pos.x, pos.y, entidad);
		this->posicionesEntidades->push_back(posEntidad);
	} catch ( TileEstaOcupado &e ) {
		//Log::imprimirALog(ERR,"Se intento agregar una entidad en un tile ocupado");
	} // TODO: Alguna devolución de que no se pudo insertar?


}

/********************************************************************************/
void Escenario::quitarEntidad(Coordenada pos, Entidad* entidad) {
	// La quita del vector posicionesEntidades.
	PosEntidad pE(pos.x, pos.y, entidad);
	std::vector<PosEntidad>::iterator it = std::find(this->posicionesEntidades->begin(), this->posicionesEntidades->end(), pE);
	if (it != this->posicionesEntidades->end()) {
		this->posicionesEntidades->erase(it);

		// La quita de los Tile que ocupaba.
		pair<int,int> dim = entidad->getTam();
		for (int j = 0; j < dim.second; j++)
			for (int i = 0; i < dim.first; i++) {
				Tile* tile = getTile(pos.x+i, pos.y+j);
				vector<Entidad*> entidades = tile->getEntidades();
				vector<Entidad*>::iterator it2 = find(entidades.begin(), entidades.end(), entidad);
				if (it2 != entidades.end())
					entidades.erase(it2);
			}
	} //else entidad no estaba en el vector posicionesEntidades... Algo TODO?
}

/********************************************************************************/
// Verifica que (x;y) corresponde a una posición en el escenario que está desocupada.
bool Escenario::tileEsOcupable(Coordenada c) {
	if (c.x < 0 || c.y < 0 || c.x >= this->size_x || c.y >= this->size_y)
		return false;
	return (this->matriz_tiles[c.x][c.y]->estaLibre());
}

/********************************************************************************/
void Escenario::setearTileClic(Tile* tile, Coordenada c_tile){
	this->tile_clic = tile;
	if (capa->getEstadoTile(c_tile.x, c_tile.y) == ESTADO_COLOR){
		vector<Entidad*> entidades = tile->getEntidades();
		for (unsigned i = 0; i < entidades.size(); i++){
			if (entidades[i]->ocupaSuTile()){
				this->entidadSeleccionada = entidades[i];
			}
		}
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

/********************************************************************************/
Escenario::~Escenario() {
	for (unsigned i = 0; i < posicionesEntidades->size(); i++){
		Entidad* entidad = (*posicionesEntidades)[i].entidad;
		delete entidad; // meter en quitarEntidad y reemplazar acá?
	}

	this->posicionesEntidades->clear();
	delete this->posicionesEntidades;

	delete this->capa;

	for (int i = 0; i < size_x; i++){
		for (int j = 0; j < size_y; j++){
			delete matriz_tiles[i][j];
		}
		delete[] matriz_tiles[i];
	}
	delete[] matriz_tiles;
}

