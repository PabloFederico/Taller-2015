/*
 * Boton.h
 *
 *  Created on: 25 de oct. de 2015
 *      Author: emanuel
 */

#ifndef VISTA_BOTON_H_
#define VISTA_BOTON_H_
#include <SDL2/SDL.h>
#include "../utils/Enumerados.h"
#include "../vista/Imagen.h"
#include <string>

class Boton {
private:
	SDL_Rect rect;
	std::string info;
	bool presionado;
	Imagen* imagenTexto;
	SDL_Rect frames[2];
	SDL_Rect frameActual;

public:
	Boton(std::string info, SDL_Rect posicion);

	void setImagen(Imagen* imagen);

	bool fue_presionado();

	void presionarBoton();

	Imagen* getImagen();

	SDL_Rect getPosicion();

	SDL_Rect getFrameActual();

	std::string getInfoBoton();

	virtual ~Boton();
};

#endif /* VISTA_BOTON_H_ */
