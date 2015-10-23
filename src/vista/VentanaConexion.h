/*
 * VentanaConexion.h
 *
 *  Created on: 22 de oct. de 2015
 *      Author: emanuel
 */

#ifndef VISTA_VENTANACONEXION_H_
#define VISTA_VENTANACONEXION_H_
#include "../vista/Ventana.h"

class VentanaConexion : public Ventana{
private:
	Imagen* imagenFondo;
	vector<Imagen*> imagenesBasura;

public:
	VentanaConexion(Controller *controlador);

	void mostrar();

	virtual ~VentanaConexion();
};

#endif /* VISTA_VENTANACONEXION_H_ */
