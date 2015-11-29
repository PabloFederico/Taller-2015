/*
 * Calculador.cpp
 *
 *  Created on: 16 de sept. de 2015
 *      Author: emanuel
 */

#include "../utils/Calculador.h"

#include <iostream>

Coordenada Calculador::calcularPosicionRelativa(Coordenada coord_tile, Coordenada coord_ceros_pixel){
	int x_nuevo, y_nuevo;

	//Posicionamiento en el eje y relativo
	x_nuevo = coord_ceros_pixel.x - coord_tile.y * DISTANCIA_ENTRE_X;
	y_nuevo = coord_ceros_pixel.y + coord_tile.y * DISTANCIA_ENTRE_Y;

	//posicionamiento en el eje x relativo
	x_nuevo = x_nuevo + coord_tile.x * DISTANCIA_ENTRE_X;
	y_nuevo = y_nuevo + coord_tile.x * DISTANCIA_ENTRE_Y;

	return Coordenada(x_nuevo,y_nuevo);
}

Coordenada Calculador::calcularPosicionInversa(Coordenada coord_pixel, Coordenada coord_ceros_pixel, Escenario *escenario){
	int tiles_x = escenario->getDimension().first;
	int tiles_y = escenario->getDimension().second;

	int tileX, tileY;
	bool salir = false;
	int x_relativo;
	int y_relativo;
	int cero_relativo_x = coord_ceros_pixel.x;
	int cero_relativo_y = coord_ceros_pixel.y;
	for (int i = 0; i < tiles_x && !salir; i++){
		x_relativo = cero_relativo_x;
		y_relativo = cero_relativo_y;
		for(int j = 0; j < tiles_y && !salir; j++){
			if (((coord_pixel.x > x_relativo) && (coord_pixel.x < x_relativo + ANCHO_PIXEL_PASTO)) && ((coord_pixel.y > y_relativo) && (coord_pixel.y < y_relativo + ALTO_PIXEL_PASTO))){
				tileX = i;
				tileY = j;
				salir = true;
			}
			x_relativo += DISTANCIA_ENTRE_X;
			y_relativo += DISTANCIA_ENTRE_Y;
		}
		cero_relativo_x -= DISTANCIA_ENTRE_X;
		cero_relativo_y += DISTANCIA_ENTRE_Y;

	}
	return Coordenada(tileX,tileY);
}

double Calculador::calcularDistanciaEntrePixeles(Coordenada pixel1, Coordenada pixel2){	// ESTO TIENE FLOOR, POR QUÉ DOUBLE?
	double DifferenceX = pixel1.x - pixel2.x;
    double DifferenceY = pixel1.y - pixel2.y;
    double distance = floor(sqrt((DifferenceX * DifferenceX) + (DifferenceY * DifferenceY)));
    return distance;
}

Direccion Calculador::calcularDireccion(Coordenada coord_pixel_dest, Coordenada coord_pixel_orig){
	Direccion direccion;
	int x_result = coord_pixel_dest.x - coord_pixel_orig.x; //x_dest - x_orig;
	int y_result = coord_pixel_dest.y - coord_pixel_orig.y; //y_dest - y_orig;

	if (abs(x_result) < 20){
		if (y_result < 0) direccion = NORTE;
		else direccion = SUR;
	}
	else if (abs(y_result) < 15){
			if (x_result < 0) direccion = OESTE;
				else direccion = ESTE;
	}
	else if (x_result > 0){
			if (y_result > 0) direccion = SURESTE;
				else direccion = NORESTE;
	}else{
		  if (y_result < 0) direccion = NOROESTE;
		  else direccion = SUROESTE;
	}

	return direccion;
}

