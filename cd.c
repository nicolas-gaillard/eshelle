/*** Author : Quentin Levavasseur ***/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "IS_file.h"

/*
	Function name : 
		cd 
	Arguments : 
		int argc (number of passed arguments)
		char *argv[] (arguments)
	Description : 
		changes the current repertory
*/
int cd(int argc,char *argv[]){
	
	// An error is displayed if there is too few arguments
	if(argc<2){
		printf("cd : too few arguments\n");
		return(-1);
	}
	// An error is displayed if there are too many arguments
	else if(argc>2){
		printf("cd : too many arguments\n");
		return(-1);
	}
	// Change current repertory
	else{
		// We change the repertory or an error will be displayed if it doesn't exist
		if(isFolder(argv[1])){
			printf("cd : Test : Repertoire courant avant : %s\n",get_current_dir_name());
			chdir(argv[1]);
			printf("cd : Test : Repertoire courant apr�s : %s\n",get_current_dir_name());
		}
		else{
			printf("cd : directory %s doesn't exist\n",argv[1]);
			return(-1);
		}
	}

    return 0;
}
