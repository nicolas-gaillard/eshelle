/* Author : Jordan BLOUIN */

#include <stdio.h>
#include <stdlib.h>

/*  Function :
*       main
*    Arguments :
*       int argc        -> number of arguments
*       char *argv[]    -> arguments
*   Description :
*       Print the indicated text
*/
int main(int argc, char *argv[])
{
    int i; /* Iteration variable */
    for(i=1;i<argc;i++)
    {
        /* We print every component from argv */
        printf("%s ", argv[i]);
    }
    printf("\n");
}