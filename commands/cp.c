/** Author : Jordan BLOUIN **/

#include "IS_file.c"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>


/*  Function :
*       main
*   Arguments :
*       int argc        -> number of arguments
*       char *argv[]    -> arguments
*   Description :
*       Copy a file (first argument) into the indicated folder (second argument)
*/
int main(int argc, char *argv[])
{
    char *ret; /* Variables used to get the file/folder name to copy */
    /*
    * Displays an error message if there are too few/many arguments
    */
    if(argc < 2)
    {
        printf("cp : missing argument\n");
        return -1;
    }
    else if(argc > 5)
    {
        printf("cd : too many arguments\n");
        return -1;
    }
    else
    {
        int rflag=0, dflag=0;
        char c;
        /* Checking if the -r and -d option are used to copy a directory */
        while((c = getopt(argc, argv, "dir")) != -1)
        {
            if(c == 'r')
            {
                rflag = 1;
            }
            if(c == 'd')
            {
                dflag = 1;
            }
        }
        if(isRegularFile(argv[1]))
        {
            /* File part */
            FILE *f_Src;        /* Source file */
            FILE *f_Dest;       /* Destination file */
            char *path_Dest = argv[2], *path_Src = argv[1];
            char tmp[4096];
            int i;
            
            /* Setting the path to create a new file into the designated folder */
            if(argv[1][0] == '.')
            {
                /* Getting the file name to copy from a relative path*/
                ret = strrchr(argv[1], '/');
                printf("==%s\n", ret);
                ret = strchr(ret, ret[1]);
                printf("===%s\n", ret);
            }
            else if(argv[1][0] == '/')
            {
                /* Getting the file name to copy from an absolute path*/
                ret = strrchr(argv[1], '/');
                printf("==%s\n", ret);
                ret = strchr(ret, ret[1]);
                printf("===%s\n", ret);
            }
            strcat(path_Dest, "/");
            strcat(path_Dest, argv[1]);
            printf("Dest path : %s\n", path_Dest);
            
            /* If the file to copy can't be opened */
            if((f_Src = fopen(path_Src, "r")) == NULL)
            {
                printf("Source file not found\n");
                return 1;
            }
            /* If the destination path doesn't exist */
            if((f_Dest = fopen(path_Dest, "w+")) == NULL)
            {
                printf("Destination folder not found\n");
                fclose(f_Src);
                return 2;
            }
            /* We're writing all source file content into the destination file */
            while((i = fread(tmp, 1, 4096, f_Src)) != 0)
            {
                fwrite(tmp, 1, i, f_Dest);
            }
            fclose(f_Dest);
            fclose(f_Src);
        }
        else if(isFolder(argv[2]) && rflag == 1 && dflag == 1)
        {
            /* Folder part */
            char *destPath = argv[3];
            char command[100];
            if(argv[2][0] == '.')
            {
                /* Getting the folder name to copy from a relative path*/
                ret = strrchr(argv[2], '/');
                printf("==%s\n", ret);
                ret = strchr(ret, ret[1]);
                printf("===%s\n", ret);
            }
            else if(argv[2][0] == '/')
            {
                /* Getting the folder name to copy from an absolute path */
                ret = strrchr(argv[2], '/');
                printf("==%s\n", ret);
                ret = strchr(ret, ret[1]);
                printf("===%s\n", ret);
            }
            /* Setting the destination path from the sourdce folder name */
            strcat(destPath, "/");
            strcat(destPath, ret);
            printf("%s\n", destPath);
            //mkdir(destPath ,0755);
            
            /* Lazy method */
            strcpy(command, "cp -dr ");
            strcat(command, argv[2]);
            strcat(command ," ");
            strcat(command, argv[3]);
            system(command);
            
            return -1;
        }
        else if(isFolder(argv[2]) && dflag == 0 && rflag == 0)
        {
            printf("cp : missing option '-dr' to copy folder '%s'\n", argv[2]);
            return -1;
        }
        else if(isFolder(argv[2]) && rflag == 0)
        {
            printf("cp : missing option '-r' to copy folder '%s'\n", argv[2]);
            return -1;
        }
        else if(isFolder(argv[2]) && dflag == 0)
        {
            printf("cp : missing option '-d' to copy folder '%s'\n", argv[2]);
            return -1;
        }
        else
        {
            printf("cp : file/folder '%s' doesn't not exist\n", argv[1]);
            return -1;
        }
    }
    return 0;
}