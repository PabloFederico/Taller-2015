/*
 * ControladorMouse.cpp
 *
 *  Created on: 8 de oct. de 2015
 *      Author: emanuel
 */

#include "ControladorMouse.h"
#include "../utils/Calculador.h"
#include "../modelo/DetectorDeColisiones.h"

ControladorMouse::ControladorMouse(Juego *juego) {
	this->juego = juego;
}

void ControladorMouse::procesarEvento(SDL_Event &event, int MouseX, int MouseY){
	Sprite *sprite = juego->getSpritePlayer();

	/********** Actualización de la capa negra ***********/
	int x = sprite->getPosicion().x + sprite->getPosicion().w / 2;
	int y = sprite->getPosicion().y + sprite->getPosicion().h / 2;

	Coordenada coord_pixel_sprite(x,y);
	Coordenada coord_pixel_ceros(*juego->getCeros().first + DISTANCIA_ENTRE_X, *juego->getCeros().second);

	try {
		Coordenada coord_tile_sprite = Calculador::tileParaPixel(coord_pixel_sprite,coord_pixel_ceros);
		juego->getEscenario()->getCapa()->descubrirDesdePunto(coord_tile_sprite.x, coord_tile_sprite.y);
	}catch (FueraDeEscenario &e){}


	/*********** Análisis del clic del mouse *************/
	if (event.type == SDL_MOUSEBUTTONDOWN){
		if (event.button.button == SDL_BUTTON_LEFT){
			bool clicValido;
			Escenario *escenario = juego->getEscenario();

			try{
				Coordenada c_tile_clic = Calculador::tileParaPixel(Coordenada(MouseX,MouseY), coord_pixel_ceros);
				clicValido = Calculador::puntoContenidoEnEscenario(c_tile_clic, escenario);
			}catch(FueraDeEscenario &e) {
				clicValido = false;
			}

			/* Si el clic es válido, buscamos el camino mínimo. */
			if (clicValido){
				SDL_Rect posicionPlayer = sprite->getPosicion();
	            int Follow_Point_X = MouseX - posicionPlayer.w / 2;
	            int Follow_Point_Y = MouseY - posicionPlayer.h;

				////código de prueba
	            vector<Coordenada> vec;
	        	try {
	        		cout << MouseX << ";" << MouseY << "/" << Follow_Point_X << ";" << Follow_Point_Y << endl;
 	        		Coordenada coord_tile_sprite = Calculador::tileParaPixel(coord_pixel_sprite, coord_pixel_ceros);
	        		cout << coord_tile_sprite.x << ";" << coord_tile_sprite.y << " ";
	        		vec = Calculador::obtenerCaminoMin(escenario, coord_pixel_sprite, Coordenada(Follow_Point_X, Follow_Point_Y), coord_pixel_ceros);
					for (vector<Coordenada>::iterator it = vec.begin(); it < vec.end(); ++it) {
						try {
							Coordenada tile = Calculador::tileParaPixel(*it, coord_pixel_ceros);
							cout << tile.x << ";" << tile.y << " ";
						} catch ( FueraDeEscenario &e ) { cout << "Fuera de escenario "; }
					}
					cout << endl;
				} catch ( FueraDeEscenario &e ) {}
	        	////

				/* Activamos el movimiento del sprite y seteamos el nuevo camino que debe recorrer. */
				if (vec.size() > 0) {
					 Coordenada c_prox_punto = vec[0];
					 c_prox_punto.x -= sprite->getPosicion().w / 2;
					 c_prox_punto.y -= sprite->getPosicion().h / 2;

					 Direccion direccion = Calculador::calcularDireccion(c_prox_punto, coord_pixel_sprite);
					 sprite->setDireccion(direccion);
					 sprite->activarMovimiento(true);
		             sprite->setearNuevoCamino(vec);
				 }

			}
		}
	} /* Fin if SDL_MOUSEBUTTONDOWN */


	if (sprite->quedaCaminoPorRecorrer()){
		/*	- calcular distancia al sig punto
		 *  - si es mayor a 1, seguir moviendo la posicion
		 *    sino cambiar al siguiente punto del camino (analizando la dirección)
		 *  - si ya no quedan camino a recorrer, cambiar estado de activar movimiento y acomodar
		 * */
		Coordenada c_prox_pixel = sprite->getCaminoARecorrer()[0];

		/* Esta condición es para que el chabon se ubique en el centro del tile,
		 * pero no se aplica para el último punto destino. */
		if (sprite->getCaminoARecorrer().size() > 1){
			c_prox_pixel.x -= sprite->getPosicion().w / 2;
			c_prox_pixel.y -= sprite->getPosicion().h / 2;
		}

		float distancia = Calculador::calcularDistanciaEntrePixeles(Coordenada(sprite->regPos.posX_player, sprite->regPos.posY_player), c_prox_pixel);

		if (distancia > 1){

            if (sprite->regPos.posX_player != c_prox_pixel.x) {
            	float x_result = (sprite->regPos.posX_player - ((sprite->regPos.posX_player - c_prox_pixel.x) / distancia) *(juego->getVelocidad())  * 0.05);
            	sprite->setPosX(int(x_result));
            	sprite->regPos.posX_player = x_result;
            }

            if (sprite->regPos.posY_player != c_prox_pixel.y) {
                float y_result = (sprite->regPos.posY_player - ((sprite->regPos.posY_player - c_prox_pixel.y) / distancia) * (juego->getVelocidad()) * 0.05);
                sprite->setPosY(int(y_result));
                sprite->regPos.posY_player = y_result;
            }

		}else{
			/* cambiar a la próxima coordenada */
			 sprite->quitarPrimeraCoordenada();

			 /* Seteamos la dirección para el siguiente punto. */
			 if (sprite->getCaminoARecorrer().size() > 0){
				 Coordenada c_prox_punto = sprite->getCaminoARecorrer()[0];
				 c_prox_punto.x -= sprite->getPosicion().w / 2;
				 c_prox_punto.y -= sprite->getPosicion().h / 2;

				 Direccion direccion = Calculador::calcularDireccion(c_prox_punto, coord_pixel_sprite);
			     sprite->setDireccion(direccion);
			 }

		}

	}else{
		/* Cuando se deja de mover, se debería quedar en una posición
		 * firme correspondiente a su dirección. */
		 sprite->activarMovimiento(false);
		 sprite->acomodar();
	}

           // bool hayColision = DetectorDeColisiones::verificarColisiones(sprite,juego->getSpritesEntidades());
           // if (hayColision) sprite->activarMovimiento(false);
}

ControladorMouse::~ControladorMouse() {
	// TODO Auto-generated destructor stub
}

