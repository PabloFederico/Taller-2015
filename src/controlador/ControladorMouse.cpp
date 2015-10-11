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

	/* Actualiza la capa negra */
	int x = sprite->getPosicion().x + sprite->getPosicion().w;
	int y = sprite->getPosicion().y + sprite->getPosicion().h;
	Coordenada coord_pixel_sprite(x,y);
	Coordenada coord_pixel_ceros(*juego->getCeros().first, *juego->getCeros().second);

	Coordenada coord = Calculador::calcularPosicionInversa(coord_pixel_sprite,coord_pixel_ceros,juego->getEscenario());

    juego->getEscenario()->getCapa()->descubrirDesdePunto(coord.x,coord.y);


	int Follow_Point_X = sprite->regPos.x_anterior;
	int Follow_Point_Y = sprite->regPos.y_anterior;
	//======Analisis del evento de movimiento======//
	if (event.type == SDL_MOUSEBUTTONDOWN){
		if (event.button.button == SDL_BUTTON_LEFT){
			SDL_Rect posicionPlayer = sprite->getPosicion();
            Follow_Point_X = MouseX - posicionPlayer.w / 2;
            Follow_Point_Y = MouseY - posicionPlayer.h ;


			////código de prueba
        	std::pair<int,int> coord2, coord3;
        	try {
        	//	std::pair<int,int> coord3 = Calculador::tileParaPixel(MouseX, MouseY, *juego->getCeros().first+DISTANCIA_ENTRE_X, *juego->getCeros().second);
        	//	std::pair<int,int> coord2 = Calculador::tileParaPixel(posicionPlayer.x, posicionPlayer.y, *juego->getCeros().first+DISTANCIA_ENTRE_X, *juego->getCeros().second);
        		//std::cout << MouseX-(*juego->getCeros().first+DISTANCIA_ENTRE_X) << ";" << MouseY-*juego->getCeros().second << ": v" << coord.first  << ";" << coord.second << " | n" << coord2.first << ";" << coord2.second << std::endl;
			//	cout << "["<<coord2.first<<";"<<coord2.second<<"|"<<coord3.first<<";"<<coord3.second<<"] " << "cm: ";

        		vector<Coordenada> vec = Calculador::obtenerCaminoMin(this->juego->getEscenario(), Coordenada(posicionPlayer.x, posicionPlayer.y), Coordenada(MouseX, MouseY), Coordenada(*juego->getCeros().first+DISTANCIA_ENTRE_X, *juego->getCeros().second));
				for (vector<Coordenada>::iterator it = vec.begin(); it < vec.end(); ++it) {
					try {
						Coordenada tile = Calculador::tileParaPixel(*it, Coordenada(*juego->getCeros().first+DISTANCIA_ENTRE_X, *juego->getCeros().second));
						cout << tile.x << ";" << tile.y << " ";
					} catch ( FueraDeEscenario &e ) { cout << "Fuera de escenario "; }
				}
				cout << endl;
			} catch ( FueraDeEscenario &e ) {}
        	////


            /* Validación de click dentro del escenario */
           // pair<int*,int*> ceros = this->juego->getCeros();
            Coordenada c_pixel(Follow_Point_X + posicionPlayer.w, Follow_Point_Y+posicionPlayer.h);
            Coordenada c_pixel_ceros(*juego->getCeros().first, *juego->getCeros().second);

            if (Calculador::puntoContenidoEnEscenario(c_pixel, c_pixel_ceros, juego->getEscenario())){
                sprite->activarMovimiento(true);
                sprite->regPos.x_anterior = Follow_Point_X;
                sprite->regPos.y_anterior = Follow_Point_Y;
                Direccion direccion = Calculador::calcularDireccion(Coordenada(Follow_Point_X, Follow_Point_Y), Coordenada(posicionPlayer.x, posicionPlayer.y));
                sprite->setDireccion(direccion);
            }else {
            	/* Si el click esta fuera del escenario, su punto destino será
            	 * el anterior al click (si es que se encontraba en movimiento) */
            	Follow_Point_X = sprite->regPos.x_anterior;
            	Follow_Point_Y = sprite->regPos.y_anterior;
            }

		}
	}

    if (sprite->estaEnMovimiento()) {
    	float distance = Calculador::calcularDistanciaEntrePixeles(Coordenada(sprite->regPos.posX_player, sprite->regPos.posY_player), Coordenada(Follow_Point_X, Follow_Point_Y));

		if (distance > 1.0){
            if (sprite->regPos.posX_player != Follow_Point_X) {
            	float x_result = (sprite->regPos.posX_player - ((sprite->regPos.posX_player - Follow_Point_X) / distance) *(this->juego->getVelocidad())  * 0.05);
            	sprite->setPosX(int(x_result));
            	sprite->regPos.posX_player = x_result;
            }

            if (sprite->regPos.posY_player != Follow_Point_Y) {
                float y_result = (sprite->regPos.posY_player - ((sprite->regPos.posY_player - Follow_Point_Y) / distance) * (juego->getVelocidad()) * 0.05);
                sprite->setPosY(int(y_result));
                sprite->regPos.posY_player = y_result;
            }
            if (sprite->currentTime() > (1000/sprite->getFps())){
            	sprite->efectuarMovimiento();
            }

            bool hayColision = DetectorDeColisiones::verificarColisiones(sprite,juego->getSpritesEntidades());
            if (hayColision) sprite->activarMovimiento(false);

		}else  sprite->activarMovimiento(false);

    }else{
    	/* Cuando se deja de mover, se queda en una posición firme */
    	 sprite->acomodar();
    }
    //=======Fin de analisis de evento de movimiento=========//
}

ControladorMouse::~ControladorMouse() {
	// TODO Auto-generated destructor stub
}

