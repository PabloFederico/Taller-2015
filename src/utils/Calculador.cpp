/*
 * Calculador.cpp
 *
 *  Created on: 16 de sept. de 2015
 *      Author: emanuel
 */

#include "../utils/Calculador.h"

#include <iostream>

Coordenada Calculador::calcularPosicionRelativa(Coordenada coord_tile, Coordenada coord_ceros_pixel, Escenario *escenario){
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
	int y_result = coord_pixel_dest.y - coord_pixel_orig.y;//y_dest - y_orig;

	if (abs(x_result) < 15){
		if (y_result < 0) direccion = NORTE;
		else direccion = SUR;
	}
	else if (abs(y_result) < 10){
			if (x_result < 0) direccion = OESTE;
				else direccion = ESTE;
	}
	else if (x_result > 0){
			if (y_result < 0) direccion = NORESTE;
				else direccion = SURESTE;
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

// PRE: Chequeo de destino ocupable; posiciones en píxeles. POST: camino posee pares de posiciones EN TILES que debe recorrer secuencialmente.
//std::vector< Coordenada > Calculador::obtenerCaminoMin(Escenario *esc, int inic_x, int inic_y, int dest_x, int dest_y, int cero_x, int cero_y) {
Camino Calculador::obtenerCaminoMin(Escenario *esc, Coordenada coord_pixel_orig, Coordenada coord_pixel_dest, Coordenada coord_ceros) {
	Camino camino;
	Coordenada pos_tile_inicial, pos_tile_destino;
	try {
		pos_tile_inicial = tileParaPixel(coord_pixel_orig, coord_ceros);
		pos_tile_destino = tileParaPixel(coord_pixel_dest, coord_ceros);

		if ((!esc->tileEsOcupable(pos_tile_destino)) || (pos_tile_inicial == pos_tile_destino))
			return camino;
		/*if (!esc->tileEsOcupable(pos_tile_destino))
			return camino;
		else if (pos_tile_inicial == pos_tile_destino) {
			camino.agregar(coord_pixel_dest);
			return camino;
		}*/
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
					if ( (!pActual->padre || !pActual->padre->esTile(c)) && (!pActual->esTile(c)) && esc->tileEsOcupable(c) ) {

						if (c == pos_tile_destino) {
							Nodo nodoAux(c, pActual, pos_tile_destino);
							if ((!encontrado) || nodoAux.f() < nodoFinal.f())
								nodoFinal = nodoAux;
							encontrado = true;

						} else if (std::find_if(visitados.begin(), visitados.end(), Nodo::CmpPointerXY(c)) == visitados.end()) {
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
				//camino.agregar( pixelCentralDeTile(pActual->pos, coord_ceros) );
				camino.agregar( pActual->pos );
				pActual = pActual->padre;
			}
			camino.invertir();
			//camino.agregar(coord_pixel_dest);
			camino.agregar( pos_tile_destino );
	}

	for (pActualIt = visitados.begin(); pActualIt < visitados.end(); ++pActualIt)
		delete *pActualIt;
	for (pActualIt =   vecinos.begin(); pActualIt <   vecinos.end(); ++pActualIt)
		delete *pActualIt;
	visitados.clear();
	vecinos.clear();

	return camino;
	// CAMBIADO A CAMINO DE TILES
}

Coordenada generarPosRandom(int size_x_final ,int size_x_inicial, int size_y_final, int size_y_inicial){
	int x_rand, y_rand;
	srand((int) time(0)); //seedeo el random bien
	x_rand = (rand() % size_x_final  )+ size_x_inicial;
	y_rand = (rand() % size_y_final) + size_y_inicial;


	return Coordenada(x_rand,y_rand);
}
