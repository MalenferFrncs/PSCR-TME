#include "Stack.h"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <sys/mman.h>
#include <fcntl.h>


using namespace std;
using namespace pr;

void consomateur (Stack<char> * stack) {
	while (true) {
		char c = stack->pop();
		cout << c << flush ;
	}
}


void handler(int sig){
    cout << "Je suis le consommateur " << getpid() <<" et je m'arrête proprement" << endl;

    exit(0);
}


int main () {

	/* TODO
	*À mettre dans un segment de mémoire partagé (anonyme)
	qui prend l'adresse 
	* Construire la stak dans la mémoire partagée
	* syntaxe  : 
		new (adresse choisie plutôt qu'un malloc) Constructeur (args)
	*/

	size_t len = sizeof(Stack<char>);



    int fd = shm_open("/myshm",O_RDWR,0666);


	void * addr = mmap(nullptr,len, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (addr == MAP_FAILED){
        perror("mmap failed");
    }



	Stack<char> * s = (Stack<char> *) addr;

    /*Gestion du signal*/

    struct sigaction sa;
    sigfillset(&sa.sa_mask);
    sa.sa_handler = &handler;
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);


	consomateur(s);


	munmap(addr,len);
	return 0;
}