#include "headers.h"

void pwd()
{
	char *cwd = malloc(PATH_MAX);
	if (getcwd(cwd, PATH_MAX) != NULL)
		printf("%s\n",cwd);
	else
		perror("getcwd() error");
}

void cd(int argc,char **argv)
{
	if(argc > 2)
		printf("bash: cd: too many arguments\n");

	else if(argc==1)
		chdir(PATH_HOME);

	else if(chdir(argv[1])!=0)
		printf("bash: cd: %s: No such file or directory\n",argv[1]);
}

void echo(int argc,char **argv)
{
	// Can improve on '' and "" presence
	for(int i=1;i<argc;i++)
		printf("%s ",argv[i]);
	printf("\n");
}
