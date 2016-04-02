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
    printf("%d\n", argc);
    printf("%s\n%s\n%s\n", argv[0], argv[1], argv[2]);
    /*
    * Displays an error message if there are too few/many arguments
    */
    if(argc < 2)
    {
        printf("mv : missing argument\n");
        return -1;
    }
    else if(argc > 3)
    {
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
                printf("Source file not found\n");
                return 1;
            }
            if((f_Dest = fopen(path_Dest, "w+")) == NULL)
            {
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
        else
        {
            printf("mv : file '%s' doesn't not exist\n", argv[1]);
            return -1;
        }
    }
    return 0;
}