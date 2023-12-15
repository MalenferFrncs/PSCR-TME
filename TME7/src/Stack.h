#pragma once

#include <cstring> // size_t,memset
#include <semaphore.h>

namespace pr {

#define STACKSIZE 100

template<typename T>
class Stack {

	/* TODO
	* mutex pour les sections critiques
	* sémaphore pour compter les cases vides
	* dans le constructeur, sem init pour les positionner
	* 
	*/
	T tab [STACKSIZE];

	sem_t sem_push;
	sem_t sem_pop;
	sem_t sem_mutex;
	
	size_t sz;
public :
	Stack () : sz(0) { 
		sem_init (&sem_pop, 1, 0);
		sem_init (&sem_push, 1, STACKSIZE);
		sem_init (&sem_mutex, 1, 1);
		memset(tab,0,sizeof tab) ;
		}

	T pop () {
		// bloquer si vide
		sem_wait(&sem_pop);

		sem_wait (&sem_mutex);
		T toret = tab[--sz];
		sem_post (&sem_mutex);

		sem_post (&sem_push);

		return toret;
	}

	void push(T elt) {
		sem_wait(&sem_push);

		sem_wait (&sem_mutex);

		tab[sz++] = elt;

		sem_post (&sem_mutex);

		sem_post (&sem_pop);


	}

	~Stack(){
		sem_destroy(&sem_pop);
		sem_destroy(&sem_push);
		sem_destroy(&sem_mutex);
	}
};

}
