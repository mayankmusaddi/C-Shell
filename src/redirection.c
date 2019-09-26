#include "headers.h"

char *filterRedirection(char *command) //inserts spaces before and after redirection
{
	char* s = malloc(MAX_ARG_STRLEN);
	int len = strlen(command);
	int k = 0;
	for(int i=0;i<len;i++)
	{
		if(command[i]=='<')
		{
			s[k++] = ' ';
			s[k++] = '<';
			s[k++] = ' ';
		}
		else if(command[i]=='>')
		{
			s[k++] = ' ';
			while(command[i]=='>')
			{
				s[k++] = '>';
				i++;
			}
			s[k++] = ' ';
		}
		else
			s[k++] = command[i];
	}
	return s;
}

int redirection(char *command, char *fin[])
{
	//takes in an unpiped command then returns last input, output, appended output
	char *cmd = filterRedirection(command);
	char *args[ARG_MAX] = {NULL};
	int words = tokenize(args,cmd," ");
	free(cmd);

	int k =0;

	int failed = 0;
	for(int i=0;i<words;i++)
	{
		if(!strcmp(args[i],"<"))
		{
			if(i == words-1)
			{
				perror("syntax error: no redirection input mentioned");
				failed = 1;
				break;
			}
			int fd = open(args[i+1],O_RDONLY);
			if(fd < 0)
			{
				perror("File Not Found");
				failed = 1;
				break;
			}
			if(dup2(fd, 0)!=0) // 0 refers to stdin
				perror("dup2 fail");	
			close(fd);
			i+=1;
		}
		else if(!strcmp(args[i],">"))
		{
			if(i == words-1)
			{
				perror("syntax error: no redirection output mentioned");
				failed = 1;
				break;
			}
			int fd = open(args[i+1], O_WRONLY | O_TRUNC | O_CREAT ,0644);
			if(fd < 0)
			{
				perror("File Not Found");
				failed = 1;
				break;
			}
			if(dup2(fd, 1)!=1) // 0 refers to stdin
				perror("dup2 fail");	
			close(fd);
			i+=1;
		}
		else if(!strcmp(args[i],">>"))
		{
			if(i == words-1)
			{
				perror("syntax error: no redirection output mentioned");
				failed = 1;
				break;
			}
			int fd = open(args[i+1], O_WRONLY | O_APPEND | O_CREAT ,0644);
			if(fd < 0)
			{
				perror("File Not Found");
				failed = 1;
				break;
			}
			if(dup2(fd, 1)!=1) // 0 refers to stdin
				perror("dup2 fail");	
			close(fd);
			i+=1;
		}
		else
			fin[k++] = args[i];
	}
	if(failed)
		return -1;
	return k;
}