bool Calculador::puntoContenidoEnEscenario(Coordenada coord_tile, Escenario *escenario){
	int size_x = escenario->getDimension().first;
	int size_y = escenario->getDimension().second;

	if ( coord_tile.x >= 0  &&  coord_tile.x < size_x  && coord_tile.y >= 0  &&  coord_tile.y < size_y )
		return true;

	return false;
/*
	int x_relativo;
	int y_relativo;
	int cero_relativo_x = coord_ceros_pixel.x;
	int cero_relativo_y = coord_ceros_pixel.y;
	for (int i = 0; i < size_x; i++){
		x_relativo = cero_relativo_x;
		y_relativo = cero_relativo_y;
		for(int j = 0; j < size_y; j++){
			if (((coord_pixel.x > x_relativo) && (coord_pixel.x < x_relativo + ANCHO_PIXEL_PASTO)) && ((coord_pixel.y > y_relativo) && (coord_pixel.y < y_relativo + ALTO_PIXEL_PASTO))){
				return true;
			}
			x_relativo += DISTANCIA_ENTRE_X;
			y_relativo += DISTANCIA_ENTRE_Y;
		}
		cero_relativo_x -= DISTANCIA_ENTRE_X;
		cero_relativo_y += DISTANCIA_ENTRE_Y;

	}
	return false;
	*/
}

// No lanza FueraDeEscenario.
Coordenada Calculador::tileParaCualquierPixel(Coordenada coord_pixel, Coordenada coord_ceros) {
	int px =  coord_pixel.x - coord_ceros.x;
	int py = (coord_pixel.y - coord_ceros.y) * (ANCHO_PIXEL_PASTO / ALTO_PIXEL_PASTO);
	int tile_x = floor( 1.0*(px+py) / ANCHO_PIXEL_PASTO );
	int tile_y = floor( 1.0*(py-px) / ANCHO_PIXEL_PASTO );

	return Coordenada(tile_x,tile_y);
}


// Calculado con el píxel (0;0) en la esquina superior del tile (0;0). Atrapar FueraDeEscenario.
Coordenada Calculador::tileParaPixel(Coordenada coord_pixel, Coordenada coord_ceros) {
	int px =  coord_pixel.x - coord_ceros.x;
	int py = (coord_pixel.y - coord_ceros.y) * (ANCHO_PIXEL_PASTO / ALTO_PIXEL_PASTO);
	int tile_x = floor( 1.0*(px+py) / ANCHO_PIXEL_PASTO );
	int tile_y = floor( 1.0*(py-px) / ANCHO_PIXEL_PASTO );

	if (tile_x < 0 || tile_y < 0)//|| tile_x >= this->tiles_x || tile_y >= this->tiles_y)
		throw FueraDeEscenario();
	return Coordenada(tile_x,tile_y);
}

// Atrapar FueraDeEscenario.
Coordenada Calculador::pixelCentralDeTile(Coordenada coord_tile, Coordenada coord_ceros) {
	if (coord_tile.x < 0 || coord_tile.y < 0)// || tile_x >= this->tiles_x || tile_y >= this->tiles_y)
		throw FueraDeEscenario();
	int pix_x = coord_ceros.x + 	(coord_tile.x - coord_tile.y) * DISTANCIA_ENTRE_X;
	int pix_y = coord_ceros.y + (1 + coord_tile.x + coord_tile.y) * DISTANCIA_ENTRE_Y;
	return Coordenada(pix_x, pix_y);
}


// No lanza FueraDeEscenario.
Coordenada Calculador::pixelCentralDeCualquierTile(Coordenada coord_tile, Coordenada coord_ceros) {
	int pix_x = coord_ceros.x + 	(coord_tile.x - coord_tile.y) * DISTANCIA_ENTRE_X;
	int pix_y = coord_ceros.y + (1 + coord_tile.x + coord_tile.y) * DISTANCIA_ENTRE_Y;
	return Coordenada(pix_x, pix_y);
}


float distEuclidiana(Coordenada a, Coordenada z) {
	return sqrt( pow(z.x-a.x,2) + pow(z.y-a.y,2) );
}

