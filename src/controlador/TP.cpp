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
	srand((time(0)) * (clock()+321) + 123);

	Server* server = NULL;
	Client* lan = NULL;
	if (argc > 1) {
		try {
			if (argv[1][0] == 's') {
				server = new Server(8888);
				server->correr();
			} //else if (argv[1][0] == 'c')
			//lan = new Client("127.0.0.1",8888);
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

//		try {
//			if (lan != NULL) {
//				//infoJuego = Proxy::clienteEsperarConfigGame(lan);
//				//Proxy::clienteEsperarComienzo(lan);
//				this->juego = new Juego(lan, NULL);//, &infoJuego);
//			}
//		} catch ( Disconnected &e ) {
//			std::cout << "Desconectado."<<std::endl;
//			lan = NULL;
//		}
		Controller *controller = new Controller(lan);

		VentanaConexion *ventanaConexion = new VentanaConexion(controller);
		EstadoFinVentana estado = ventanaConexion->run();
		delete ventanaConexion;
		//EstadoFinVentana estado = OK;//
		std::cout<<"Conexion exitosa\n";

		if (estado == OK){
			VentanaEspera *ventanaEspera = new VentanaEspera(controller);
			estado = ventanaEspera->run();
			delete ventanaEspera;

			if (estado == OK){
				controller->crearJuego();
				VentanaJuego *ventanaJuego = new VentanaJuego(controller);
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
