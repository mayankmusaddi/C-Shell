#include "headers.h"

int PTR = 0;
int MAX = 0;
char cmdlist[ARG_MAX][200];

void store(char *str)
{
    strcpy(cmdlist[PTR],str);
    PTR= (PTR+1)%20;
    MAX++;
    if(MAX > 20)
        MAX--;
}

void history(int argc,char **argv)
{
    if(argc > 2)
    {
        printf("bash: history: too many arguments\n");
        return;
    }

    int num = 20;
    if(argc == 2)
        num = atoi(argv[1]) > 20 ? 20 : atoi(argv[1]);
    
    int end = PTR-1;
    int st = end-num+1;

    if(MAX < num)
        st = 0;

    for(int i=st;i<=end;i++)
        printf("%s\n",cmdlist[(i+20)%20]);
}