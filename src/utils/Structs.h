/*
 * Structs.h
 *
 *  Created on: 26 de sept. de 2015
 *      Author: emanuel
 */

#ifndef UTILS_STRUCTS_H_
#define UTILS_STRUCTS_H_
#include <SDL2/SDL.h>
#include <vector>
#include <sstream>
#include <algorithm>
#include "../modelo/Exceptions.h"
#include "../utils/Constantes.h"
#include "../modelo/Entidad.h"

/* Estructura para guardar el Tipo de Entidad que se encuentra en las
 *  coordenadas x,y */
struct PosTipoEntidad{
	int x;
	int y;
	TipoEntidad tipo;
	PosTipoEntidad(int x, int y, TipoEntidad tipo){
		this->x = x;
		this->y = y;
		this->tipo = tipo;
	};

	// Encodeado: "tipo(x;y)"
	std::string enc() {
		ostringstream Encode;
		Encode << tipo<<"("<<x<<";"<<y<<")";
		return Encode.str();
	}
	static PosTipoEntidad dec(std::string s) {
		std::stringstream ss(s);
		int x,y,tipo;
		ss >> tipo;
		ss.ignore();
		ss >> x;
		ss.ignore();
		ss >> y;
		return PosTipoEntidad(x, y, TipoEntidad(tipo));
	}
};

/* Estructura para guardar información para Escenario */
struct InfoEscenario{
	int size_x;
	int size_y;
	vector<PosTipoEntidad> posTipoEntidades;
	TipoEntidad protagonista;
	int posX_protagonista;
	int posY_protagonista;

	InfoEscenario(){
		size_x = 1;
		size_y = 1;
		protagonista = SOLDADO;
		posX_protagonista = 0;
		posY_protagonista = 0;
	};

	void agregarEntidad(pair<int,int> pos, TipoEntidad tipo){
		PosTipoEntidad posTipoEnte(pos.first,pos.second,tipo);
		this->posTipoEntidades.push_back(posTipoEnte);
	};

	vector<PosTipoEntidad> getPosicionesEntidades(){
		return this->posTipoEntidades;
	}

	bool operator!() {
		return !((size_x > 0) && (size_y > 0) && (posX_protagonista >= 0) && (posY_protagonista >= 0));
	}

	// Encodeado: "x;y|[...,PTE,...,]|protTE"
	std::string enc() {
		ostringstream Encode;
		Encode << size_x << ";" << size_y << "|[";
		for (vector<PosTipoEntidad>::iterator it = posTipoEntidades.begin(); it < posTipoEntidades.end(); ++it) {
			Encode << it->enc()<<",";
		}
		Encode << "]|"<<(PosTipoEntidad(posX_protagonista, posY_protagonista, protagonista).enc());
		return Encode.str();
	}
	static InfoEscenario dec(std::string s) {
		std::stringstream ss(s);
		InfoEscenario ie;
		ss >> ie.size_x;
		ss.ignore();
		ss >> ie.size_y;
		char cs[16];
		ss.ignore(2);
		while (ss.peek() != ']') {
			ss.get(cs, 14, ',');
			ie.posTipoEntidades.push_back(PosTipoEntidad::dec(cs));
			ss.ignore();
		}
		ss.ignore(2);
		ss.get(cs, 14);
		PosTipoEntidad prot = PosTipoEntidad::dec(cs);
		ie.protagonista = prot.tipo;
		ie.posX_protagonista = prot.x;
		ie.posY_protagonista = prot.y;

		return ie;
	}
};


/* Estructura para guardar el Tipo de Entidad asociado a un SDL_Rect para
 * poder dibujar sobre el renderer */
struct DataPos{
	SDL_Rect posicion;
	Entidad *entidad;
	DataPos(SDL_Rect pos, Entidad *entidad){
		this->posicion = pos;
		this->entidad = entidad;
	};
};


/* Estructura para guardar información sobre una Entidad*/
struct InfoEntidad{
	TipoEntidad tipo;
	string path;
	string descripcion;
	int ancho;
	int alto;
	int pixel_ref_x;
	int pixel_ref_y;
	int fps;
	int delay;
	InfoEntidad(){
		tipo = PASTO;
		path = "images/missing1.png";
		descripcion = "";
		ancho = 1;
		alto = 1;
		pixel_ref_x = 0;
		pixel_ref_y = 0;
		fps = 10;
		delay = 0;
	};
	bool operator==(TipoEntidad tipo) const {
		return (this->tipo == tipo);
	}
};

struct RegistroPosicion{
	int x_anterior;
	int y_anterior;
	float posX_player;
	float posY_player;
	RegistroPosicion(){
		x_anterior = 0;
		y_anterior = 0;
		posX_player = 0;
		posY_player = 0;
	}
};

