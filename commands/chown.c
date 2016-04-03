/** Author : Jordan BLOUIN **/

#include "IS_file.c"
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
*/
int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        printf("chown : too few arguments\n");
        
    }
    else if(argc > 3)
    {
        printf("chown : too many arguments\n");
    }
    else
    {
        //printf("%s\n%s\n%s\n", argv[0], argv[1], argv[2]); //Use to check arguments values
        FILE *pass;         /* File used to read /etc/passwd and /etc/group */
        char ligne[100];    /* Variable used to store one line after another from /etc/passwd and /etc/group */
        char *ret;          /* Buffer variable used for char* manipulations */
        int i;              /* Iteration variable */
        char *user;         /* User part from argv[1] when user and group are indicated in argv[1] */
        char *groupe;       /* group part from argv[1] when user and group are indicated in argv[1] */
        uid_t usr;          /* New user ID */
        gid_t group;        /* New group ID */
        if(isRegularFile(argv[2]))
        {
            if(strchr(argv[1], ':') == NULL)
            {
                /* Case when no group ID is specified (no ':' char found in argv[1])*/
                if((pass = fopen("/etc/passwd", "r")) != NULL)
                {
                    /* We're searching for the specified user in /etc/passwd to get its ID */
                    while((fgets(ligne,100,pass)) != NULL)
                    {
                        ret = strtok(ligne, ":");
                        for(i=0;i<2;i++)
                        {
                            /* If a user in a line has the same name as the user argument, 
                            * we go to the third field of the line to get the user ID */
                            if(strcmp(ret,argv[1]) == 0)
                            {
                                ret = strtok(NULL, ":");
                                ret = strtok(NULL, ":");
                                usr = atoi(ret);
                                break;
                            }
                        }
                    }
                    fclose(pass);
                    /* We're calling the function chown with an unchanged group ID */
                    chown(argv[2],usr,(gid_t)-1);
                }
                else
                {
                    printf("error opening /etc/passwd\n");
                    return -1;
                }
            }
            else if(strchr(argv[1], ':') != NULL)
            {
                 /* Case when group ID and user ID are specified (':' char found in argv[1])*/
                 ret = strtok(argv[1], ":");
                 user = ret;
                 ret = strtok(NULL, ":");
                 groupe = ret;
                 printf("%s\n%s\n", user, groupe);
                /* Looking for user ID */
                if((pass = fopen("/etc/passwd", "r")) != NULL)
                {
                    /* We're searching for the specified user in /etc/passwd to get its ID */
                    while((fgets(ligne,100,pass)) != NULL)
                    {
                        ret = strtok(ligne, ":");
                        for(i=0;i<2;i++)
                        {
                            /* If a user in a line has the same name as the user argument, 
                            * we go to the third field of the line to get the user ID */
                            if(strcmp(ret,user) == 0)
                            {
                                ret = strtok(NULL, ":");
                                ret = strtok(NULL, ":");
                                usr = atoi(ret);
                                break;
                            }
                        }
                    }
                }
                else
                {
                    printf("error opening /etc/passwd\n");
                    return -1;
                }
                fclose(pass);
                /* Looking for group ID */
                if((pass = fopen("/etc/group", "r")) != NULL)
                {
                    /* We're searching for the specified user in /etc/passwd to get its ID */
                    while((fgets(ligne,100,pass)) != NULL)
                    {
                        ret = strtok(ligne, ":");
                        for(i=0;i<2;i++)
                        {
                            /* If a user in a line has the same name as the user argument, 
                            * we go to the third field of the line to get the user ID */
                            if(strcmp(ret,groupe) == 0)
                            {
                                ret = strtok(NULL, ":");
                                ret = strtok(NULL, ":");
                                group = atoi(ret);
                                break;
                            }
                        }
                    }
                }
                else
                {
                    printf("error opening /etc/group\n");
                    return -1;
                }
                fclose(pass);
                chown(argv[2], usr, group);
            }
            else if(argv[1][0] == ':')
            {
                /* Case when only the group ID is specified (':' char found as first char in argv[1])*/
                ret = strtok(argv[1], ":");
                groupe = ret;
                if((pass = fopen("/etc/group", "r")) != NULL)
                {
                    /* We're searching for the specified user in /etc/group to get its ID */
                    while((fgets(ligne,100,pass)) != NULL)
                    {
                        ret = strtok(ligne, ":");
                        for(i=0;i<2;i++)
                        {
                            /* If a group in a line has the same name as the group argument, 
                            * we go to the third field of the line to get the group ID */
                            if(strcmp(ret,groupe) == 0)
                            {
                                ret = strtok(NULL, ":");
                                ret = strtok(NULL, ":");
                                group = atoi(ret);
                                break;
                            }
                        }
                    }
                }
                else
                {
                    printf("error opening /etc/group\n");
                    return -1;
                }
                fclose(pass);
            }
        }
    }
    return 0;
}