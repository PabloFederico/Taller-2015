/*
 * VentanaMenu.cpp
 *
 *  Created on: 25 de oct. de 2015
 *      Author: emanuel
 */

#include "VentanaMenu.h"
#include "../utils/Loader.h"
#include <iostream>

VentanaMenu::VentanaMenu(Controller* controlador):Ventana(controlador) {
	if ( !init() ) std::cout << "error" << std::endl;
	 this->fuenteTexto = TTF_OpenFont("TERMINAT.TTF",20);
	imagenFondo = Loader::cargarImagen(renderer,"images/fondo_ventana.png");
	fondoBoton = Loader::cargarImagen(renderer,"images/fondo_boton_1.png");
	generarBotonesMenu();
}

void VentanaMenu::generarBotonesMenu(){
	int width_window;
	int height_window;
	SDL_GetRendererOutputSize(renderer,&width_window,&height_window);

	int div_alto = height_window / CANT_BOTON_MENU;
	SDL_Rect posBoton;
	posBoton.x = width_window / 2 - 100;
	posBoton.y = 0.5 * div_alto;
	posBoton.w = 200;
	posBoton.h = 0.5 * div_alto;

	Boton *boton = new Boton(BOTON_NAME,posBoton);
	Imagen* image = Loader::cargarTexto(renderer,fuenteTexto,boton->getInfoBoton());
	boton->setImagen(image);
	botones.push_back(boton);

	posBoton.y += div_alto;
	boton = new Boton(BOTON_SELECTOR_ESCENARIO,posBoton);
	image = Loader::cargarTexto(renderer,fuenteTexto,boton->getInfoBoton());
	boton->setImagen(image);
	botones.push_back(boton);

	posBoton.y += div_alto;
	boton = new Boton(BOTON_CARGAR_PARTIDA,posBoton);
	image = Loader::cargarTexto(renderer,fuenteTexto,boton->getInfoBoton());
	boton->setImagen(image);
	botones.push_back(boton);

	posBoton.y += div_alto;
	boton = new Boton(BOTON_NUEVA_PARTIDA,posBoton);
	image = Loader::cargarTexto(renderer,fuenteTexto,boton->getInfoBoton());
	boton->setImagen(image);
	botones.push_back(boton);

	posBoton.y += div_alto;
	boton = new Boton(BOTON_EXIT,posBoton);
	image = Loader::cargarTexto(renderer,fuenteTexto,boton->getInfoBoton());
	boton->setImagen(image);
	botones.push_back(boton);
}

void VentanaMenu::mostrar(){
	bool quit = false;
	SDL_Event e;

	std::string labelIngresarNombre = " Nombre Jugador ";
	Imagen *textoLabelIP = Loader::cargarTexto(renderer, fuenteTexto, labelIngresarNombre);
	//SDL_Rect rect_label;
	//rect_label.x = SCREEN_WIDTH / 2 - textoLabelIP->getPixelsX()/2;
	//rect_label.y = SCREEN_HEIGHT / 3 - textoLabelIP->getPixelsY();
	//rect_label.w = textoLabelIP->getPixelsX();
	//rect_label.h = textoLabelIP->getPixelsY();

	std::string inputText = "";
	Imagen *textoInput = NULL;
	SDL_Rect rect_input;
	rect_input.x = SCREEN_WIDTH / 2 - textoLabelIP->getPixelsX() / 3;
	rect_input.y = SCREEN_HEIGHT / 2 + textoLabelIP->getPixelsY()/4;

	TipoBoton botonPresionado = BOTON_NADA;

	//SDL_StartTextInput();
	//While application is running
	while( !quit ){
		//The rerender text flag
		bool renderText = false;

		//Handle events on queue
		while( SDL_PollEvent( &e ) != 0 ){
			//User requests quit
			if( e.type == SDL_QUIT ) quit = true;

			if (e.type == SDL_MOUSEBUTTONDOWN){
				if (e.button.button == SDL_BUTTON_LEFT){
					Coordenada c;
					SDL_GetMouseState(&c.x,&c.y);
					botonPresionado = procesarClic(c);
				}
			}

			if (botonPresionado == BOTON_NAME){
				//SDL_StartTextInput();
				procesarTeclado(e,inputText,renderText);
				//SDL_StopTextInput();
			}

		} /* Fin while SDL_PollEvent*/
		SDL_StopTextInput();
		//Rerender text if needed
		if( renderText ){
			//Text is not empty
			if( inputText != "" ){
				//Render new text
				textoInput = Loader::cargarTexto(renderer,fuenteTexto,inputText);
			}
			//Text is empty
			else{
				//Render space texture
				textoInput = Loader::cargarTexto(renderer, fuenteTexto, " ");
			}
			imagenesBasura.push_back(textoInput);
		}

		//Clear screen
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, imagenFondo->getTexture(), NULL, NULL);

		for (unsigned i = 0; i < botones.size(); i++){
			Boton* boton = botones[i];
			SDL_Rect rect = boton->getPosicion();
			SDL_RenderCopy(renderer,fondoBoton->getTexture(),NULL,&rect);
			rect.w = boton->getImagen()->getPixelsX();
			rect.h = boton->getImagen()->getPixelsY();
			SDL_RenderCopy(renderer,boton->getImagen()->getTexture(),NULL,&rect);
		}

		//Render text textures

		//SDL_RenderCopy(renderer, textoLabelIP->getTexture(), NULL, &botones[0]->getPosicion());
		if (textoInput != NULL){
			rect_input.x = botones[0]->getPosicion().x;
			rect_input.y = botones[0]->getPosicion().y;
			rect_input.w = textoInput->getPixelsX();
			rect_input.h = textoInput->getPixelsY();
			SDL_RenderCopy(renderer, textoInput->getTexture(), NULL, &rect_input);
		}
		//Update screen
		SDL_RenderPresent(renderer);

	} /* Fin while quit */

	//Disable text input
	//SDL_StopTextInput();
	delete textoLabelIP;
}

TipoBoton VentanaMenu::procesarClic(Coordenada c){
	for (unsigned i = 0; i < botones.size(); i++){
		SDL_Rect rectBoton = botones[i]->getPosicion();
		if (rectBoton.x < c.x &&
			rectBoton.x + rectBoton.w > c.x &&
			rectBoton.y < c.y &&
			rectBoton.y + rectBoton.h > c.y)
			std::cout <<botones[i]->getInfoBoton()<<std::endl;
			return botones[i]->getTipo();
	}
	return BOTON_NADA;
}

void VentanaMenu::procesarTeclado(SDL_Event e, string &inputText, bool &renderText){
	if( e.type == SDL_KEYDOWN ){
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
}

VentanaMenu::~VentanaMenu() {
	close();
	delete this->imagenFondo;
	delete this->fondoBoton;
	for (unsigned i = 0; i < imagenesBasura.size(); i++){
		delete imagenesBasura[i];
	}
	for (unsigned i = 0; i < botones.size(); i++){
		delete botones[i];
	}
	imagenesBasura.clear();
}

