#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int main () {
	const int N = 3;
	pid_t racine_pid = getpid();
	std::cout << "main pid=" << getpid() << std::endl;
	int cpt_fils = 0;
	int j = N ;
	int i = 1;
	for (; i<=N && j==N ; i++ ) {
		if (fork()==0) { /* fils 1er niveau*/ cpt_fils++;
		std::cout << " i:j " << i << ":" << j << std::endl;
		for (int k=1; k<=i && j==N ; k++) {
			if ( fork() == 0) {
				j=0;
				std::cout << " k:j " << k << ":" << j << std::endl;
			}
		}
		


		}else{
			cpt_fils++;
			break;
		}
		
	}


	if (getpid()==racine_pid){//On est la racine, on n'a qu'un fils !
		if (wait(NULL) == -1){
			std::cout << "On attend trop pour la racine !" << std::endl;
		}
	}
	else if (j == 0 ){ //cas feuille
		if (wait(NULL) != -1){
			std::cout << "Erreur" << std::endl;
		}
	}
	else if (i >= N){		// Cas du noeud qui est au bout du peigne
		for (int w = 0 ; w < (i -1) ; w++){		//On a i fils (dernier étage qui fait le peigne)
			if (wait(NULL) == -1){
				std::cout << "On attend trop pour le bout du peigne !" << std::endl;
			}
		}
	}
	else{
		for (int w = 0 ; w < i ; w++){		//On a i + 1 fils
			if (wait(NULL) == -1){
				std::cout << "On attend trop au milieu !" << std::endl;
			}
		}
	}

	if (wait(NULL)== -1){
		std::cout << "On a fait le bon nombre de waits" << std::endl;
	}
	
	return 0;
}


	