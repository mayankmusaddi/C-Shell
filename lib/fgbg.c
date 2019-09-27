#include "headers.h"

void fg(int argc, char **argv)
{
    if(argc !=2)
        perror("Usage: fg <jobnumber>");
    else
    {
        int jobNum = atoi(argv[1]);
        int pid = processes[jobNum];
        if(jobNum <= 0 || jobNum > pno || pid == -1)
            perror("Invalid Job Number");
        else
        {
            processes[jobNum] = -1;
            kill(pid,SIGCONT);
            int status;
            while(waitpid(pid, &status, WNOHANG) != pid);
        }
    }
}

void bg(int argc, char **argv)
{
    if(argc !=2)
        perror("Usage: bg <jobnumber>");
    else
    {
        int jobNum = atoi(argv[1]);
        int pid = processes[jobNum];
        if(jobNum <= 0 || jobNum > pno || pid == -1)
            perror("Invalid Job Number");
        else
            kill(pid,SIGCONT);
    }
}