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
	bool click_izq_presionado;
	bool click_der_presionado;
	Coordenada c;
	Coordenada c_anterior;
	bool coord_ant_inicializada;

public:
	Mouse();

	EstadoMouse getEstado();

	void setEstado(EstadoMouse estado);

	bool estaInicializadaCoordAnterior();

	void inicializarCoordenadaAnterior(Coordenada c);

	void setXY(Coordenada c);

	void setXY_anterior(Coordenada c);

	bool click_der_y_arrastrado();

	Coordenada getXY();

	Coordenada getXY_anterior();

	void pulsar_click_izq();

	void soltar_click_izq();

	void pulsar_click_der();

	void soltar_click_der();

	bool click();

	bool clickIzqPresionado();

	bool clickDerPresionado();

	bool botonFuePresionado(Boton* boton);

	virtual ~Mouse();
};

#endif /* SRC_CONTROLADOR_MOUSE_H_ */
