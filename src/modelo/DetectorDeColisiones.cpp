/*
 * DetectorDeColisiones.cpp
 *
 *  Created on: 9 de oct. de 2015
 *      Author: emanuel
 */

#include "DetectorDeColisiones.h"

bool DetectorDeColisiones::verificarColisiones(Sprite *sprite, Map<Entidad*, Sprite*> *mapSprites){
	bool colision = false;
	map<Entidad*, Sprite* >::iterator it = mapSprites->begin();
	while (it != mapSprites->end() && !colision){
		Sprite *otro = (*it).second;
		if (sprite != otro){
			colision = sprite->checkColision(otro);
		}
		it++;
	}
	return colision;
}

