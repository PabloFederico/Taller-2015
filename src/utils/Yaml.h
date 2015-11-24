/*
 * Yaml.h
 *
 *  Created on: 29 de oct. de 2015
 *      Author: emanuel
 */

#ifndef UTILS_YAML_H_
#define UTILS_YAML_H_
#include "../utils/Structs.h"

class Yaml {

public:

	static ConfiguracionJuego cargarConfiguracionJuego(std::string archivoConfiguracion);

	static ConfiguracionJuego OdioYAML();

	static InfoEscenario infoEscenarioDefault();
};

#endif /* UTILS_YAML_H_ */
