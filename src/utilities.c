#include "headers.h"

void init()
{
	char *username = getlogin();
	if(username==NULL)
		perror("Couldn't get user name");

	char sysnam[256];
	if(gethostname(sysnam,sizeof(sysnam))!=0)
		perror("Couldn't get system name");

	char homepath[PATH_MAX];
	if (getcwd(homepath, sizeof(homepath))== NULL)
		perror("Couldn't get home path");

	PATH_HOME = homepath;
	USERNAME = username;
	SYSNAME = sysnam;
}

void shellPrompt()
{
	char *path;
	char *cwd = malloc(PATH_MAX);
	if (getcwd(cwd, PATH_MAX) != NULL)
		path  = getRelativePath(cwd);
	else
		perror("getcwd() error");
	printf("%s@%s:%s>",USERNAME,SYSNAME,path);
}

char *getRelativePath(char *cwd)
{
	if(strlen(cwd)<strlen(PATH_HOME))
		return cwd;
	else
	{
		char *sub = malloc(PATH_MAX);
		char *new = malloc(PATH_MAX);
		memcpy(sub,&cwd[0],strlen(PATH_HOME));
		if(strcmp(sub,PATH_HOME)!=0)
			return cwd;
		else
		{
			sprintf(new,"~%s",&cwd[strlen(PATH_HOME)]);
			return new;
		}
	}
}

int tokenize(char *arr[], char *str, char *ch)
{
	char *copy = malloc(strlen(str) + 1),*token;
	strcpy(copy, str);
	int len = 0;
	while ((token = strtok_r(copy, ch, &copy)))
		arr[len++] = token;
	return len;
}

void wait_handler()
{
    int status;
    for (int i=1; i<=pno; i++)
    {
		int pid = processes[i];
        if ( waitpid(pid, &status, WNOHANG) && pid != -1 )
        {
			printf("%s with pid %d exited normally\n",processNames[i],pid);
            processes[i] = -1;
        }
    }
    return;
}

void stopSignal(int sig)
{
    stopFlag = 1;
    return;
}

void endSignal(int sig)
{
    endFlag = 1;
    return;
}

int checkUP(char *statement)
{
	int ups=0;
	for(int i=0;i<strlen(statement);i++)
	{
		if( (i+1)%3==0 && statement[i]=='A')
			ups++;
	}
	return ups;
}