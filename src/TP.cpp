#include "Controller.h"

int main(int arg, char** argv) {
	Controller *controller = new Controller();
	controller->iniciarJuego();
	delete controller;

	return 0;
}
