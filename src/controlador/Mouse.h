/*
 * Mouse.h
 *
 *  Created on: 14 de nov. de 2015
 *      Author: emanuel
 */

#ifndef SRC_CONTROLADOR_MOUSE_H_
#define SRC_CONTROLADOR_MOUSE_H_
#include "../utils/Structs.h"
#include "../vista/Boton.h"

class Mouse {
private:
	EstadoMouse estado;
	Coordenada c;
	Coordenada c_anterior;
	bool moviendoImagen;

public:
	Mouse();

	EstadoMouse getEstado();

	void setEstado(EstadoMouse estado);

	void setXY(Coordenada c);

	void setXY_anterior(Coordenada c);

	Coordenada getXY();

	Coordenada getXY_anterior();

	bool click();

	void setearMoviendoImagen(bool valor);

	bool estaMoviendoImagen();

	bool botonFuePresionado(Boton* boton);

	virtual ~Mouse();
};

#endif /* SRC_CONTROLADOR_MOUSE_H_ */
