#include "headers.h"

void jobs(int argc, char **argv)
{
    for(int i=1;i<=pno;i++)
    {
        if(processes[i]==-1)
            continue;
        
        int pid = processes[i];
        //To get Process Status
        char stat_path[PATH_MAX];
        sprintf(stat_path,"/proc/%d/stat",pid);
        FILE *fp = fopen(stat_path, "r");
        if(fp == NULL)
        {
            printf("There is no process with pid %d\n", pid);
            continue;
        }
        // third word of stat file contains current status of process
        char tmp[1] = {'\0'};
        char status[5];
        fscanf(fp, "%s %s %s", tmp, tmp, status);

        char name_path[PATH_MAX];
        sprintf(name_path,"/proc/%d/cmdline",pid);
        fp = fopen(name_path, "r");
        char name[100];
        fscanf(fp, "%s", name);

        printf("[%d] ",i);
        if(!strcmp(status, "T"))
            printf("Stopped  ");
        else
            printf("Running  ");
        printf("%s[%d]\n", name, pid);
    }
}