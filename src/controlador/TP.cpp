#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../controlador/Controller.h"
#include "../vista/VentanaJuego.h"
#include "../vista/VentanaConexion.h"
#include "../vista/VentanaMenu.h"
#include "../red/Connection.h"

int main(int argc, char** argv) {

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
		return -1;
	}else{
		TTF_Init();
		Connection* lan = NULL;//
		Server* server = NULL;
		if (argc > 1) {
			try {
				if (argv[1][0] == 's') {
					server = new Server();
					while (true)
						server->correr();
				} else if (argv[1][0] == 'c')
					lan = new Client();
			} catch ( ConnectionProblem &e ) { lan = NULL; }
		}

		Controller *controller = new Controller(lan);
		VentanaJuego *ventana = new VentanaJuego(controller);
		//VentanaConexion *ventana = new VentanaConexion(controller);
		//VentanaMenu *ventana = new VentanaMenu(controller);
		ventana->mostrar(server);
		//ventana->mostrar();
		delete ventana;
		delete controller;
		SDL_Quit();
		IMG_Quit();
		TTF_Quit();
	}

	return 0;
}
