#include "headers.h"

void execute(int argc, char **argv)
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
        wait(NULL);
        // printf("\nWaiting OVER\n");
    }
}
void run(int argc, char **argv)
{
    int stdinC = 0,stdoutC = 1;
    for(int i=0;i<argc;i++)
    {
        if(argv[i]=='<')
        {
            if( i+1 == argc)
            {
                printf("syntax error near unexpected token `newline'\n");
                return 0;
            }
            else
            {
                int fd = open(argv[i+1], O_RDONLY);
                if(fd < 0)
                {
                    printf("%s: No such file or directory\n", argv[i+1]);
                    return 0;
                }
                else
                {
                    if(dup2(fd, stdinC) != 1)  // 1 refers to stdout
                        perror("dup2 fail\n");

                    
                    close(fd);
                }
                
            }
        }
    }
}