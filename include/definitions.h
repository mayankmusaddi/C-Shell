//File containing defintions for all the functions

#ifndef DEFINITIONS
#define DEFINTIONS

char *PATH_HOME;
char *USERNAME;
char *SYSNAME;

void pwd();
void cd(int argc,char **argv);
void echo(int argc,char **argv);
void ls(int argc, char **argv);
void run(int argc, char **argv);
void pinfo(int argc, char **argv);
char *getRelativePath(char *cwd);

#endif