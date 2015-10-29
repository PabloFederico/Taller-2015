/*
 * Yaml.h
 *
 *  Created on: 29 de oct. de 2015
 *      Author: emanuel
 */

#ifndef MODELO_YAML_H_
#define MODELO_YAML_H_
#include "../utils/Structs.h"

class Yaml {

public:

	static ConfiguracionJuego cargarConfiguracionJuego(std::string archivoConfiguracion);

	static ConfiguracionJuego OdioYAML();

	static InfoEscenario infoEscenarioDefault();
};

#endif /* MODELO_YAML_H_ */
