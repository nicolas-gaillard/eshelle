/** Author : Jordan BLOUIN **/

#include "../../lib/IS_file.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*  Function :
*       main
*   Arguments :
*       int argc        -> number of arguments
*       char *argv[]    -> arguments
*   Description :
*       Change a file's owner and/or group
*   How to launch it :
*       sudo ./chgrp GROUP FILE
*/
int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        printf("chgrp : too few arguments\n");
        
    }
    else if(argc > 3)
    {
        printf("chgrp : too many arguments\n");
    }
    else
    {
        //printf("%s\n%s\n%s\n", argv[0], argv[1], argv[2]); //Use to check arguments values
        FILE *pass;         /* File used to read /etc/group */
        char ligne[100];    /* Variable used to store one line after another from /etc/group */
        char *ret;          /* Buffer variable used for char* manipulations */
        int i;              /* Iteration variable */
        gid_t group;        /* New group ID */
        if(isRegularFile(argv[2]))
        {
            if((pass = fopen("/etc/group", "r")) != NULL)
            {
                /* We're searching for the specified group in /etc/group to get its ID */
                while((fgets(ligne,100,pass)) != NULL)
                {
                    ret = strtok(ligne, ":");
                    for(i=0;i<2;i++)
                    {
                        /* If a group in a line has the same name as the group argument, 
                        * we go to the third field of the line to get the group ID */
                        if(strcmp(ret, argv[1]) == 0)
                        {
                            ret = strtok(NULL, ":");
                            ret = strtok(NULL, ":");
                            group = atoi(ret);
                            break;
                        }
                    }
                }
                chown(argv[2], (uid_t)-1, group);
            }
            else
            {
                printf("error opening /etc/group\n");
                return -1;
            }
            fclose(pass);
        }
    }
    return 0;
}