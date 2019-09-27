//File containing defintions for all the functions
#define MAX_ARG_STRLEN (getpagesize() * 32)
#define ARG_MAX 20
// #define ARG_MAX sysconf(_SC_ARG_MAX)
#define MAX_PROC 1000


#ifndef DEFINITIONS
#define DEFINTIONS

char *PATH_HOME;
char *USERNAME;
char *SYSNAME;

int processes[MAX_PROC];
char *processNames[MAX_PROC];
int pno;

char *getRelativePath(char *cwd);
void store(char *str);

char *filterRedirection(char *command);
int redirection(char *command, char *fin[]);

int tokenize(char *arr[], char *str, char *ch);
void execute(int len,char **args,int flag_bg);

void pwd();
void cd(int argc,char **argv);
void echo(int argc,char **argv);
void ls(int argc, char **argv);
void run(int argc, char **argv, int flag_bg);
void pinfo(int argc, char **argv);
void history(int argc,char **argv);
void set(int argc, char **argv);
void unset(int argc, char **argv);
void jobs(int argc, char **argv);
void kjob(int argc, char **argv);
void fg(int argc, char **argv);
void bg(int argc, char **argv);
void overkill(int argc, char **argv);
#endif