#pragma once

#include "Queue.h"
#include "Job.h"
#include <vector>
#include "Barrier.h"
#include <thread>

namespace pr {


	void poolWorker(Queue<Job> & queue){
	/*
	qui sera le corps des
	threads gérés par le Pool. Les threads tournent en boucle sur le comportement suivant :
	• extraire un Job de la queue
	• le traiter*/
		while(true){
			Job* j = queue.pop();		//on récupère un travail à faire dans la queue
			if(j == nullptr){
				break;
			}
			j->run();					//on lance le travail à faire
			delete j;					//On supprime le travail à faire une fois qu'il est fini
		}
	}

class Pool {
	Queue<Job> queue;
	std::vector<std::thread> threads;
	Barrier barrier;			//objet servant à la synchronisation
public:
	Pool(int qsize) : queue(qsize), barrier(qsize){	};
	
	void start (int nbthread){
		threads.reserve(nbthread);
		for(int i = 0 ; i < nbthread ;  ++i){
			threads.emplace_back(poolWorker, std::ref(queue));
		}
	}


	void submit (Job * job){
		queue.push(job);
		barrier.waitFor();		//après avoir soumis un travail à faire, on se met en arrêt sur la barrière
	}

	void stop(){
		queue.setBlocking(false);
		for (auto & t : threads){
			t.join();
			barrier.done(); //On notifie à la barrière qu'un worker s'est arrêté
		}
	}
	~Pool() {};	//rien à faire dans le destructeur
};

}