struct Rectangulo{
	int x_ini;
	int width;
	int y_ini;
	int height;
	Rectangulo(int x1, int width, int y1, int height){
		x_ini = x1;
		this->width = width;
		y_ini = y1;
		this->height = height;
	}

	bool existeColision(Rectangulo rect){
		if ( ((x_ini + width) > rect.x_ini)     &&
			 ((rect.x_ini + rect.width) > x_ini) &&
			 ((y_ini + height) > rect.y_ini)    &&
			 ((rect.y_ini + rect.height) > y_ini) ){

			return true;
		}
		return false;
	}
};

struct Coordenada{
	int x,y;
	Coordenada(int x, int y){
		this->x = x;
		this->y = y;
	}

	Coordenada(){
		this->x = 0;
		this->y = 0;
	}

	bool operator==(const Coordenada & c) const {
		return (this->x == c.x && this->y == c.y);
	}

	bool operator<(const Coordenada & c) const {
		return (this->x < c.x || this->y < c.y);	//Adivino que esto está bien
	}

	// Encodeado: "x;y"
	std::string enc() {
		ostringstream Encode;
		Encode << x << ";" << y;
		return Encode.str();
	}
	static Coordenada dec(std::string s) {
		std::stringstream ss(s);
		int x,y;
		ss >> x;
		ss.ignore();
		ss >> y;
		return Coordenada(x,y);
	}
};


struct Camino {
	std::vector<Coordenada> v;

	Camino() {}
	void agregar(Coordenada c) {
		v.push_back(c);
	}
	void invertir() {
		std::reverse(v.begin(), v.end());
	}
	std::size_t size() { return v.size(); }
	Coordenada sacarProximaCoordenada() {
		if (v.empty())
			throw CaminoVacio();
		Coordenada c = v[0];
		v.erase(v.begin());
		return c;
	}
	void convertirTilesAPixeles(Coordenada coord_ceros) {
		std::vector<Coordenada> vAux;
		for(std::vector<Coordenada>::iterator it = v.begin(); it < v.end(); ++it)
			try {
				vAux.push_back( pixelCentralDeTile(*it, coord_ceros) );
			} catch ( FueraDeEscenario &e ) {}
		v.clear();
		v = vAux;
	}
	Coordenada operator[](int k) { return v[k]; }

	// Encodeado: "c1|c2|c3|...|cn"
	std::string enc() {
		ostringstream Encode;
		for (std::vector<Coordenada>::iterator it = v.begin(); it < v.end(); ++it) {
			if (it != v.begin())
				Encode << "|";
			Encode << it->enc().c_str();
		}
		return Encode.str();
	}
	static Camino dec(std::string s) {
		stringstream ss(s);
		char cs[13];
		Camino cam;
		while (!ss.eof()) {
			ss.get(cs, 11, '|');
			cam.agregar(Coordenada::dec(cs));
			ss.ignore();
		}
		return cam;
	}

	// Copipeisteado de Calculador.cpp para evitar redundancia.
	static Coordenada pixelCentralDeTile(Coordenada coord_tile, Coordenada coord_ceros) {
		if (coord_tile.x < 0 || coord_tile.y < 0)// || tile_x >= this->tiles_x || tile_y >= this->tiles_y)
			throw FueraDeEscenario();
		int pix_x = coord_ceros.x + 	(coord_tile.x - coord_tile.y) * DISTANCIA_ENTRE_X;
		int pix_y = coord_ceros.y + (1 + coord_tile.x + coord_tile.y) * DISTANCIA_ENTRE_Y;
		return Coordenada(pix_x, pix_y);
	}

	~Camino() {
		v.clear();
	}
};


/* Estructura para guardar una instancia Entidad que se encuentra en las
 *  coordenadas x,y */
struct PosEntidad{
	int x,y;
	Entidad* entidad;

	PosEntidad(int x, int y, Entidad* ente){
		this->x = x;
		this->y = y;
		this->entidad = ente;
	};
	PosEntidad(Coordenada c, Entidad* ente) {
		this->x = c.x;
		this->y = c.y;
		this->entidad = ente;
	};

	Coordenada coord() {
		return Coordenada(this->x,this->y);
	}

	bool operator==(const PosEntidad &r) const {
		return (this->x == r.x && this->y == r.y
				  && this->entidad == r.entidad);
	};

	// Encodeado: "x;y[entidad]"
	std::string enc() {
		ostringstream Encode;
		Encode << x<<";"<<y<<"["<<entidad->enc()<<"]";
		return Encode.str();
	};
	static PosEntidad dec(std::string s) {
		std::stringstream ss(s);
		int x,y; char aux[21];
		ss >> x; ss.ignore(); // ','
		ss >> y; ss.ignore(); // '['
		ss.get(aux, 20, ']');
		Entidad e = Entidad::dec(aux);
		return PosEntidad(x,y,&e);
	};
};


#endif /* UTILS_STRUCTS_H_ */
