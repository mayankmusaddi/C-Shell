//File containing defintions for all the functions

#ifndef DEFINITIONS
#define DEFINTIONS

char *PATH_HOME;
char *USERNAME;
char *SYSNAME;

char *getRelativePath(char *cwd);
void store(char *str);

void pwd();
void cd(int argc,char **argv);
void echo(int argc,char **argv);
void ls(int argc, char **argv);
void run(int argc, char **argv);
void pinfo(int argc, char **argv);
void history(int argc,char **argv);

#endif

#define MAX_ARG_STRLEN (getpagesize() * 32)
#define ARG_MAX 20
// #define ARG_MAX sysconf(_SC_ARG_MAX)