struct Nodo {
	Coordenada pos;
	Nodo *padre;
	int g,h;
	Nodo(Coordenada coord, Nodo *nodoPadre, Coordenada pos_dest): pos(coord), padre(nodoPadre) {
		this->h = 10*distEuclidiana(this->pos, pos_dest);
		if (nodoPadre != NULL)
			this->g = nodoPadre->g + 10*distEuclidiana(nodoPadre->pos, this->pos);
		else this->g = 0;
	}
	Nodo(): padre(NULL), g(0), h(0) {} // No usar! jaja.
	bool guardarMenorG(Nodo *nuevoPadre) {
		int nuevaG = nuevoPadre->g + 10*distEuclidiana(nuevoPadre->pos, this->pos);
		if (nuevaG < this->g) {
			this->g = nuevaG;
			this->padre = nuevoPadre;
			return true;
		}
		return false;
	}
	int f() const { return g+h; }
	bool operator< (const Nodo & r) const {
		return (this->f() < r.f());
	}
	bool esTile(const Nodo* r) {
		return (this->pos == r->pos);
	}
	bool esTile(Coordenada coord) const {
		return (this->pos == coord);
	}
	struct CmpPointersF {
		bool operator()(const Nodo* l, const Nodo* r) {
			return (*l < *r);
		}
	};
	struct CmpNodoVsPointerF {
		bool operator()(const Nodo* l, const Nodo r) {
			return (l->f() < r.f());
		}
		bool operator()(const Nodo l, const Nodo* r) {
			return (l.f() < r->f());
		}
	};
	struct CmpPointerXY {
	  explicit CmpPointerXY(Coordenada c): coord(c) { }
	  inline bool operator()(const Nodo* l) const { return (*l).esTile(coord); }
	private:
	  Coordenada coord;
	};
};

// PRE: posiciones en tiles. POST: Devuelve camino que termina en primera posición que está a distancia euclidiana <= rangoAtaque.
Camino Calculador::obtenerCaminoMinParaAcercarse(Escenario *esc, Coordenada coord_tile_orig, Coordenada coord_tile_dest, Coordenada coord_ceros, int rango_accion) {
	Coordenada c_actual;
	Camino camino, caminoCompleto = obtenerCaminoMin(esc, coord_tile_orig, coord_tile_dest, coord_ceros, false);

	if (caminoCompleto.empty()) return caminoCompleto;
	do {
		c_actual = caminoCompleto.sacarProximaCoordenada();
		camino.agregar(c_actual);
	} while (distEuclidiana(c_actual, coord_tile_dest) > (rango_accion + 1) && !caminoCompleto.empty());

	return camino;
}

// PRE: coord_en_pixeles=false si se pasan posiciones en tiles. POST: camino posee pares de posiciones EN TILES que debe recorrer secuencialmente.
Camino Calculador::obtenerCaminoMin(Escenario *esc, Coordenada coord_orig, Coordenada coord_dest, Coordenada coord_ceros, bool coord_en_pixeles) {
	Camino camino;
	Coordenada pos_tile_inicial, pos_tile_destino;
	try {
		if (coord_en_pixeles) {
			pos_tile_inicial = tileParaPixel(coord_orig, coord_ceros);
			pos_tile_destino = tileParaPixel(coord_dest, coord_ceros);
		} else {
			pos_tile_inicial = coord_orig;
			pos_tile_destino = coord_dest;
		}

		//if ((!esc->tileEsOcupable(pos_tile_destino)) || (pos_tile_inicial == pos_tile_destino))
		if ((pos_tile_inicial == pos_tile_destino) || ((!esc->tileEsOcupable(pos_tile_destino)) && distEuclidiana(pos_tile_inicial, pos_tile_destino) < 2))
			return camino;
	} catch ( FueraDeEscenario &e ) {
		return camino;
	}

	std::vector<Nodo*> visitados, vecinos;
	Nodo *tile_inicial = new Nodo(pos_tile_inicial, NULL, pos_tile_destino);
	Nodo nodoFinal;
	vecinos.push_back(tile_inicial);
	std::vector<Nodo*>::iterator it, pActualIt;
	Nodo *pActual;

	try {
		while (!vecinos.empty()) {
			bool encontrado = false;
			Coordenada c;
			pActualIt = vecinos.begin();
			pActual = (*pActualIt);

			for (c.y = pActual->pos.y-1; c.y <= pActual->pos.y+1; c.y++) {
				for (c.x = pActual->pos.x-1; c.x <= pActual->pos.x+1; c.x++) {
					if ( (!pActual->padre || !pActual->padre->esTile(c)) && (!pActual->esTile(c)) ) {

						if (c == pos_tile_destino) {
							Nodo nodoAux(c, pActual, pos_tile_destino);
							if ((!encontrado) || nodoAux.f() < nodoFinal.f())
								nodoFinal = nodoAux;
							encontrado = true;

						} else if (esc->tileEsOcupable(c) && std::find_if(visitados.begin(), visitados.end(), Nodo::CmpPointerXY(c)) == visitados.end()) {
							it = std::find_if(vecinos.begin(), vecinos.end(), Nodo::CmpPointerXY(c));
							if (it == vecinos.end()) {
								std::vector<Nodo*>::iterator itV = std::lower_bound(vecinos.begin(), vecinos.end(), Nodo(c, pActual, pos_tile_destino), Nodo::CmpNodoVsPointerF());
								vecinos.insert(itV, new Nodo(c, pActual, pos_tile_destino));
							} else if ((*it)->guardarMenorG(pActual)) {
								Nodo *ppVecino = *it;
								vecinos.erase(it);
								it = std::lower_bound(vecinos.begin(), vecinos.end(), ppVecino, Nodo::CmpPointersF());
								vecinos.insert(it, ppVecino);
							}
						}

					}
				}
			}
			if (encontrado) {
				pActual = nodoFinal.padre;
				throw DestinoEncontrado();
			}
			visitados.push_back(pActual);
			pActualIt = std::find_if(vecinos.begin(), vecinos.end(), Nodo::CmpPointerXY(pActual->pos));
			vecinos.erase(pActualIt);
		}
	} catch ( DestinoEncontrado &e ) {	// pActual tiene ahora el último tile del camino, NO el destino.
		while (!pActual->esTile(tile_inicial)) {
				camino.agregar( pActual->pos );
				pActual = pActual->padre;
			}
			camino.invertir();
			if (esc->tileEsOcupable(pos_tile_destino))
				camino.agregar( pos_tile_destino );
			// Nuevo: En el caso de que se pueda hacer el camino entero hasta el lugar anterior porque el destino está ocupado, lo hará.
	}

	for (pActualIt = visitados.begin(); pActualIt < visitados.end(); ++pActualIt)
		delete *pActualIt;
	for (pActualIt =   vecinos.begin(); pActualIt <   vecinos.end(); ++pActualIt)
		delete *pActualIt;
	visitados.clear();
	vecinos.clear();

	return camino;
}


