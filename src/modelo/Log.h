/*
 * Log.h
 *
 *  Created on: 11 de oct. de 2015
 *      Author: emanuel
 */

#ifndef MODELO_LOG_H_
#define MODELO_LOG_H_
#include <ctime>
#include <fstream>
#include "../utils/Enumerados.h"

class Log {
public:
	static void imprimirALog(TipoLog tipo, std::string output){
		time_t rawtime;
		struct tm * timeinfo;
		char buffer[80];
		time (&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(buffer,80,"%Y-%m-%d %I:%M:%S",timeinfo);
		std::string timestamp(buffer);

		ofstream fLog("TP.log", std::ofstream::app);
		fLog << "[" << timestamp << "]" << '\t' <<LogToString(tipo)<< '\t' << output << std::endl;
		fLog.close();
		std::cerr <<output <<std::endl;
	};
};

#endif /* MODELO_LOG_H_ */
