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
            setpgid(pid,pid);
            signal(SIGINT, SIG_IGN);
            processNames[++pno] = argv[0];
            processes[pno] = pid;
        }
        else
        {
            stopFlag = 0;
            endFlag = 0;
            signal(SIGTSTP, stopSignal);
            signal(SIGINT, endSignal);

            int status;
            while(!endFlag && !stopFlag && waitpid(pid, &status, WNOHANG)!= pid);

            if(endFlag)
            {
                kill(pid,SIGINT);
                endFlag = 0;
            }
            if(stopFlag)
            {
                setpgid(pid,pid);
                kill(pid,SIGSTOP);
                processNames[++pno] = argv[0];
                processes[pno] = pid;
                stopFlag = 0;
            }
        }
    }
}