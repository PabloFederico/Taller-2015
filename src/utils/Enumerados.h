/*
 * Enumerados.h
 *
 *  Created on: 26 de sept. de 2015
 *      Author: emanuel
 */

#ifndef UTILS_ENUMERADOS_H_
#define UTILS_ENUMERADOS_H_

enum TipoEntidad  { DEFAULT , PASTO, TIERRA , AGUA , ARBOL , CASTILLO, SOLDADO, JUANA_DE_ARCO, ANIMAL, OTROS };

/* Tipos de Errores:
 * * INFO = Error Informátivo
 * * WAR  = Warning
 * * ERR  = Error Secundario
 * * ERR_FAT = Error Fatal
 * */
enum TipoError { INFO, WAR, ERR, ERR_FAT};
/*
const char* toString(TipoError tipo){
	string tipo_s;
	switch (tipo){
	   case INFO : tipo_s = "INFO";
	   	   	   	   break;
	   case WAR  : tipo_s = "WAR";
	   	   	   	   break;
	   case ERR : tipo_s = "ERR";
	   	   	   	   break;
	   case ERR_FAT : tipo_s = "ERR_FAT";
	   	   	   	   	  break;
	   default : tipo_s = "DESCONOCIDO";
	}
	return tipo_s.c_str();
}
*/
#endif /* UTILS_ENUMERADOS_H_ */