Coordenada Calculador::generarPosRandom(int size_x_final, int size_x_inicial, int size_y_final, int size_y_inicial, int seed = 0){
	int x_rand, y_rand;
	srand((time(0)) * SDL_GetTicks());	// Esto FRUSTRA el randomness, siempre va a dar lo mismo, porque vuelve al número múltiplo de 50...
	x_rand = (rand() % (size_x_final - size_x_inicial)) + size_x_inicial;
	y_rand = (rand() % (size_y_final - size_y_inicial)) + size_y_inicial;

	return Coordenada(x_rand,y_rand);
}

Coordenada Calculador::generarPosRandomDentroDeEscenarioConLimites(int size_x, int x_max, int x_min, int size_y, int y_max, int y_min, int seed = 3) {
	Coordenada c = generarPosRandom(x_max, x_min, y_max, y_min, seed);
	return Coordenada(ChequeoDeBorde(size_x, c.x), ChequeoDeBorde(size_y, c.y));
}

int Calculador::ChequeoDeBorde(int max, int input) {
	if (input >= 0) {
		if (input <= max)
			return input;
		else return max;
	} else return 0;
}

Coordenada Calculador::obtenerCoordenadaLibreCercaDeEdificio(Edificio* edificio,Escenario* escenario){
	Coordenada c_edificio = edificio->getPosicion();
	pair<int,int> tam = edificio->getTam();
	int max_distancia = 3;
	int x_ini = c_edificio.x - max_distancia;
	int y_ini = c_edificio.y - max_distancia;
	int x_fin = c_edificio.x+tam.first+max_distancia;
	int y_fin = c_edificio.y+tam.second+max_distancia;
	Coordenada c_inicial(x_ini, y_ini);
	Coordenada c_final(x_fin, y_fin);
	for (Coordenada c1 = c_inicial; c1.y <= c_final.y; c1.y++){
		for (Coordenada c2 = c1; c2.x <= c_final.x; c2.x++){
			if (puntoContenidoEnEscenario(c2,escenario)){
				Tile* tile = escenario->getTile(c2);
				if (tile->estaLibre())
					return c2;
			}
		}
	}

	return Coordenada(-1,-1);
}
