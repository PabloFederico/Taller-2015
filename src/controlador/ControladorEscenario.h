/*
 * ControladorEscenario.h
 *
 *  Created on: 9 de nov. de 2015
 *      Author: emanuel
 */

#ifndef CONTROLADOR_CONTROLADORESCENARIO_H_
#define CONTROLADOR_CONTROLADORESCENARIO_H_
//#include "../red/Connection.h"
#include "../modelo/Juego.h"

class ControladorEscenario {
private:
	Juego* juego;
public:
	ControladorEscenario(Juego* juego);

	void actualizarEstadoEscenario();//(Connection* lan);

	virtual ~ControladorEscenario();
};

#endif /* CONTROLADOR_CONTROLADORESCENARIO_H_ */
