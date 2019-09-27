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

void execute(int len,char **args,int flag_bg)
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
	else if(!strcmp(args[0],"setenv"))
		set(len,args);
	else if(!strcmp(args[0],"unsetenv"))
		unset(len,args);
	else if(!strcmp(args[0],"jobs"))
		jobs(len,args);
	else if(!strcmp(args[0],"kjob"))
		kjob(len,args);
	else if(!strcmp(args[0],"fg"))
		fg(len,args);
	else if(!strcmp(args[0],"bg"))
		bg(len,args);
	else if(!strcmp(args[0],"overkill"))
		overkill(len,args);
	else if(!strcmp(args[0],"exit") || !strcmp(args[0],"quit"))
		kill(-getpid(), SIGINT);
	else
		run(len,args,flag_bg);
}

void main(int argc, char **argv)
{
	init();
	pno = 0;
	processes[pno] = -1;

	while(1)
	{
		signal(SIGCHLD, wait_handler);
		shellPrompt();

		char statements[MAX_ARG_STRLEN];
		fgets(statements, MAX_ARG_STRLEN, stdin);
		if (statements[strlen(statements)-1] == '\n') statements[strlen(statements)-1] = '\0'; // removes last '\n' character from fgets

		store(statements);

		char *statement[ARG_MAX] = {NULL};
		int colons = tokenize(statement,statements,";");
		for(int i=0;i<colons;i++)
		{
			char *commands[ARG_MAX] = {NULL};
			int backgs = tokenize(commands,statement[i],"&");
			for(int j=0;j<backgs;j++)
			{
				// printf("NO1 : %d\n",i);
				int flag_bg = 0;
				if(j<backgs-1 || (statement[i][strlen(statement[i])-1] == '&') )
					flag_bg = 1;

				char *command[ARG_MAX] = {NULL};
				int pipes = tokenize(command,commands[j],"|");

				int act_std_in = dup(0), act_std_out = dup(1);
				int rd = 0, wrt = 1;

				for(int k=0;k<pipes;k++)
				{
					// printf("NO2 : %d\n",i);

					if(rd!=0)
					{
						if(dup2(rd, 0)!=0) // 0 refers to stdin
							perror("dup2 fail");	
						close(rd);
					}
	
					int pipefd[2];
					if(pipe(pipefd)!=0)
						perror("pipe failed");
	
					wrt = pipefd[1];rd = pipefd[0];
	
					if(dup2(wrt, 1) != 1)  // 1 refers to stdout
      					perror("dup2 fail");
					close(wrt);
	
					if(k==pipes-1) dup2(act_std_out, 1);

					//piping ends

					// printf("NO2.5 : %d\n",i);

					char **args = (char **)malloc(ARG_MAX * sizeof(char *)); 
    				for (int l=0; l<ARG_MAX; l++)
					{
         				args[l] = (char *)malloc(MAX_ARG_STRLEN * sizeof(char)); 
						args[l] = '\0';
					}

					int len = redirection(command[k],args);
					if(len==-1)
						break;

					// printf("len %d\n",len);
					// for(int p=0;p<len;p++)
						// printf("%s ",args[p]);
					// printf("command \n");
					// printf("NO4 : %d\n",i);
					execute(len,args,flag_bg);
				}

				dup2(act_std_in, 0);
            	dup2(act_std_out,1);
			}
		}
	}
}