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
	TipoBoton tipo;
	SDL_Rect rect;
	std::string info;
	Imagen* imagenTexto;

public:
	Boton(TipoBoton tipo, SDL_Rect posicion);

	TipoBoton getTipo();

	void setImagen(Imagen* imagen);

	Imagen* getImagen();

	SDL_Rect getPosicion();

	std::string getInfoBoton();

	virtual ~Boton();
};

#endif /* VISTA_BOTON_H_ */
