#include "headers.h"

void set(int argc, char **argv)
{
    if(argc == 2)
        setenv(argv[1], "", 1);
    else if (argc == 3)
        setenv(argv[1],argv[2], 1);
    else
        perror("Usage: setenv variable [value]");
}

void unset(int argc, char **argv)
{
    if(argc == 2) 
        unsetenv(argv[1]);
    else
        perror("Usage: unsetenv variable");
}