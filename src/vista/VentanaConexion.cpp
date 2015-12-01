/*
 * VentanaConexion.cpp
 *
 *  Created on: 22 de oct. de 2015
 *      Author: emanuel
 */

#include "VentanaConexion.h"
#include "../utils/Loader.h"
#include "../utils/Log.h"
#include "../vista/Boton.h"
#include <iostream>

VentanaConexion::VentanaConexion(Controller *controlador): Ventana(controlador) {
	if ( !init() ) std::cout << "error" << std::endl;
	this->fuenteTexto = TTF_OpenFont("images/utils/censcbk.ttf",TAM_LETRA_CONEXION);
	imagenFondo = Loader::cargarImagen(renderer,"images/utils/fondo_ventana.png");
	imagenFondoCampo = Loader::cargarImagen(renderer,"images/utils/fondo_campo1.png");
}

Campo campoACompletar(Mouse* mouse, vector<pair<SDL_Rect,Campo> > vec){
	Campo campo = NINGUNO;
	Coordenada c = mouse->getXY();
	for (unsigned i = 0; i < vec.size(); i++){
		int x = vec[i].first.x;
		int y = vec[i].first.y;
		int w = vec[i].first.w;
		int h = vec[i].first.h;
		if (c.x > x+w  &&  c.x < x+2*w  && c.y > y  &&  c.y < y+h){
		   campo = vec[i].second;
		}
	}
	return campo;
}

