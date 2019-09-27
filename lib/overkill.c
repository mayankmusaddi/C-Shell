#include "headers.h"

void overkill(int argc, char **argv)
{
    for(int i=1; i<=pno; i++)
    {
        if(processes[i] != -1)
        {
            kill(processes[i], 9);
            processes[i] = -1;
        }
    }
}