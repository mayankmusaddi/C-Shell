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

int tokenize(char *arr[], char *str)
{
	char *cpy = str;
	char *token;
	int len = 0;
	while ((token = strtok_r(cpy, "|", &cpy)))
		arr[len++] = token;
	return len;
}

void main(int argc, char **argv)
{
	init();

	while(1)
	{
		shellPrompt();

		char statement[MAX_ARG_STRLEN];
		fgets(statement, MAX_ARG_STRLEN, stdin);
		if (statement[strlen(statement)-1] == '\n') statement[strlen(statement)-1] = '\0'; // removes last '\n' character from fgets

		store(statement);

		char *statcpy = statement;
		char* commands;

		while ((commands = strtok_r(statcpy, ";", &statcpy)))
		{

			char *command[ARG_MAX] = {NULL};
			char *cmd,*cmdscpy = commands;
			int pipes = 0;
			while ((cmd = strtok_r(cmdscpy, "|", &cmdscpy)))
				command[pipes++] = cmd;

			int act_std_in = dup(0), act_std_out = dup(1);
			int rd = 0, wrt = 1;

			for(int i=0;i<pipes;i++)
			{
				if(rd!=0)
				{
					if(dup2(rd, 0)!=0) // 0 refers to stdin
						perror("dup2 fail");	
					close(rd);
				}

				int pipefd[2];
				if(pipe(pipefd)!=0)
					perror("pipe failed");

				wrt = pipefd[1];
				rd = pipefd[0];

				if(dup2(wrt, 1) != 1)  // 1 refers to stdout
      				perror("dup2 fail");
				close(wrt);

				if(i==pipes-1) dup2(act_std_out, 1);


				int flag_bg = 0;
				char *args[ARG_MAX] = {NULL};
				char *token,*cmdcpy = command[i];;
				int len = 0;
				while ((token = strtok_r(cmdcpy, " ", &cmdcpy)))
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

				int failed = 0;
				for(int j=0;j<len;j++)
				{
					if( (!strcmp(args[j],"<")) || (!strcmp(args[j],">")) || (!strcmp(args[j],">>")))
					{
						if(j == len-1)
						{
							perror("syntax error: no redirection io mentioned");
							failed = 1;
							break;
						}
						else
						{
							int fd;
							if(!strcmp(args[j],"<"))
								fd = open(args[j+1], O_RDONLY);
							else if(!strcmp(args[j],">"))
								fd = open(args[j+1], O_WRONLY | O_TRUNC | O_CREAT ,0644);
							else
								fd = open(args[j+1], O_WRONLY | O_APPEND | O_CREAT ,0644);

                			if(fd < 0)
                			{
                			    printf("Cannot access '%s': No such file or directory\n", args[j+1]);
								failed = 1;
                			    break;
                			}
							else
							{
								if(!strcmp(args[j],"<"))
								{
									if(dup2(fd, 0)!=0) // 0 refers to stdin
										perror("dup2 fail");	
									close(fd);
								}
								else
								{
									if(dup2(fd, 1)!=1) // 0 refers to stdin
										perror("dup2 fail");	
									close(fd);
								}	
							}
							args[j]=args[len-2];args[j+1]=args[len-1];
							args[len-2]=NULL;args[len-1]=NULL;
							len-=2;
						}
						j-=1;
					}
				}
				if(failed)
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
						exit(0);
					}
				}

			} //end of a piped command
			dup2(act_std_in, 0);
            dup2(act_std_out,1);

			// char f_content[102];
			// read(wrt, f_content, 100);
			// printf("%d\n",wrt);
    		// printf("Contents of fildes[0]: %s\n", f_content);
		}
	}
}