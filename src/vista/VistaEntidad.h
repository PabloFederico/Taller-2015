/*
 * VistaEntidad.h
 *
 *  Created on: Sep 18, 2015
 *      Author: martin
 */

#ifndef VISTA_VISTAENTIDAD_H_
#define VISTA_VISTAENTIDAD_H_
#include <string>
using namespace std;



class VistaEntidad {
	string nombreTipo;
	string pathImagen;

public:
	// Ya sé que a Carlos Fontela no le gusta esto, pero esto no es Java, así que no jo*a.
	int alto_base, ancho_base;
	int pixel_ref_x, pixel_ref_y;
	int fps, delay;


	VistaEntidad(string nombre, string path="mann.png",
			int alto_base=0, int ancho_base=0, int pixel_ref_x=0, int pixel_ref_y=0, int fps=0, int delay=0);

	/* Devuelve el path de la imagen de la entidad*/
	std::string getPath();

	/* Setea el path de la imagen de la entidad */
	void setPath(std::string path);

	std::string getTipo();

	void setAltoBase(int n);
	void setAnchoBase(int n);
	void setPixelRefX(int n);
	void setPixelRefY(int n);
	void setFps(int n);
	void setDelay(int n);

	virtual ~VistaEntidad();
};

#endif /* VISTA_VISTAENTIDAD_H_ */
