#include "headers.h"

void kjob(int argc, char **argv)
{
    if(argc !=3)
        perror("Usage: kjobs <jobnumber> <signalNumber>");
    else
    {
        int jobNum = atoi(argv[1]);
        int sigNum = atoi(argv[2]);
        if(jobNum <= 0 || jobNum > pno || processes[jobNum] == -1)
            perror("Invalid Job Number");
        else if(kill(processes[jobNum], sigNum) < 0)
                perror("Invalid Signal Number");
    }
}