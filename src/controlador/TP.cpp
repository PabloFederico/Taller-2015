#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../controlador/Controller.h"
#include "../vista/VentanaJuego.h"

#include "../red/Client.h"
#include "../red/Server.h"

int main(int arg, char** argv) {

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
		return -1;
	}else{
		//Server::ejecutar();
		//Client::ejecutar();
		Controller *controller = new Controller();
		VentanaJuego *ventana = new VentanaJuego(controller);
		ventana->mostrar();
		delete ventana;
		delete controller;
		SDL_Quit();
		IMG_Quit();
	}

	return 0;
}
