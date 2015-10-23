/*
 * Loader.h
 *
 *  Created on: 16 de sept. de 2015
 *      Author: emanuel
 */

#ifndef UTILS_LOADER_H_
#define UTILS_LOADER_H_
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <SDL2/SDL_ttf.h>
#include "../vista/Imagen.h"

class Loader {
public:
	static Imagen* cargarImagen(SDL_Renderer* renderer, std::string path){
		SDL_Texture *texture = NULL;
		SDL_Surface* surface = IMG_Load(path.c_str());


		texture = SDL_CreateTextureFromSurface(renderer,surface);
		Imagen* imagen = new Imagen(surface->w, surface->h, texture);
		SDL_FreeSurface(surface);
		surface = NULL;

		return imagen;
	};
/*
	static Texto* cargarTexto(SDL_Renderer* renderer, TTF_Font* fuente, std::string texto){
		SDL_Texture* texture = NULL;
		SDL_Color colorText = {0,0,0,0xFF};
		SDL_Surface* surface = TTF_RenderText_Blended(fuente, texto.c_str(), colorText);
		texture = SDL_CreateTextureFromSurface(renderer,surface);
		Texto* text = new Texto(texto, texture);
		SDL_FreeSurface(surface);
		return text;
	}
*/
	static Imagen* cargarTextoConFondo(SDL_Renderer* renderer, TTF_Font* fuente, std::string texto, SDL_Color backgroundColor){
		SDL_Texture* texture = NULL;
		SDL_Color colorText = {0,0,0,0xFF};
		SDL_Surface* surface = TTF_RenderText_Shaded(fuente, texto.c_str(), colorText, backgroundColor);
		texture = SDL_CreateTextureFromSurface(renderer,surface);
		Imagen* imagen = new Imagen(surface->w, surface->h, texture);
		SDL_FreeSurface(surface);
		surface = NULL;
		return imagen;
	}
};

#endif /* UTILS_LOADER_H_ */
