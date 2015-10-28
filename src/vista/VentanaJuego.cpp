/*
 * VentanaJuego.cpp
 *
 *  Created on: 2 de sept. de 2015
 *      Author: emanuel
 */

#include "../vista/VentanaJuego.h"
#include <SDL2/SDL_image.h>
#include "../utils/Constantes.h"
#include "../vista/Camara.h"

VentanaJuego::VentanaJuego(Controller *controlador):Ventana(controlador){
	this->dibujador = NULL;
	this->cargarJuego(controlador->getJuego());
}

/********************************************************************************/
void VentanaJuego::cargarJuego(Juego *juego){
	int TILES_X = juego->getEscenario()->getDimension().first;
	int TILES_Y = juego->getEscenario()->getDimension().second;

	int LIMITE_DESPLAZAMIENTO_EN_X = DISTANCIA_ENTRE_X * TILES_X;
	int LIMITE_DESPLAZAMIENTO_EN_Y = DISTANCIA_ENTRE_Y * TILES_Y;

	if (init()){
		this->fuenteTexto = TTF_OpenFont("censcbk.ttf",TAM_LETRA_JUEGO);
		/* El (0,0) relativo del mapa respecto a la ventana principal */
		//PosEntidad protag = juego->getPosEntDeProtagonista();
		int centro_x = SCREEN_WIDTH / 2;  //protag.x;
		int centro_y = SCREEN_HEIGHT / 2; //protag.y;
		int *cero_x = new int(centro_x);
		int *cero_y = new int(centro_y - LIMITE_DESPLAZAMIENTO_EN_Y);

		juego->setCeros(cero_x,cero_y);

		Camara* camara = new Camara(cero_x,cero_y);
		camara->setDimension(SCREEN_WIDTH,SCREEN_HEIGHT - controlador->getJuego()->getBarraEstado()->getDimension().second);
		camara->setMargenScrolling(juego->getMargenScroll());
		camara->setLimites(LIMITE_DESPLAZAMIENTO_EN_X,LIMITE_DESPLAZAMIENTO_EN_Y);

		this->controlador->agregarCamara(camara);

		this->dibujador = new Dibujador(renderer);
		this->dibujador->setOrigen(cero_x,cero_y);

		/* Las imagenes , los sprites y los SDL_Rect (para cada entidad)
		 * se cargar en el contenedor */

		this->cargarImagenesYSprites(juego);

		controlador->posicionarCamaraEnProtagonista();

		string nombreJugador = juego->getProtagonista()->getInfo();
		SDL_SetWindowTitle(window,nombreJugador.c_str());
	}
}


/********************************************************************************/
void VentanaJuego::cargarImagenesYSprites(Juego* juego){
	ContenedorDeRecursos * contenedor = new ContenedorDeRecursos(renderer);
	contenedor->cargarImagenesEntidades(juego->getInfoTiposEntidades());

	vector<PosEntidad>* posEntidades = juego->getEscenario()->getVectorEntidades();

	Coordenada coord_ceros(*juego->getCeros().first, *juego->getCeros().second);
	contenedor->generarYGuardarSpritesEntidades(posEntidades,coord_ceros,juego->getEscenario());

	this->controlador->getJuego()->agregarContenedorDeRecursos(contenedor);
	this->dibujador->setContenedorDeRecursos(contenedor);
}

/********************************************************************************/
void VentanaJuego::dibujar(){
	SDL_RenderClear(this->renderer);

	Escenario* escenario = controlador->getJuego()->getEscenario();
	BarraEstado* barraEstado = controlador->getJuego()->getBarraEstado();

	dibujador->dibujarEscenario(escenario, fuenteTexto);
	dibujador->dibujarBarraEstado(escenario, barraEstado, fuenteTexto);
	//dibujador->dibujarContorno(escenario, fuenteTexto);

	SDL_RenderPresent(this->renderer);
}

/********************************************************************************/
void VentanaJuego::mostrar(){
		bool run = true;

		SDL_Event event;
		uint32_t tiempo;
		SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
		if (cursor == NULL) printf("Fallo la creacion del cursor %s",SDL_GetError());
		SDL_SetCursor(cursor);

		while ( run ){
				tiempo = SDL_GetTicks();
				SDL_PollEvent(&event);

				if (event.type == SDL_QUIT) run = false;
				/*if ((tiempo % 10000) == 0){
					controlador->getJuego()->getEscenario()->agregarRecurso(coord_random);
				}*/
				controlador->procesarEvento(event);

	            /* Actualiza el renderer */
	            this->dibujar();

	            SDL_Delay(15);

	            if (event.type == SDL_KEYDOWN){
	            	if (event.key.keysym.sym == KEY_RESET){
	            		this->reiniciar();
	            	}
	            }

		} /* Fin del while*/
}

/********************************************************************************/
void VentanaJuego::reiniciar(){
	this->controlador->reiniciarJuego();
	this->liberarRecursos();
	this->cargarJuego(controlador->getJuego());
}

/********************************************************************************/
void VentanaJuego::liberarRecursos(){
	delete this->dibujador;
	this->close();
}


/********************************************************************************/
VentanaJuego::~VentanaJuego() {
	this->liberarRecursos();
}
