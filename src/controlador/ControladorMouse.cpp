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
	Coordenada c_tile = Calculador::tileParaPixel(coord_pixel_sprite, Coordenada(coord_pixel_ceros.x + DISTANCIA_ENTRE_X, coord_pixel_ceros.y));
	//código de prueba
	//std::cout << *juego->getCeros().first << ";" << *juego->getCeros().second << std::endl;
	//std::cout <<"calcularPosicionInversa : " <<coord.x << ";" << coord.y << "     tileParaPixel : " << c_tile.x  << ";" << c_tile.y << std::endl;
	/*
	try {
		Coordenada coord2 = Calculador::tileParaPixel(Coordenada(MouseX, MouseY), Coordenada(coord_pixel_ceros.x + DISTANCIA_ENTRE_X, coord_pixel_ceros.y));
		std::cout << MouseX-(*juego->getCeros().first+DISTANCIA_ENTRE_X) << ";" << MouseY-*juego->getCeros().second << ": v" << coord.x  << ";" << coord.y << " | n" << coord2.x << ";" << coord2.y << std::endl;
		Coordenada coord3 = Calculador::pixelCentralDeTile(Coordenada(coord2.x,coord2.y));
		std::cout << coord3.x << ";" << coord3.y << endl;
	} catch ( FueraDeEscenario &e ) {}
	*/
	std::cout << std::endl;
	///

    juego->getEscenario()->getCapa()->descubrirDesdePunto(c_tile.x,c_tile.y);


	int Follow_Point_X = sprite->regPos.x_anterior;
	int Follow_Point_Y = sprite->regPos.y_anterior;
	//======Analisis del evento de movimiento======//
	if (event.type == SDL_MOUSEBUTTONDOWN){
		if (event.button.button == SDL_BUTTON_LEFT){
			//Coordenada c_tile = Calculador::tileParaPixel(Coordenada(MouseX,MouseY),Coordenada(*juego->getCeros().first, *juego->getCeros().second));
			//std::cout <<c_tile.x <<"  "<<c_tile.y << std::endl;
			Coordenada coord_pixel_ceros(*juego->getCeros().first, *juego->getCeros().second);
			bool clickValido = Calculador::puntoContenidoEnEscenario(Coordenada(MouseX,MouseY), coord_pixel_ceros, juego->getEscenario());
			if (clickValido){
				SDL_Rect posicionPlayer = sprite->getPosicion();
	            Follow_Point_X = MouseX - posicionPlayer.w / 2;
	            Follow_Point_Y = MouseY - posicionPlayer.h ;

	            Escenario *esc = juego->getEscenario();
	            Coordenada coord_orig(posicionPlayer.x + posicionPlayer.w / 2, posicionPlayer.y + posicionPlayer.h);
	            Coordenada coord_dest(Follow_Point_X, Follow_Point_Y);
	            vector<Coordenada> caminoMinimo = Calculador::obtenerCaminoMin(esc,coord_orig,coord_dest,coord_pixel_ceros);
	            std::cout <<caminoMinimo.size()<< std::endl;

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
			/*
			SDL_Rect posicionPlayer = sprite->getPosicion();
            Follow_Point_X = MouseX - posicionPlayer.w / 2;
            Follow_Point_Y = MouseY - posicionPlayer.h ;
			*/
            /* Validación de click dentro del escenario */
           // pair<int*,int*> ceros = this->juego->getCeros();
            /*
			Coordenada c_pixel(Follow_Point_X + posicionPlayer.w, Follow_Point_Y+posicionPlayer.h);
            Coordenada c_pixel_ceros(*juego->getCeros().first, *juego->getCeros().second);

            if (Calculador::puntoContenidoEnEscenario(c_pixel, c_pixel_ceros, juego->getEscenario())){
                sprite->activarMovimiento(true);
                sprite->regPos.x_anterior = Follow_Point_X;
                sprite->regPos.y_anterior = Follow_Point_Y;
                Direccion direccion = Calculador::calcularDireccion(Coordenada(Follow_Point_X, Follow_Point_Y), Coordenada(posicionPlayer.x, posicionPlayer.y));
                sprite->setDireccion(direccion);
            }else {
            	* Si el click esta fuera del escenario, su punto destino será
            	 * el anterior al click (si es que se encontraba en movimiento) *
            	Follow_Point_X = sprite->regPos.x_anterior;
            	Follow_Point_Y = sprite->regPos.y_anterior;
            }
			*/
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

