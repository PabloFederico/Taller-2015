/*
 * VentanaEspera.h
 *
 *  Created on: 18 de nov. de 2015
 *      Author: emanuel
 */

#ifndef SRC_VISTA_VENTANAESPERA_H_
#define SRC_VISTA_VENTANAESPERA_H_
#include "Ventana.h"


class VentanaEspera: public Ventana {
private:
	Imagen* imagenFondo;
	Imagen* imagenEsperando;

public:
	VentanaEspera(std::pair<int,int> dimensionesVentana);
	//VentanaEspera(Controller* controlador);
	EstadoFinVentana run();
	virtual ~VentanaEspera();
};

#endif /* SRC_VISTA_VENTANAESPERA_H_ */
