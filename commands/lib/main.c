/* Author : Jordan BLOUIN */

#include "libcommands.h"
#include <stdio.h>

/**
 * Method :
 *      main
 * Arguments :
 *      int argc -> number of arguments
 *      char *argv[] -> arguments
 * Description :
 *      This function 
 **/
int main(int argc, char *argv[])
{
    char* truc[] = {"echo","salut ça va ?"};
    echo(2, truc);
}