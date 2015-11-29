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
#include "../utils/Exceptions.h"
#include "../utils/Constantes.h"
#include "../modelo/Entidad.h"
#include "../utils/Common.h"

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
struct InfoEntidad {
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

	std::string enc() {
		ostringstream Encode;
		Encode << tipo<<",'"<<path<<"','"<<descripcion<<"',"<<ancho<<","<<alto<<","<<pixel_ref_x<<","<<pixel_ref_y<<","<<fps<<","<<delay;
		return Encode.str();
	}

	static InfoEntidad dec(std::string s) {
		InfoEntidad ie;
		int tipo; char cs[101];
		stringstream ss(s);
		ss >> tipo; ie.tipo = TipoEntidad(tipo); ss.ignore(2);
		ss.get(cs, 100, '\''); ie.path = cs; ss.ignore(3);
		ss.get(cs, 100, '\''); ie.descripcion = cs; ss.ignore(2);
		ss >> ie.ancho; ss.ignore();
		ss >> ie.alto; ss.ignore();
		ss >> ie.pixel_ref_x; ss.ignore();
		ss >> ie.pixel_ref_y; ss.ignore();
		ss >> ie.fps; ss.ignore();
		ss >> ie.delay;
		return ie;
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
	bool empty() { return (size() == 0); }
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
	Coordenada operator[](int k) { return v[k]; }	// Acceso, no modificación
	Coordenada ultimo() {
		return v.back();
	}

	// Encodeado: "c1|c2|c3|...|cn|"
	std::string enc() {
		ostringstream Encode;
		for (std::vector<Coordenada>::iterator it = v.begin(); it < v.end(); ++it) {
			Encode << it->enc().c_str() << "|";
		}
		return Encode.str();
	}
	static Camino dec(std::string s) {
		stringstream ss(s);
		char cs[13];
		Camino cam;
		while (!ss.eof() && ss.peek() != '\0') {
			ss.get(cs, 11, '|');
			if (cs[0] == 0)
				break;
			cam.agregar(Coordenada::dec(cs));
			ss.ignore(); // '|'
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

	void borrar() {
		v.clear();
	}
	~Camino() {
		borrar();
	}
};


/* Estructura para guardar una instancia Entidad que se encuentra en las
 *  coordenadas x,y */
//struct PosEntidad{
//	int x,y;
//	Entidad* entidad;
//
//	PosEntidad(int x, int y, Entidad* ente){
//		this->x = x;
//		this->y = y;
//		this->entidad = ente;
//	};
//	PosEntidad(Coordenada c, Entidad* ente) {
//		this->x = c.x;
//		this->y = c.y;
//		this->entidad = ente;
//	};
//
//	Coordenada coord() {
//		return Coordenada(this->x,this->y);
//	}
//
//	bool operator==(const PosEntidad &r) const {
//		return (this->x == r.x && this->y == r.y
//				  && this->entidad == r.entidad);
//	};
//
//	// Encodeado: "x;y[entidad]"
//	std::string enc() {
//		ostringstream Encode;
//		Encode << x<<";"<<y<<"["<<entidad->enc()<<"]";
//		return Encode.str();
//	};
//	static PosEntidad dec(std::string s) {
//		std::stringstream ss(s);
//		int x,y; char aux[21];
//		ss >> x; ss.ignore(); // ';'
//		ss >> y; ss.ignore(); // '['
//		ss.get(aux, 20, ']');
//		Entidad *e = Entidad::dec(aux);
//		return PosEntidad(x,y,e);
//	};
//};


/* Estructura para guardar información para Escenario */
struct InfoEscenario {
	string nombre;
	int size_x;
	int size_y;
	vector<PosTipoEntidad> posTipoEntidades;

	InfoEscenario(){
		nombre = "";
		size_x = 1;
		size_y = 1;
	};

	void agregarEntidad(pair<int,int> pos, TipoEntidad tipo){
		PosTipoEntidad posTipoEnte(pos.first,pos.second,tipo);
		this->posTipoEntidades.push_back(posTipoEnte);
	};

	void agregarEntidad(Coordenada pos, TipoEntidad tipo){
		PosTipoEntidad posTipoEnte(pos.x,pos.y,tipo);
		this->posTipoEntidades.push_back(posTipoEnte);
	};

	vector<PosTipoEntidad> getPosicionesEntidades(){
		return this->posTipoEntidades;
	}

	bool posicionYaOcupada(Coordenada c) {	// Hardcodeado para edificios de 4x4.
		for (vector<PosTipoEntidad>::iterator it = posTipoEntidades.begin(); it < posTipoEntidades.end(); ++it)
			if ((it->x == c.x && it->y == c.y) || (EsEdificio(it->tipo) && (c.x - it->x < 4) && (c.y - it->y < 4)))
				return true;
		return false;
	}

	bool operator!() {
		return !((size_x > 0) && (size_y > 0));// && (posX_protagonista >= 0) && (posY_protagonista >= 0));
	}

	// Encodeado: "nombre,size_x;size_y|[...,PTE,...,]"
	std::string enc() {
		ostringstream Encode;
		Encode << nombre << "," << size_x << ";" << size_y << "|[";
		for (vector<PosTipoEntidad>::iterator it = posTipoEntidades.begin(); it < posTipoEntidades.end(); ++it) {
			Encode << it->enc()<<",";
		}
		Encode << "]";
		return Encode.str();
	}

	static InfoEscenario dec(std::string s) {
		char cs[21];
		InfoEscenario ie;
		stringstream ss(s);
		ss.get(cs, 20, ',');
		ie.nombre = cs;
		ss.ignore();
		ss >> ie.size_x;
		ss.ignore();
		ss >> ie.size_y;
		ss.ignore(2);
		while (ss.peek() != ']') {
			ss.get(cs, 14, ',');
			ie.posTipoEntidades.push_back(PosTipoEntidad::dec(cs));
			ss.ignore();
		}
		return ie;
	}
};

struct ConfiguracionJuego{
	string nombreJugador;
	string direccion_ip;
	int ancho_pantalla;
	int alto_pantalla;
	int vel_personaje;
	int margen_scroll;
	int rango_vision;
	vector<InfoEntidad> entidades;
	vector<InfoEscenario> escenarios;

	ConfiguracionJuego(){
		nombreJugador = "Village Slayer";
		direccion_ip = "127.0.0.1";
		ancho_pantalla = 640;
		alto_pantalla = 480;
		vel_personaje = 50;
		margen_scroll = 20;
		rango_vision = 10;
	}

	void agregarInfoEntidad(InfoEntidad infoEntidad){
		entidades.push_back(infoEntidad);
	}

	void agregarInfoEscenarios(InfoEscenario infoEscenario){
		escenarios.push_back(infoEscenario);
	}

	// Encodeado: "nombreJugador,direccion_ip,ancho_pantalla,alto_pantalla,vel_personaje,margen_scroll,rango_vision,[...;entidades.enc();],[...;escenarios.enc();]"
	std::string enc() {
		ostringstream Encode;
		Encode << nombreJugador<<","<<direccion_ip<<","<<ancho_pantalla<<","<<alto_pantalla<<","<<vel_personaje<<","<<margen_scroll<<","<<rango_vision<<",[";
		for (vector<InfoEntidad>::iterator it = entidades.begin(); it < entidades.end(); ++it) {
			Encode << it->enc()<<";";
		}
		Encode << "],[";
		for (vector<InfoEscenario>::iterator it2 = escenarios.begin(); it2 < escenarios.end(); ++it2) {
			Encode << it2->enc()<<";";
		}
		Encode << "]";
		return Encode.str();
	}

	static ConfiguracionJuego dec(std::string s) {
		ConfiguracionJuego cj;
		char cs[502];
		stringstream ss(s);
		ss.get(cs, 21, ','); cj.nombreJugador = cs; ss.ignore();
		ss.get(cs, 21, ','); cj.direccion_ip = cs; ss.ignore();
		ss >> cj.ancho_pantalla; ss.ignore();
		ss >> cj.alto_pantalla; ss.ignore();
		ss >> cj.vel_personaje; ss.ignore();
		ss >> cj.margen_scroll; ss.ignore();
		ss >> cj.rango_vision; ss.ignore(2);
		while (ss.peek() != ']') {
			ss.get(cs, 100, ';');
			cj.entidades.push_back(InfoEntidad::dec(string(cs)));
			ss.ignore(); // ';'
		}
		while (ss.peek() != ']') {
			ss.get(cs, 500, ';');
			cj.escenarios.push_back(InfoEscenario::dec(string(cs)));
			ss.ignore(); // ';'
		}
		return cj;
	}
};


#endif /* UTILS_STRUCTS_H_ */
