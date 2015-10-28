/*
 * VentanaConexion.cpp
 *
 *  Created on: 22 de oct. de 2015
 *      Author: emanuel
 */

#include "VentanaConexion.h"
#include "../utils/Loader.h"
#include "../utils/Log.h"
#include <iostream>

VentanaConexion::VentanaConexion(Controller *controlador): Ventana(controlador) {
	if ( !init() ) std::cout << "error" << std::endl;
	 this->fuenteTexto = TTF_OpenFont("censcbk.ttf",TAM_LETRA_CONEXION);
	imagenFondo = Loader::cargarImagen(renderer,"images/fondo_ventana.png");
}

void VentanaConexion::mostrar(){
	bool quit = false;
	SDL_Event e;

	SDL_Color backGroundColor = {255,255,255};
	std::string labelIP = "Ingrese IP del servidor : ";
	Imagen *textoLabelIP = Loader::cargarTextoConFondo(renderer, fuenteTexto, labelIP, backGroundColor);
	SDL_Rect rect_label;
	rect_label.x = SCREEN_WIDTH / 2 - textoLabelIP->getPixelsX();
	rect_label.y = SCREEN_HEIGHT / 2;
	rect_label.w = textoLabelIP->getPixelsX();
	rect_label.h = textoLabelIP->getPixelsY();

	std::string inputText = "";
	Imagen *textoInput;
	SDL_Rect rect_input;
	rect_input.x = SCREEN_WIDTH / 2;
	rect_input.y = SCREEN_HEIGHT / 2;


	//Enable text input
	SDL_StartTextInput();
	//While application is running
	while( !quit ){
		//The rerender text flag
		bool renderText = false;

		//Handle events on queue
		while( SDL_PollEvent( &e ) != 0 ){
			//User requests quit
			if( e.type == SDL_QUIT ) quit = true;

			//Special key input
			else if( e.type == SDL_KEYDOWN ){

				/* Debería tomar la tecla ENTER
				 * pero no la toma, por eso puse el TAB*/
				if (e.key.keysym.sym == SDLK_TAB){
					int conexionValida = controlador->verificarConexion(inputText);

					if (conexionValida < 0){
						Log::imprimirALog(ERR_FAT, "No se pudo conectar con el Servidor");
						inputText = "";
						renderText = true;
						SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
						                         "Connection Failed",
						                         "La conexion con el servidor fallo. Vuelva a intentarlo",
						                         NULL);

					}else{
						Log::imprimirALog(INFO, "Conectado al Servidor");
						quit = true;
					}
				}


				//Handle backspace
				if( e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0 ){
					//lop off character
					inputText.erase(inputText.length()-1);
					renderText = true;
				}
				//Handle copy
				else if( e.key.keysym.sym == SDLK_c && (SDL_GetModState() & KMOD_CTRL) ){
					SDL_SetClipboardText( inputText.c_str() );
				}
				//Handle paste
				else if( e.key.keysym.sym == SDLK_v && (SDL_GetModState() & KMOD_CTRL) ){
					inputText = SDL_GetClipboardText();
					renderText = true;
				}
			}
			//Special text input event
			else if( e.type == SDL_TEXTINPUT ){
				//Not copy or pasting
				if( !( ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' ) && ( e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V' ) && (SDL_GetModState() & KMOD_CTRL) ) )
				{
					if (inputText.length() < 14){
						//Append character
						inputText += e.text.text;
						renderText = true;
					}

				}
			}
		} /* Fin while SDL_PollEvent*/

		//Rerender text if needed
		if( renderText ){
			//Text is not empty
			if( inputText != "" ){
				//Render new text
				textoInput = Loader::cargarTextoConFondo(renderer,fuenteTexto,inputText, backGroundColor);
			}
			//Text is empty
			else{
				//Render space texture
				textoInput = Loader::cargarTextoConFondo(renderer, fuenteTexto, " ",backGroundColor);
			}
			imagenesBasura.push_back(textoInput);
		}

		//Clear screen
		SDL_RenderClear(renderer);
		rect_input.w = textoInput->getPixelsX();
		rect_input.h = textoInput->getPixelsY();

		//Render text textures
		SDL_RenderCopy(renderer, imagenFondo->getTexture(), NULL, NULL);
		SDL_RenderCopy(renderer, textoLabelIP->getTexture(), NULL, &rect_label);
		SDL_RenderCopy(renderer, textoInput->getTexture(), NULL, &rect_input);

		//Update screen
		SDL_RenderPresent(renderer);

	} /* Fin while quit */

	//Disable text input
	SDL_StopTextInput();
	delete textoLabelIP;
    delete textoInput;
}

VentanaConexion::~VentanaConexion() {
	close();
	delete this->imagenFondo;
	for (unsigned i = 0; i < imagenesBasura.size()-1; i++){
		delete imagenesBasura[i];
	}
	imagenesBasura.clear();
}

