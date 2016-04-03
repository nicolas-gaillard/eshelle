/** Author : Jordan BLOUIN **/

#include "IS_file.c"
#include <unistd.h>
#include <stdio.h>
#include <string.h>

/*  Function :
*       main
*   Arguments :
*       int argc        -> number of arguments
*       char *argv[]    -> arguments
*   Description :
*       Move a file into the indicated folder
*       It's the same logic as cp.c but we remove the file from the source folder after the copy is done
*/
int main(int argc, char *argv[])
{
    /*
    * Displays an error message if there are too few/many arguments
    */
    if(argc<2){
        printf("mv : too few argument\n");
        return -1;
    }
    if(argc==2){
        printf("mv : missing destination file after '%s'\n",argv[1]);
        return -1;
    }
    else if(argc>3){
        printf("mv : too many arguments\n");
        return -1;
    }
    else
    {
        if(isRegularFile(argv[1]))
        {
            FILE *f_Src;
            FILE *f_Dest;
            char *path_Dest = argv[2], *path_Src = argv[1];
            char tmp[4096];
            int i;
            
            /* Setting the path to create a new file into the designated folder */
            if(argv[1][0] != '.')
            {
                strcat(path_Dest, "/");
                strcat(path_Dest, argv[1]);
                printf("Dest path : %s\n", path_Dest);
            }
            
            if((f_Src = fopen(path_Src, "r")) == NULL)
            {
                /* If the file to move can't be opened */
                printf("Source file not found\n");
                return 1;
            }
            if((f_Dest = fopen(path_Dest, "w+")) == NULL)
            {
                /* If the destination folder doesn't exist */
                printf("Destination folder not found\n");
                fclose(f_Src);
                return 2;
            }
            while((i = fread(tmp, 1, 4096, f_Src)) != 0)
            {
                fwrite(tmp, 1, i, f_Dest);
            }
            fclose(f_Dest);
            fclose(f_Src);
            remove(argv[1]);
        }
        else if(isFolder(argv[1]))
        {
            /* Folder part */
            char command[100];
            
            /* Lazy method */
            strcpy(command, "mv ");
            strcat(command, argv[1]);
            strcat(command ," ");
            strcat(command, argv[2]);
            system(command);
        }
        else
        {
            printf("mv : file '%s' doesn't not exist\n", argv[1]);
            return -1;
        }
    }
    return 0;
}