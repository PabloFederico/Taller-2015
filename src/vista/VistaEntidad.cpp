/*
 * VistaEntidad.cpp
 *
 *  Created on: Sep 18, 2015
 *      Author: martin
 */

#include "../vista/VistaEntidad.h"
#include "../modelo/Exceptions.h"


VistaEntidad::VistaEntidad(string nombre, string path,
				int alto_base, int ancho_base, int pixel_ref_x, int pixel_ref_y, int fps, int delay)
					: nombreTipo(nombre), pathImagen(path), alto_base(alto_base), ancho_base(ancho_base),
					  pixel_ref_x(pixel_ref_x), pixel_ref_y(pixel_ref_y), fps(fps), delay(delay) {
	if (nombre == "") throw bad_nombreTipoEntidad(nombre);
}


void VistaEntidad::setPath(std::string path) {
	this->pathImagen = path;
}

std::string VistaEntidad::getPath() {
	return this->pathImagen;
}

std::string VistaEntidad::getTipo() {
	return this->nombreTipo;
}


void VistaEntidad::setAltoBase(int n) {
	this->alto_base = n;
}

void VistaEntidad::setAnchoBase(int n) {
	this->ancho_base = n;
}

void VistaEntidad::setPixelRefX(int n) {
	this->pixel_ref_x = n;
}

void VistaEntidad::setPixelRefY(int n) {
	this->pixel_ref_y = n;
}

void VistaEntidad::setFps(int n) {
	this->fps = n;
}

void VistaEntidad::setDelay(int n) {
	this->delay = n;
}


VistaEntidad::~VistaEntidad() {
	// TODO Auto-generated destructor stub
}

