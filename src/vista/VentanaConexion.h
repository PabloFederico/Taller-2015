/*
 * VentanaConexion.h
 *
 *  Created on: 22 de oct. de 2015
 *      Author: emanuel
 */

#ifndef VISTA_VENTANACONEXION_H_
#define VISTA_VENTANACONEXION_H_
#include "../vista/Ventana.h"

enum Campo { NINGUNO, IP, NOMBRE, PUERTO};

class VentanaConexion : public Ventana{
private:
	Imagen* imagenFondo;
	Imagen* imagenFondoCampo;

public:
	VentanaConexion(Controller *controlador);

	EstadoFinVentana run();

	virtual ~VentanaConexion();
};

#endif /* VISTA_VENTANACONEXION_H_ */
