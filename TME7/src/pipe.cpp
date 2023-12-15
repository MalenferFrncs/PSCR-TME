#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>

int main(int argc, char **argv)
{
    
    
    char **argv2;
   
    for (int i = 0; argv[i]; i++)
    { // On parcourt tout argc
        if (!strcmp(argv[i], "|"))
        {
            argv[i] = 0x0;
            argv2 = &argv[i + 1];
            
            break;
        }
    }
    
    int tubeDesc[2];
    int pid_fils1;
    int pid_fils2;

    /*On créé la pipe*/
    if (pipe(tubeDesc) == -1)
    {
        perror("pipe");
        exit(1);
    }

    if ((pid_fils1 = fork()) == -1)
    {
        perror("fork");
        exit(2);
    }

     if (pid_fils1 == 0)
    { // Code fils 1
        dup2(tubeDesc[1], STDOUT_FILENO); // On connecte la sortie
        close(tubeDesc[1]);
        close(tubeDesc[0]);
        if (execv(argv[1], &argv[1]) == -1)
        { // argv[1] est le path, &argv[1] permet de passer les arguments (ATTENTION : argv[1] est le premier argument du binaire cible !!!)
            perror("execv");
            exit(3);
        }
    }

     if ((pid_fils2 = fork()) == -1)
    {
        perror("fork");
        exit(2);
    }
    if (pid_fils2 == 0)
    { // Code fils 2

        dup2(tubeDesc[0], STDIN_FILENO);        //on connecte l'entrée
        close(tubeDesc[0]);
        close(tubeDesc[1]);

        if (execv(argv2[0], argv2) == -1)
        { // argv2[0] est le path trouvé lors du parcours, &argv2[0] permet de passer les arguments (ATTENTION : argv2[0] est le premier argument du binaire cible !!!)
            perror("execv");
            exit(3);
        }
    }
    
    close(tubeDesc[1]);
    close(tubeDesc[0]);

    wait(NULL);
    wait(NULL);

    return EXIT_SUCCESS;
}