#include "headers.h"

void run(int argc, char **argv,int flag_bg)
{
    int pid = fork();
    if(pid==0)
    {
        execvp(argv[0],argv);
	    printf("Command not found\n");
        exit(0);
    }
    else
    {
        //Parent Process waiting for child to finish
        if(flag_bg)
        {
            processNames[++pno] = argv[0];
            processes[pno] = pid;
        }
        else
            wait(NULL);
        // printf("\nWaiting OVER\n");
    }
}