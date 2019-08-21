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

void main(int argc, char **argv)
{
	init();

	while(1)
	{
		shellPrompt();

		char commands[MAX_ARG_STRLEN];
		fgets(commands, MAX_ARG_STRLEN, stdin);
		if (commands[strlen(commands)-1] == '\n') commands[strlen(commands)-1] = '\0'; // removes last '\n' character from fgets

		store(commands);

		char *com = commands;
		char* command;

		while ((command = strtok_r(com, ";", &com)))
		{
			char *args[ARG_MAX] = {NULL};
			char *copy = command;
			char* token;
			int flag_bg = 0;

			int len = 0;
			while ((token = strtok_r(copy, " ", &copy)))
			{
				if(token[0]=='&')
				{
					flag_bg = 1;
					break;
				}
				args[len++] = token;
			}
			if(len==0)
				continue;

			int pid=0;

			if(flag_bg)
				pid = fork();

			if(pid==0)
			{
				if(!strcmp(args[0],"cd"))
					cd(len,args);
				else if(!strcmp(args[0],"echo"))
					echo(len,args);
				else if(!strcmp(args[0],"pwd"))
					pwd();
				else if(!strcmp(args[0],"ls"))
					ls(len,args);
				else if(!strcmp(args[0],"pinfo"))
					pinfo(len,args);
				else if(!strcmp(args[0],"history"))
					history(len,args);
				else if(!strcmp(args[0],"exit") || !strcmp(args[0],"quit"))
					kill(-getpid(), SIGINT);
				else
					run(len,args);

				if(flag_bg)
				{
					printf("%s with pid %d exited normally\n",args[0],getpid());
					kill(-getpid(), SIGINT);
				}
			}
		}
	}
}