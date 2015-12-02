TALLER DE PROGRAMACION I [75.42]
AÑO: 2015
CUATRIMESTRE : 2°
NUMERO DE TP: 3

Grupo 2

NOMBRE Y APELLIDO DE INTEGRANTES:
Condo, Emanuel 94773
Cura, Martin 95874
Federico, Pablo 90280
Iogha, Octavio 95505
Negri, Guido 95615

Sistema Operativo: Linux (Ubuntu)

Bibliotecas usadas:
	SDL 2 (Repositorio ubuntu con comando sudo apt-get install libsdl2-dev)
	YAML (Repositorio ubuntu con comando sudo apt-get install libyaml-cpp-dev)
		-> Dependencia de YAML: LibBoost (sudo apt-get install libboost-all-dev)
		-> Dependencia de YAML : Cmake (sudo apt-get install cmake)


Cómo jugar:

Primero que nada, debe especificarse la dirección IP de quien será el servidor en
el archivo config.yaml.
Se debe correr una instancia de TP con el argumento 's' para iniciar el servidor.
Este permitirá elegir el modo de juego para la partida. Tras esto, se dispondrá
para escuchar conexiones entrantes. Ahora se podrán iniciar otras instancias con
el argumento 'c', indicando que son clientes. Estos se irán conectando al servidor
hasta cumplir el límite o no recibir más conexiones. Esperar unos segundos y
comenzará la partida.