EstadoFinVentana VentanaConexion::run(){
	bool quit = false;
	EstadoFinVentana estado = OK;
	SDL_Event e;

	Imagen *imageLabelIP = Loader::cargarTexto(renderer, fuenteTexto, "Ingrese IP del Servidor : ");
	Imagen *imageLabelName = Loader::cargarTexto(renderer, fuenteTexto, "Ingrese su Nombre : ");
	Imagen *imageLabelPort = Loader::cargarTexto(renderer,fuenteTexto,"Puerto : ");

	SDL_Rect rect_labelIP;
	rect_labelIP.x = SCREEN_WIDTH / 2 - imageLabelName->getPixelsX();
	rect_labelIP.y = SCREEN_HEIGHT / 4;
	rect_labelIP.w = imageLabelIP->getPixelsX();
	rect_labelIP.h = imageLabelIP->getPixelsY();

	SDL_Rect rect_labelName;
	rect_labelName.x = SCREEN_WIDTH / 2 - imageLabelName->getPixelsX();
	rect_labelName.y = 0.4 * SCREEN_HEIGHT;
	rect_labelName.w = imageLabelName->getPixelsX();
	rect_labelName.h = imageLabelName->getPixelsY();

	SDL_Rect rect_labelPort;
	rect_labelPort.x = SCREEN_WIDTH / 2 - imageLabelPort->getPixelsX();
	rect_labelPort.y = SCREEN_HEIGHT / 2 + imageLabelPort->getPixelsY();
	rect_labelPort.w = imageLabelPort->getPixelsX();
	rect_labelPort.h = imageLabelPort->getPixelsY();

	std::string inputIP = "";
	std::string inputName = "";
	std::string inputPort = "";

	vector<pair<SDL_Rect,Campo> > par_de_campos;
	par_de_campos.push_back(make_pair(rect_labelIP,IP));
	par_de_campos.push_back(make_pair(rect_labelName,NOMBRE));
	par_de_campos.push_back(make_pair(rect_labelPort,PUERTO));

	Imagen *imageInputIP = NULL;
	Imagen *imageInputName = NULL;
	Imagen *imageInputPort = NULL;

	SDL_Rect rect_inputIP;
	rect_inputIP.x = rect_labelIP.x + rect_labelIP.w;
	rect_inputIP.y = rect_labelIP.y;

	SDL_Rect rect_inputName;
	rect_inputName.x = rect_labelName.x + rect_labelName.w;
	rect_inputName.y = rect_labelName.y;

	SDL_Rect rect_inputPort;
	rect_inputPort.x = rect_labelPort.x + rect_labelPort.w;
	rect_inputPort.y = rect_labelPort.y;

	SDL_Rect rect_boton;
	rect_boton.x = 450;
	rect_boton.y = 500;
	Boton* buttonConnect = new Boton("Connect",rect_boton);
	Imagen* imageButtonConnect = Loader::cargarImagen(renderer,"images/utils/boton_conectar.png");
	buttonConnect->setImagen(imageButtonConnect);
	rect_boton = buttonConnect->getPosicion();

	SDL_Rect rect_fondo_campo_ip;
	rect_fondo_campo_ip.x = rect_labelIP.x - 10;
	rect_fondo_campo_ip.y = rect_labelIP.y;
	rect_fondo_campo_ip.w = 600;
	rect_fondo_campo_ip.h = rect_labelIP.h;

	SDL_Rect rect_fondo_campo_nombre;
	rect_fondo_campo_nombre.x = rect_labelName.x - 10;
	rect_fondo_campo_nombre.y = rect_labelName.y;
	rect_fondo_campo_nombre.w = 600;
	rect_fondo_campo_nombre.h = rect_labelName.h;

	SDL_Rect rect_fondo_campo_puerto;
	rect_fondo_campo_puerto.x = rect_labelPort.x - 10;
	rect_fondo_campo_puerto.y = rect_labelPort.y;
	rect_fondo_campo_puerto.w = 250;
	rect_fondo_campo_puerto.h = rect_labelPort.h;

	Campo campoSelecionado = NINGUNO;
	std::string inputText = "";

	while( !quit ){

		while(SDL_PollEvent( &e )){

			if( e.type == SDL_QUIT ) {
				estado = EXIT;
				quit = true;
			}

			if (e.button.button == SDL_BUTTON_LEFT){
				int x,y;
				SDL_GetMouseState(&x,&y);
				controlador->getMouse()->setEstado(CLICK_IZQUIERDO);
				controlador->getMouse()->setXY(Coordenada(x,y));
				if (controlador->getMouse()->botonFuePresionado(buttonConnect)) {
					if (inputIP != "" && inputName != "" && inputPort != ""){
						int puerto = 8888;//std::stoi(inputPort);
						bool conexion_ok = controlador->realizarConexion(inputIP,inputName,puerto);
						if (conexion_ok)
							quit = true;
						else{
							inputIP = "";
							inputName = "";
							inputPort = "";
						}
					}
					buttonConnect->resetBoton();
				}
				campoSelecionado = campoACompletar(controlador->getMouse(),par_de_campos);
			}

			//Special key input
			if( campoSelecionado != NINGUNO && e.type == SDL_KEYDOWN ){

				switch (campoSelecionado){
				case IP : inputText = inputIP;
						  break;
				case NOMBRE : inputText = inputName;
							  break;
				default : inputText = inputPort;
						  break;
				}

				//Handle backspace
				if( e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0 ){
					//lop off character
					inputText.erase(inputText.length()-1);
				}
				//Handle copy
				else if( e.key.keysym.sym == SDLK_c && (SDL_GetModState() & KMOD_CTRL) ){
					SDL_SetClipboardText( inputText.c_str() );
				}
				//Handle paste
				else if( e.key.keysym.sym == SDLK_v && (SDL_GetModState() & KMOD_CTRL) ){
					inputText = SDL_GetClipboardText();
				}

				switch (campoSelecionado){
				case IP : inputIP = inputText;
						  break;
				case NOMBRE : inputName = inputText;
							  break;
				default : inputPort = inputText;
						  break;
				}
			}

			//Special text input event
			else if( campoSelecionado != NINGUNO && e.type == SDL_TEXTINPUT ){
				switch (campoSelecionado){
				case IP : inputText = inputIP;
						  break;
				case NOMBRE : inputText = inputName;
							  break;
				default : inputText = inputPort;
						  break;
				}

				//Not copy or pasting
				if( !( ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' ) && ( e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V' ) && (SDL_GetModState() & KMOD_CTRL) ) )
				{
					if (campoSelecionado != PUERTO && inputText.length() < 14)
						//Append character
						inputText += e.text.text;
					if (campoSelecionado == PUERTO && inputText.length() < 5)
						//Append character
						inputText += e.text.text;
					switch (campoSelecionado){
					case IP : inputIP = inputText;
							  break;
					case NOMBRE : inputName = inputText;
								  break;
					default : inputPort = inputText;
							  break;
					}


				}

			}
		} /* Fin while SDL_PollEvent*/

		if (imageInputIP != NULL) delete imageInputIP;
		if (inputIP != "") imageInputIP = Loader::cargarTexto(renderer,fuenteTexto,inputIP);
		else imageInputIP = Loader::cargarTexto(renderer,fuenteTexto," ");

		if (imageInputName) delete imageInputName;
		if (inputName != "") imageInputName = Loader::cargarTexto(renderer,fuenteTexto,inputName);
		else imageInputName = Loader::cargarTexto(renderer,fuenteTexto," ");

		if (imageInputPort != NULL) delete imageInputPort;
		if (inputPort != "") imageInputPort = Loader::cargarTexto(renderer,fuenteTexto,inputPort);
		else imageInputPort = Loader::cargarTexto(renderer,fuenteTexto," ");

		rect_inputIP.w = imageInputIP->getPixelsX();
		rect_inputIP.h = imageInputIP->getPixelsY();
		rect_inputName.w = imageInputName->getPixelsX();
		rect_inputName.h = imageInputName->getPixelsY();
		rect_inputPort.w = imageInputPort->getPixelsX();
		rect_inputPort.h = imageInputPort->getPixelsY();

		//Clear screen
		SDL_RenderClear(renderer);

		//Render text textures
		SDL_RenderCopy(renderer, imagenFondo->getTexture(), NULL, NULL);
		SDL_RenderCopy(renderer,imagenFondoCampo->getTexture(), NULL, &rect_fondo_campo_ip);
		SDL_RenderCopy(renderer,imagenFondoCampo->getTexture(), NULL, &rect_fondo_campo_nombre);
		SDL_RenderCopy(renderer,imagenFondoCampo->getTexture(), NULL, &rect_fondo_campo_puerto);
		SDL_RenderCopy(renderer, imageLabelIP->getTexture(), NULL, &rect_labelIP);
		SDL_RenderCopy(renderer, imageInputIP->getTexture(), NULL, &rect_inputIP);
		SDL_RenderCopy(renderer, imageLabelName->getTexture(), NULL, &rect_labelName);
		SDL_RenderCopy(renderer, imageInputName->getTexture(), NULL, &rect_inputName);
		SDL_RenderCopy(renderer, imageLabelPort->getTexture(), NULL, &rect_labelPort);
		SDL_RenderCopy(renderer, imageInputPort->getTexture(), NULL, &rect_inputPort);
		SDL_Rect rect_frame_actual = buttonConnect->getFrameActual();
		SDL_Rect rect_button_connect = buttonConnect->getPosicion();
		SDL_RenderCopy(renderer, imageButtonConnect->getTexture(), &rect_frame_actual, &rect_button_connect);

		//Update screen
		SDL_RenderPresent(renderer);

		//SDL_Delay(20);

	} /* Fin while quit */
	delete buttonConnect;
	delete imageLabelIP;
	delete imageLabelName;
	delete imageLabelPort;
	delete imageInputIP;
	delete imageInputName;
	delete imageInputPort;

	return estado;
}

VentanaConexion::~VentanaConexion() {
	close();
	delete this->imagenFondo;
	delete this->imagenFondoCampo;
}

