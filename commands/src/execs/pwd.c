/*** Author : Quentin Levavasseur ***/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define TAILLE 100

/*
	Function name : 
		main 
	Arguments : 
		int argc (number of passed arguments)
		char *argv[] (arguments)
	Description : 
		displays the current repertory
*/
int main(int argc,char *argv[]){
	// An error is displayed if there are too many arguments
	if(argc>1){
		printf("pwd : too many arguments\n");
		return(-1);
	}
	// Display the current repertory
	else{
		// Declaration of a tab wich will contain the path
		char path[TAILLE];
		getcwd (path, TAILLE-1);
		// We display the current directory
		printf("%s\n",path);
	}

	return 0;
}