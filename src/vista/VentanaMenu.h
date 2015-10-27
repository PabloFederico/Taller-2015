/*
 * VentanaMenu.h
 *
 *  Created on: 25 de oct. de 2015
 *      Author: emanuel
 */

#ifndef VISTA_VENTANAMENU_H_
#define VISTA_VENTANAMENU_H_
#include "../vista/Ventana.h"
#include "../vista/Boton.h"

const int CANT_BOTON_MENU = 5;

class VentanaMenu: public Ventana {
private:
	Imagen* imagenFondo;
	Imagen* fondoBoton;
	vector<Imagen*> imagenesBasura;
	vector<Boton*> botones;

	void generarBotonesMenu();

	TipoBoton procesarClic(Coordenada c);
	void procesarTeclado(SDL_Event e, string &input, bool &renderText);

public:
	VentanaMenu(Controller* controlador);
	void mostrar();
	virtual ~VentanaMenu();
};

#endif /* VISTA_VENTANAMENU_H_ */
