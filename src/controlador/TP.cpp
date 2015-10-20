#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../controlador/Controller.h"
#include "../vista/VentanaJuego.h"

#include "../red/Connection.h"

int main(int argc, char** argv) {

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
		return -1;
	}else{
		Connection* lan = NULL;
		if (argc > 1) {
			if (argv[1][0] == 's')
				lan = new Server();
			else if (argv[1][0] == 'c')
				lan = new Client();
		}

		Controller *controller = new Controller(lan);
		VentanaJuego *ventana = new VentanaJuego(controller);
		ventana->mostrar();
		delete ventana;
		delete controller;
		SDL_Quit();
		IMG_Quit();
	}

	return 0;
}
