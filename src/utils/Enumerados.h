/*
 * Enumerados.h
 *
 *  Created on: 26 de sept. de 2015
 *      Author: emanuel
 */

#ifndef UTILS_ENUMERADOS_H_
#define UTILS_ENUMERADOS_H_

enum TipoEntidad  { OTROS , PASTO, TIERRA , AGUA , ARBOL , CASTILLO, SOLDADO, JUANA_DE_ARCO, ANIMAL, DEFAULT };

enum TipoImagenUtil { BARRA_FONDO , BARRA_DESCRIPCION, SELECT_TILE };

enum TipoRecurso { MADERA, ORO, COMIDA};

/* Tipos de Log:
 * * INFO = Error Informátivo
 * * WAR  = Warning
 * * ERR  = Error Secundario
 * * ERR_FAT = Error Fatal
 * */
enum TipoLog { INFO, WAR, ERR, ERR_FAT};

inline const char* LogToString(TipoLog v){
    switch (v)
    {
        case INFO:   return "INFO";
        case WAR:   return "WAR";
        case ERR: return "ERR";
        case ERR_FAT: return "ERR_FAT";
        default:      return "Unknown";
    }
}

#endif /* UTILS_ENUMERADOS_H_ */
