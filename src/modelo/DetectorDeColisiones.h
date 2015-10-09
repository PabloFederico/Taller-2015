/*
 * DetectorDeColisiones.h
 *
 *  Created on: 9 de oct. de 2015
 *      Author: emanuel
 */

#ifndef MODELO_DETECTORDECOLISIONES_H_
#define MODELO_DETECTORDECOLISIONES_H_
#include "../vista/Sprite.h"
#include "../utils/Map.h"
#include "Entidad.h"

class DetectorDeColisiones {
public:
	static bool verificarColisiones(Sprite* sprite, Map<Entidad*, Sprite*> *mapSprites);
};

#endif /* MODELO_DETECTORDECOLISIONES_H_ */
