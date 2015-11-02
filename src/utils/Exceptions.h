/*
 * Exceptions.h
 *
 *  Created on: Sep 19, 2015
 *      Author: martin
 */

#ifndef UTILS_EXCEPTIONS_H_
#define UTILS_EXCEPTIONS_H_

#include <exception>
#include <stdexcept>

class bad_nombreTipoEntidad: public std::invalid_argument
{
public:
	bad_nombreTipoEntidad(const std::string& message);
	const char* what() const throw();
};

class DestinoEncontrado: public std::exception
{
public:
	DestinoEncontrado();
};

class FueraDeEscenario: public std::exception
{
public:
	FueraDeEscenario();
};

class TileEstaOcupado: public std::exception
{
public:
	TileEstaOcupado();
};

class CaminoVacio: public std::exception
{
public:
	CaminoVacio();
};

class PasoCompletado: public std::exception
{
public:
	int id;
	PasoCompletado(int ID);
};

class NoSeRecibio: public std::exception
{
public:
	NoSeRecibio();
};

class NoTieneRecurso: public std::exception
{
public:
	NoTieneRecurso();
};

class ConnectionProblem: public std::exception
{
public:
	ConnectionProblem();
};

class Disconnected: public std::exception
{
public:
	Disconnected();
};

class NoExiste: public std::exception
{
public:
	NoExiste();
};


#endif /* UTILS_EXCEPTIONS_H_ */
