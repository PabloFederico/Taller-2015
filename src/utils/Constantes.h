/*
 * Constantes.h
 *
 *  Created on: 17 de sept. de 2015
 *      Author: emanuel
 */

#ifndef UTILS_CONSTANTES_H_
#define UTILS_CONSTANTES_H_
/* Los tamaños de pixeles son lineales, en caso de cambiar algún valor,
 * se debería cambiar el valor de algunas constantes según la relación que
 *  corresponda, salvo las cantidades de direcciones e imágenes que son
 *  independientes de las demás */

#define ANCHO_PIXEL_PASTO	60
#define ALTO_PIXEL_PASTO	30

#define DISTANCIA_ENTRE_X	0.5*ANCHO_PIXEL_PASTO
#define DISTANCIA_ENTRE_Y	0.5*ALTO_PIXEL_PASTO

#define DIRECCIONES			8
#define IMAGENES_DIFERENTES	8

#define PUERTO_SERVIDOR		4050
#define IP_SERVIDOR			"127.0.0.1"
#define NOMBRE_DEFAULT		"Dieguito"

#define MAX_ESPERA_CONEXION 10 // segundos
#define MAX_CONEXIONES		3 //5
#define MAX_BYTES_LECTURA	2000

#define DELAY_RECURSOS		75 // segundos

#define KEY_RESET 'r'

#define TAM_LETRA_CONEXION  30
#define TAM_LETRA_JUEGO     10

#define ALPHA_GRIS 		   140
#define ALPHA_COLOR		   255

#endif /* UTILS_CONSTANTES_H_ */
