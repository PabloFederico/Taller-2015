#include "Controller.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int main(int arg, char** argv) {

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS) < 0){
		return -1;
	}else{
		Controller *controller = new Controller();
		controller->iniciarJuego();
		delete controller;
		SDL_Quit();
		IMG_Quit();
	}

	return 0;
}
