#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "../red/Server.h"
#include "../red/Client.h"
#include "../red/Connection.h"
#include "../controlador/Controller.h"
#include "../vista/VentanaJuego.h"
#include "../vista/VentanaConexion.h"
#include "../vista/VentanaEspera.h"

int main(int argc, char** argv) {
	srand(1707801);

	Server* server = NULL;
	Client* lan = NULL;
	if (argc > 1) {
		try {
			if (argv[1][0] == 's') {
				server = new Server();
				server->correr();
			}
		} catch ( ConnectionProblem &e ) { return -1; }
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		return -1;
	} else if (server == NULL) {
		TTF_Init();

		//Inicializo el Mixer:
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
			printf ("Error al inicializar SDL_mixer. Error: %s\n", Mix_GetError());
			SDL_Quit();
			IMG_Quit();
			TTF_Quit();
			return -1;
		}

		EstadoFinVentana estado = OK;
		VentanaEspera *ventanaEspera = NULL;
		ObjetivoEscenario modoDeJuego = MODO_DEFAULT;

		if (argc > 1) {
			try {
				if (argv[1][0] == 'c') {
					//ventanaEspera = new VentanaEspera(std::pair<int,int>(640,800)); // hardcode sabroso
					//estado = ventanaEspera->run();

					lan = new Client();	// Se conecta.
					modoDeJuego = Proxy::clienteEsperarComienzoYModoDeJuego(lan); // Espera comienzo de juego.
				}
			} catch ( ConnectionProblem &e ) { return -1; }
		}

		Controller *controller = new Controller(lan, modoDeJuego);	// Carga el juego (modelo)

		delete ventanaEspera;

		if (estado == OK){

			if (estado == OK){
				VentanaJuego *ventanaJuego = new VentanaJuego(controller);	// Carga y mantiene la vista.
				estado = ventanaJuego->run();
				delete ventanaJuego;
			}

			// Meter una ventana de resultado de la partida (GANADO - PERDIDO)
		}

		delete controller;
		Mix_Quit();
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
	}
	delete server;
	delete lan;

	return 0;
}
