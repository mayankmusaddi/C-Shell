#include "headers.h"

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
	else if(!strcmp(args[0],"cronjob"))
		cronjob(len,args);
	else if(!strcmp(args[0],"exit") || !strcmp(args[0],"quit"))
	{
		overkill(len,args);
		exit(0);
	}
	else
		run(len,args,flag_bg);
}

void executeStat(char * statements)
{
	char *statement[ARG_MAX] = {NULL};
	int colons = tokenize(statement,statements,";");
	for(int i=0;i<colons;i++)
	{
		char *commands[ARG_MAX] = {NULL};
		int backgs = tokenize(commands,statement[i],"&");
		for(int j=0;j<backgs;j++)
		{
			int flag_bg = 0;
			if(j<backgs-1 || (statement[i][strlen(statement[i])-1] == '&') )
				flag_bg = 1;

			char *command[ARG_MAX] = {NULL};
			int pipes = tokenize(command,commands[j],"|");

			int act_std_in = dup(0), act_std_out = dup(1);
			int rd = 0, wrt = 1;

			for(int k=0;k<pipes;k++)
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
	
				wrt = pipefd[1];rd = pipefd[0];
	
				if(dup2(wrt, 1) != 1)  // 1 refers to stdout
      				perror("dup2 fail");
				close(wrt);
	
				if(k==pipes-1) dup2(act_std_out, 1);
				//piping ends

				char **args = (char **)malloc(ARG_MAX * sizeof(char *)); 
    			for (int l=0; l<ARG_MAX; l++)
				{
         			args[l] = (char *)malloc(MAX_ARG_STRLEN * sizeof(char)); 
					args[l] = '\0';
				}

				int len = redirection(command[k],args);
				if(len==-1)
					break;
				execute(len,args,flag_bg);
			}
			dup2(act_std_in, 0);
        	dup2(act_std_out,1);
		}
	}
}

void main(int argc, char **argv)
{
	init();
	pno = 0;
	processes[pno] = -1;
	char *cmd;
	upFlag = 0;

	while(1)
	{
		signal(SIGCHLD, wait_handler);
		signal(SIGINT, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);

		shellPrompt();
		char statements[MAX_ARG_STRLEN];

		if(upFlag)
		{
			printf("%s",cmd);
			strcpy(statements,cmd);
			upFlag = 0;
			char stat2[MAX_ARG_STRLEN];
			fgets(stat2, MAX_ARG_STRLEN, stdin);
			if (stat2[strlen(stat2)-1] == '\n') stat2[strlen(stat2)-1] = '\0'; // removes last '\n' character from fgets
			strcat(statements, stat2);
		}
		else
		{
			fgets(statements, MAX_ARG_STRLEN, stdin);
			if (statements[strlen(statements)-1] == '\n') statements[strlen(statements)-1] = '\0'; // removes last '\n' character from fgets
		}

		int ups = checkUP(statements);
		if(ups > 0)
		{
			upFlag = 1;
			cmd = commandAt(ups);
			continue;
		}
		else
			store(statements);

		executeStat(statements);
	}
}