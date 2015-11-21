/*
 * VentanaJuego.cpp
 *
 *  Created on: 2 de sept. de 2015
 *      Author: emanuel
 */

#include "../vista/VentanaJuego.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
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
		this->fuenteTexto = TTF_OpenFont("images/censcbk.ttf",TAM_LETRA_JUEGO);
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

		/* Las imágenes, los sprites y los SDL_Rect (para cada entidad)
		 * se cargan en el contenedor */

		this->cargarImagenesYSprites(juego);
		juego->cargaInicialDeRecursos();

		controlador->posicionarCamaraEnProtagonista();

		string nombreJugador = juego->getNombreJugador();
		SDL_SetWindowTitle(window,nombreJugador.c_str());

		//Cargamos la música de fondo.
		Mix_Music *musica;
		musica = Mix_LoadMUS("sound/fondo.mp3");
		if (musica == NULL) printf("Error al cargar la cancion. Error: %s\n", Mix_GetError());
		else this->musica_fondo = musica;

	}
}


/********************************************************************************/
void VentanaJuego::cargarImagenesYSprites(Juego* juego){
	ContenedorDeRecursos * contenedor = new ContenedorDeRecursos(renderer);
	contenedor->cargarImagenesEntidades(juego->getInfoTiposEntidades());

	vector<Entidad*>* posEntidades = juego->getEscenario()->getVectorEntidades();

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

	dibujador->dibujarEscenario(escenario, fuenteTexto, pair<int,int>(this->SCREEN_WIDTH,this->SCREEN_HEIGHT));
	dibujador->dibujarBarraEstado(escenario, barraEstado, fuenteTexto);

	SDL_RenderPresent(this->renderer);
}

/********************************************************************************/
EstadoFinVentana VentanaJuego::run(){
		bool run = true;
		EstadoFinVentana estado = OK;
		//Arranca la musica.
		if (Mix_PlayingMusic() == 0){
			Mix_PlayMusic(musica_fondo, -1);
		}

		SDL_Event event;
		SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
		if (cursor == NULL) printf("Falló la creación del cursor %s",SDL_GetError());
		SDL_SetCursor(cursor);

		while ( run ) {

				controlador->capturarEvento(event);

				if (event.type == SDL_QUIT) {
					estado = EXIT;
					run = false;
				}

				controlador->procesarEvento();

				controlador->actualizarEstadoDelJuego();

	            /* Actualiza el renderer */
	            this->dibujar();

	            SDL_Delay(10); /* Delay para Emanuel */

	    		if (event.type == SDL_KEYDOWN){
	            	if (event.key.keysym.sym == KEY_RESET){
	            		this->reiniciar();
	            	}
	            }

		} /* Fin del while*/

		return estado;
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
