/*
 * Enumerados.h
 *
 *  Created on: 26 de sept. de 2015
 *      Author: emanuel
 */

#ifndef UTILS_ENUMERADOS_H_
#define UTILS_ENUMERADOS_H_

#include <string>


enum TipoEntidad  { OTROS , PASTO, TIERRA , AGUA , ARBOL , CASTILLO, SOLDADO, JUANA_DE_ARCO, ANIMAL, MADERA, COMIDA, ORO, DEFAULT, CONTORNO, CONTORNOXL };

enum TipoImagenUtil { CUADRADO_MAGENTA, CUADRADO_ROJO, CUADRADO_AZUL, CUADRADO_VERDE,
					BARRA_FONDO , BARRA_DESCRIPCION, BARRA_NEGRA, SELECT_TILE,
					ICONO_ROJO, ICONO_AZUL, ICONO_AMARILLO, ICONO_VERDE, ICONO_ROSA,
					ICONO_GRIS, ICONO_NARANJA, ICONO_VIOLETA, ICONO_BLANCO, ICONO_MAGENTA,
					CAPA_GRIS, CAPA_NEGRA, CUADRO_UBICACION };

enum TipoMensajeRed { MENSAJE, COMIENZO, ESCENARIO, MOVIMIENTO, NUEVA_ENTIDAD, RECURSO, TOGGLE, ATAQUE, GLOTON, FIN /*...*/};

enum TipoBoton { BOTON_NADA, BOTON_NAME, BOTON_NUEVA_PARTIDA, BOTON_SELECTOR_ESCENARIO, BOTON_CARGAR_PARTIDA, BOTON_EXIT };

/* Tipos de Log:
 * * INFO = Error Informátivo
 * * WAR  = Warning
 * * ERR  = Error Secundario
 * * ERR_FAT = Error Fatal
 * */
enum TipoLog { INFO, WAR, ERR, ERR_FAT };

inline const char* LogToString(TipoLog v){
    switch (v)
    {
        case INFO:	  return "INFO";
        case WAR:	  return "WAR";
        case ERR:	  return "ERR";
        case ERR_FAT: return "ERR_FAT";
        default:	  return "Unknown";
    }
}

inline const TipoMensajeRed StringToTipoMensajeRed(std::string s) {
	if (s == "MSJ")	return MENSAJE;
	if (s == "COM")	return COMIENZO;
	if (s == "ESC")	return ESCENARIO;
	if (s == "MOV")	return MOVIMIENTO;
	if (s == "ENT")	return NUEVA_ENTIDAD;
	if (s == "REC") return RECURSO;
	if (s == "TOG") return TOGGLE;
	if (s == "GLO") return GLOTON;
	if (s == "ATQ")	return ATAQUE;
	if (s == "FIN") return FIN;
	else return MENSAJE;
}

inline const bool EsRecurso(TipoEntidad tipo) {
	if (tipo == MADERA || tipo == COMIDA || tipo == ORO)
		return true;
	return false;
}

enum EstadoCapa { ESTADO_NEGRO, ESTADO_GRIS, ESTADO_COLOR};

#endif /* UTILS_ENUMERADOS_H_ */
