/*
 * Thread.h
 *
 *  Created on: 6 de oct. de 2015
 *      Author: emanuel
 */

#ifndef UTILS_THREAD_H_
#define UTILS_THREAD_H_
#include <pthread.h>
#include <stdio.h>

class Thread {
private:
	pthread_t hilo;

public:
	Thread();

	/* Método virtual. Todas las clases que hereden de Thread deberán implementar el
	 * método run. */
	virtual void* run(){return NULL;};

	/* Crea el hilo y empieza a ejecutar la función run. */
	void ejecutar();

	/*  */
	void join();

	virtual ~Thread();
};

#endif /* UTILS_THREAD_H_ */
