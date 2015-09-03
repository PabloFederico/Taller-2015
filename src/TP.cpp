//============================================================================
// Name        : TP.cpp
// Author      : P.Federico
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <SDL2/SDL.h>
#include "VentanaJuego.h"
using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int TILES_X = 20;
const int TILES_Y = 20;


	//Luego este main tiene que estar adentro de una clase juego
	 // ParsearYaml();
	// CargarPantalla();
	// IniciarBucleJuego();


int main() {
	VentanaJuego *vent_juego = new VentanaJuego(SCREEN_WIDTH,SCREEN_HEIGHT,TILES_X,TILES_Y);
	vent_juego->dibujarSuperficie();
	delete vent_juego;
	return 0;
}
