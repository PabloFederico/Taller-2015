/*
 * VentanaJuego.h
 *
 *  Created on: 2 de sept. de 2015
 *      Author: emanuel
 */

#ifndef VISTA_VENTANAJUEGO_H_
#define VISTA_VENTANAJUEGO_H_

#include "../modelo/Juego.h"
#include "../utils/Calculador.h"
#include "../utils/ContenedorDeRecursos.h"
#include "../utils/Map.h"
#include "../utils/Structs.h"
#include "Dibujador.h"
#include "../vista/Sprite.h"
#include "../vista/Ventana.h"
#include "CapaFog.h"
using namespace std;


class VentanaJuego : public Ventana {
private:
	Dibujador *dibujador;

	void cargarJuego(Juego *juego);
	void cargarImagenesYSprites(Juego *juego);

	/* Dibuja el escenario*/
	void dibujar();

	void liberarRecursos();
	void reiniciar();

public:
	VentanaJuego(Controller *controlador);

	/* Muestra la ventana principal */
	void run();

	virtual ~VentanaJuego();

};

#endif /* VISTA_VENTANAJUEGO_H_ */
