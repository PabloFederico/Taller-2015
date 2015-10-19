#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../controlador/Controller.h"
#include "../vista/VentanaJuego.h"

#include "../red/Client.h"
#include "../red/Server.h"

int main(int argc, char** argv) {

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
		return -1;
	}else{
		if (argc > 1) {
			if (argv[1][0] == 's')
				Server::ejecutar();
			else if (argv[1][0] == 'c')
				Client::ejecutar();
		}

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
