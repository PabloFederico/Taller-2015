/*
 * Thread.cpp
 *
 *  Created on: 6 de oct. de 2015
 *      Author: emanuel
 */

#include "../utils/Thread.h"

Thread::Thread() {
	hilo = 0;
}

static void* runFuncion(void* funcion){
	((Thread*)funcion)->run();
	return NULL;
}

void Thread::ejecutar(){
	pthread_create(&hilo,NULL,runFuncion,this);
}

void Thread::join(){
	pthread_join(hilo,NULL);
}

Thread::~Thread() {
	pthread_exit(NULL);
}

