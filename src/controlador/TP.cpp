#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../red/Server.h"
#include "../controlador/Controller.h"
#include "../vista/VentanaJuego.h"
#include "../vista/VentanaConexion.h"
#include "../vista/VentanaMenu.h"
#include "../red/Connection.h"

int main(int argc, char** argv) {

	Server* server = NULL;
	Client* lan = NULL;
	if (argc > 1) {
		try {
			if (argv[1][0] == 's') {
				server = new Server();
				server->correr();
				delete server;
			} else if (argv[1][0] == 'c')
				lan = new Client();
		} catch ( ConnectionProblem &e ) { return -1; }
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		return -1;
	} else if (server == NULL) {
		TTF_Init();

		Controller *controller = new Controller(lan);
		VentanaJuego *ventana = new VentanaJuego(controller);
		//VentanaConexion *ventana = new VentanaConexion(controller);
		//VentanaMenu *ventana = new VentanaMenu(controller);
		ventana->mostrar();

		delete ventana;
		delete controller;
		SDL_Quit();
		IMG_Quit();
		TTF_Quit();
		delete lan;
	}

	return 0;
}
