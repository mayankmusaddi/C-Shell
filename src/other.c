#include "headers.h"

void run(int argc, char **argv)
{
    int pid = fork();

    if(pid==0)
    {
        execvp(argv[0],argv);
	    printf("Command not found\n");
    }
    else
    {
        //Parent Process waiting for child to finish
        wait(NULL);
        // printf("\nWaiting OVER\n");
    }
}