/*** Author : Quentin Levavasseur ***/

#include <unistd.h>

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
	// Declaration of a tab wich will contain the path
	char path[TAILLE];
	getcwd (path, TAILLE-1);
	// We display the current directory
	printf("%s\n",path);

	return 0;
}
