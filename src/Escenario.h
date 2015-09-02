/*
 * Escenario.h
 *
 *  Created on: 2 de sept. de 2015
 *      Author: pgfederi
 */

#ifndef ESCENARIO_H_
#define ESCENARIO_H_

#include <string>
using namespace std;


class Escenario {

private:
	string nombre;
	int size_x;
	int size_y;
	//Diccionario o Matriz para tener las celdas


public:
	Escenario();
	virtual ~Escenario();
};

#endif /* ESCENARIO_H_ */
