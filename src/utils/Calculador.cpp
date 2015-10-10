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

double Calculador::calcularDistanciaEntrePixeles(Coordenada pixel1, Coordenada pixel2){
	double DifferenceX = pixel1.x - pixel2.x;
    double DifferenceY = pixel1.y - pixel2.y;
    double distance = floor(sqrt((DifferenceX * DifferenceX) + (DifferenceY * DifferenceY)));
    return distance;
}

Direccion Calculador::calcularDireccion(Coordenada coord_pixel_dest, Coordenada coord_pixel_orig){
	Direccion direccion;
	int x_result = coord_pixel_dest.x - coord_pixel_orig.x; //x_dest - x_orig;
	int y_result = coord_pixel_dest.y - coord_pixel_orig.y;//y_dest - y_orig;

	if (abs(x_result) < 30){
		if (y_result < 0) direccion = NORTE;
		else direccion = SUR;
	}
	else if (abs(y_result) < 30){
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

bool Calculador::puntoContenidoEnEscenario(Coordenada coord_pixel, Coordenada coord_ceros_pixel, Escenario *escenario){
	int tiles_x = escenario->getDimension().first;
	int tiles_y = escenario->getDimension().second;

	bool contenido = false;
	int x_relativo;
	int y_relativo;
	int cero_relativo_x = coord_ceros_pixel.x;
	int cero_relativo_y = coord_ceros_pixel.y;
	for (int i = 0; i < tiles_x; i++){
		x_relativo = cero_relativo_x;
		y_relativo = cero_relativo_y;
		for(int j = 0; j < tiles_y; j++){
			if (((coord_pixel.x > x_relativo) && (coord_pixel.x < x_relativo + ANCHO_PIXEL_PASTO)) && ((coord_pixel.y > y_relativo) && (coord_pixel.y < y_relativo + ALTO_PIXEL_PASTO))){
				return true;
			}
			x_relativo += DISTANCIA_ENTRE_X;
			y_relativo += DISTANCIA_ENTRE_Y;
		}
		cero_relativo_x -= DISTANCIA_ENTRE_X;
		cero_relativo_y += DISTANCIA_ENTRE_Y;

	}
	return contenido;
}

// Calculado con el píxel (0;0) en la esquina superior del tile (0;0).
Coordenada Calculador::tileParaPixel(Coordenada coord_pixel, Coordenada coord_ceros_pixel) {
	int px =  coord_pixel.x - coord_ceros_pixel.x;
	int py = (coord_pixel.y - coord_ceros_pixel.y) * (ANCHO_PIXEL_PASTO / ALTO_PIXEL_PASTO);

	int tile_x = floor( (px+py) / ANCHO_PIXEL_PASTO );
	int tile_y = floor( (py-px) / ANCHO_PIXEL_PASTO );

	if (tile_x < 0 || tile_y < 0 )//|| tile_x >= this->tiles_x || tile_y >= this->tiles_y)
		throw FueraDeEscenario();
	return Coordenada(tile_x,tile_y);
}

Coordenada Calculador::pixelCentralDeTile(Coordenada coord_tile) {
	if (coord_tile.x < 0 || coord_tile.y < 0)// || tile_x >= this->tiles_x || tile_y >= this->tiles_y)
		throw FueraDeEscenario();
	int pix_x = (coord_tile.x - coord_tile.y)   * DISTANCIA_ENTRE_X;
	int pix_y = (coord_tile.x + coord_tile.y + 1) * DISTANCIA_ENTRE_Y;
	return Coordenada(pix_x,pix_y);
}



int distEuclidiana(int x0, int y0, int x1, int y1) {
	return sqrt( pow(x1-x0,2) + pow(y1-y0,2) );
}

struct Nodo {
	int x,y;
	Nodo *padre;
	int g,h;
	Nodo(int posX, int posY, Nodo *nodoPadre, int dest_x, int dest_y):
			x(posX), y(posY), padre(nodoPadre) {
		this->h = 10*distEuclidiana(posX, posY, dest_x, dest_y);
		if (padre != NULL)
			this->g = nodoPadre->g + 10*distEuclidiana(nodoPadre->x, nodoPadre->y, x, y);
		else this->g = 0;
	}
	bool guardarMenorG(Nodo *nuevoPadre) {
		int nuevaG = nuevoPadre->g + 10*distEuclidiana(nuevoPadre->x, nuevoPadre->y, x, y);
		if (nuevaG < this->g) {
			this->g = nuevaG;
			padre = nuevoPadre;
			return true;
		}
		return false;
	}
	int f() { return g+h; }
	bool operator< (const Nodo & right) {
		return (this->f() < (right.g+right.h));
	}
};

// PRE: Chequeo de destino ocupable; posiciones en píxeles. POST: camino posee pares de posiciones que debe recorrer secuencialmente.
//std::vector< Coordenada > Calculador::obtenerCaminoMin(Escenario *esc, int inic_x, int inic_y, int dest_x, int dest_y, int cero_x, int cero_y) {
std::vector< Coordenada > Calculador::obtenerCaminoMin(Escenario *esc, Coordenada coord_pixel_orig, Coordenada coord_pixel_dest, Coordenada coord_ceros_pixel){
	std::vector< Coordenada > camino;
	Coordenada pos_tile_inicial, pos_tile_destino;
	try {
		pos_tile_inicial = tileParaPixel(coord_pixel_orig,coord_ceros_pixel);
		pos_tile_destino = tileParaPixel(coord_pixel_dest,coord_ceros_pixel);
	} catch ( FueraDeEscenario &e ) {
		return camino;
	}

	std::vector<Nodo> visitados, vecinos;
	Nodo tile_inicial(pos_tile_inicial.x, pos_tile_inicial.y, NULL, pos_tile_destino.x, pos_tile_destino.y);
	vecinos.push_back(tile_inicial);
	std::vector<Nodo>::iterator pActualIt;
	Nodo *pActual;

	try {
		while (!vecinos.empty()) {
			pActualIt = vecinos.begin();
			pActual = (Nodo*)&*pActualIt;
			for (int y = pActual->y-1; y <= pActual->y+1; y++) {
				for (int x = pActual->x-1; x <= pActual->x+1; x++) {
					if ((x != pActual->padre->x || y != pActual->padre->y)
							&& (x != pActual->x || y != pActual->y) && esc->tileEsOcupable(x, y)) {
						if (x == pos_tile_destino.x && y == pos_tile_destino.y)
							throw DestinoEncontrado();

						std::vector<Nodo>::iterator it;
						for (it = vecinos.begin(); it < vecinos.end(); ++it)
							if (it->x == x && it->y == y)
								break;
						if (it == vecinos.end()) {
							Nodo pVecino(x, y, pActual, pos_tile_destino.x, pos_tile_destino.y);
							it = std::lower_bound(vecinos.begin(), vecinos.end(), pVecino);
							vecinos.insert(it, pVecino);
						} else if (it->guardarMenorG(pActual)) {
							Nodo pVecino = *it;
							vecinos.erase(it);	//esto seguro está mal, borrando el Nodo; en ese caso simplemente crear un nuevo pVecino = Nodo(...)
							it = std::lower_bound(vecinos.begin(), vecinos.end(), pVecino);
							vecinos.insert(it, pVecino);
						}
					}
				}
			}
			visitados.push_back(*pActual);
			vecinos.erase(pActualIt); //cuidado; revisar
		}
	} catch ( DestinoEncontrado &e ) {
		while (pActual->x != tile_inicial.x || pActual->y != tile_inicial.y) {
				Coordenada coord_tile(pActual->x,pActual->y);
				camino.push_back(pixelCentralDeTile(coord_tile));
				pActual = pActual->padre;
			}
			std::reverse(camino.begin(), camino.end());
			camino.push_back(coord_pixel_dest);
	}

	for (pActualIt = visitados.begin(); pActualIt < vecinos.end(); ++pActualIt) {
		if (pActualIt == visitados.end())
			pActualIt = vecinos.begin();
		Nodo *nodoAux = (Nodo*)&*pActualIt;
		delete nodoAux;
	}

	//-------pruebas--------
	//inserting at end() ?
	//erase() ?
	//----------------------

	return camino;
	// Con return: for pos in camino: moverse a pos; (Con manejo de colisiones.)
}




