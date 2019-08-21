#include "headers.h"

void fileDetails(char *path, char *filename)
{
    struct stat stats;
    if(stat(path,&stats) < 0)    
        return;

    // printf("File Permissions: \t");
    printf( (S_ISDIR(stats.st_mode)) ? "d" : "-");
    printf( (stats.st_mode & S_IRUSR) ? "r" : "-");
    printf( (stats.st_mode & S_IWUSR) ? "w" : "-");
    printf( (stats.st_mode & S_IXUSR) ? "x" : "-");
    printf( (stats.st_mode & S_IRGRP) ? "r" : "-");
    printf( (stats.st_mode & S_IWGRP) ? "w" : "-");
    printf( (stats.st_mode & S_IXGRP) ? "x" : "-");
    printf( (stats.st_mode & S_IROTH) ? "r" : "-");
    printf( (stats.st_mode & S_IWOTH) ? "w" : "-");
    printf( (stats.st_mode & S_IXOTH) ? "x" : "-");

    //No of Links
    printf(" %ld",stats.st_nlink);

    //File Owner, Owner Group
    struct passwd *pw = getpwuid(stats.st_uid);
    struct group  *gr = getgrgid(stats.st_gid);
    printf(" %s %s", pw->pw_name, gr->gr_name);

    //File Size
    printf("%8ld",stats.st_size);

    //Last Modified
    char date[20];
    strftime(date, 20, "%b %d %H:%M", localtime(&(stats.st_ctime)));
    printf(" %s", date);

    printf(" %s\n",filename);
}
void ls(int argc, char **argv)
{
    DIR *mydir;
    struct dirent *myfile;
    struct stat mystat;

    int flag_a = 0,flag_l = 0;
    char *dirs[argc];
    int n_dir = 0;
    for(int i=1;i<argc;i++)
    {
        if(argv[i][0]=='-')
        {
            for(int j=0;j<strlen(argv[i]);j++)
            {
                if(argv[i][j]=='l')
                    flag_l = 1;
                if(argv[i][j]=='a')
                    flag_a = 1;
            }
        }
        else if(strcmp(argv[i],"~")==0)
            dirs[n_dir++]=PATH_HOME;
        else
            dirs[n_dir++]=argv[i];
    }

    for(int i=0;i<=n_dir;i++)
    {
        if(i==n_dir && i!=0)
            break;

        if(n_dir>1)
            printf("%s:\n",dirs[i]);
        
        if(n_dir==0)
            dirs[0]=".";

        if((mydir = opendir(dirs[i]))==NULL)
        {
            perror("ls");
            continue;
        }

        while((myfile = readdir(mydir)) != NULL)
        {
            if(!flag_a && (myfile->d_name)[0] == '.')
                continue;
            if(flag_l)
            {
                char path[512];
    	        sprintf(path, "%s/%s", dirs[i], myfile->d_name);
                fileDetails(path,myfile->d_name);
            }
            else
                printf("%s ",myfile->d_name);
        }
        if(!flag_l)
            printf("\n");

        closedir(mydir);
    }
}