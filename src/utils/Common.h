/*
 * Common.h
 *
 *  Created on: 11 de nov. de 2015
 *      Author: emanuel
 */

#ifndef SRC_UTILS_COMMON_H_
#define SRC_UTILS_COMMON_H_
#include <sstream>

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
	bool operator!=(const Coordenada & c) const {
		return (this->x != c.x || this->y != c.y);
	}

	bool operator<(const Coordenada & c) const {
		return (this->x < c.x || this->y < c.y);	//Adivino que esto está bien
	}

	Coordenada operator+(const Coordenada & c) const {
		return Coordenada(this->x + c.x,this->y + c.y);
	}

	Coordenada operator-(const Coordenada & c) const {
		return Coordenada(this->x - c.x,this->y - c.y);
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




#endif /* SRC_UTILS_COMMON_H_ */
