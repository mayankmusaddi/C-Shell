#include "headers.h"

void pinfo(int argc,char **argv)
{
    int pid;

    if(argc==1)
        pid = getpid();
    else if(argc == 2)
        pid = atoi(argv[1]);
    else
    {
        printf("pinfo: too many arguments\n");
        return;
    }
    
    FILE *fp;

    //To get Process Status
    char stat_path[PATH_MAX];
    sprintf(stat_path,"/proc/%d/stat",pid);
    fp = fopen(stat_path, "r");
    if(fp == NULL)
    {
        printf("There is no process with pid %d\n", pid);
        return;
    }
    // third word of stat file contains current status of process
    char tmp[1] = {'\0'};
    char status[5];
    fscanf(fp, "%s %s %s", tmp, tmp, status);

    //To get Memory Consumed
    char statm_path[PATH_MAX];
    sprintf(statm_path,"/proc/%d/statm",pid);
    fp = fopen(statm_path, "r");
    if(fp == NULL)
    {
        printf("There is no process with pid %d\n", pid);
        return;
    }
    // first word of statm file contains virtual memory used
    char memory[15];
    fscanf(fp, "%s", memory);

    //To get Executable Path
    char exec_path[PATH_MAX];
    sprintf(exec_path,"/proc/%d/exe",pid);
    // readlink reads the symlink exe which is pointed to executable path of process
    char exepath[PATH_MAX] = {'\0'};
    readlink(exec_path, exepath, sizeof(exepath));
    char * exePath = getRelativePath(exepath);

    printf("pid -- %d\n", pid);
    printf("Process Status -- %s memory\n", status);
    printf("- %s {Virtual Memory}\n", memory);
    printf("- Executable Path - %s\n", exePath);

    return;
}