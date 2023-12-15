#include "Stack.h"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <sys/mman.h>
#include <fcntl.h>

using namespace std;
using namespace pr;



void handler(int sig){
    cout << "Je suis main créateur de la stack et je m'arrête proprement" << endl;
}


int main () {

    shm_unlink("/myshm");

	size_t len = sizeof(Stack<char>);


    int fd = shm_open("/myshm", O_CREAT|O_EXCL|O_RDWR,0666);

    ftruncate(fd,len);

	void * addr = mmap(nullptr,len, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    if (addr == MAP_FAILED){
        perror("mmap failed");
    }

    
	Stack<char> * s = new  (addr) Stack<char>();


    /*Gestion du signal*/

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = &handler;
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);

    sigsuspend(&sa.sa_mask);

    
    shm_unlink("/myshm");
    munmap(addr,len);    
	return 0;
}