/*** Author : Quentin Levavasseur ***/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

/*
	Function name : 
		isMode 
	Argument : 
		char *testedMode
	Description : 
		returns 1 if the argument is a "mode" (000 to 777) or 0
*/
int isMode(char *testedMode){
    // Declaration of variables
    int i=0;		// cpt
    int isMode=1;	// boolean

    // While the characters are numbers between 0 and 7, testedMode is considered as a "mode"
    while(isMode==1 && i<strlen(testedMode)){
		if(testedMode[i]<'0' || testedMode[i]>'7') isMode=0;
		i++;
    }
    
    return isMode;
}

/*
	Function name : 
		local_mkdir 
	Arguments : 
		int argc (number of passed arguments)
		char *argv[] (arguments)
	Description : 
		creates a directory with rights if option "-m" is required
*/
int local_mkdir(int argc,char *argv[]){
	// An error is displayed if there is too few arguments
	if(argc<2){
		printf("mkdir : too few arguments\n");
		return(-1);
	}
	// Creation of the directory
	else{
		// Declaration of variables
		int mode=0775; 			// final mode
		char completeMode[]="0";	// used to transform rights in the right form (777 --> 0777)
		int mflag=0;			// used to know if option "-m" is required or not

		// We check if option "-m" is required
		int c;
		opterr=0;
		while ((c = getopt (argc, argv, "m")) != -1){
			switch (c){
				case 'm':
					mflag=1;
					break;
				case '?' :
					printf("mkdir : option %s doesn't exist for command mkdir\n",argv[optind-1]);
					return(-1);
			}
		}
		// For each element we check if it's the mode (if "-m") or the directory's name
		int index;
		for (index = optind; index < argc; index++){
			if(mflag==1){
				if(isMode(argv[index])){
					// If the mode is correct we transform it into the right form
					if(strlen(argv[index])==3){
						int i=0;
						for(i=0;i<3;i++) completeMode[i+1]=argv[index][i];
						mode = strtol(completeMode, 0, 8);
						// There is no mode to found anymore
						mflag=0;
					}
					else{
						printf("mkdir : mode %s is incorrect\n",argv[index]);
						return(-1);
					}
				}
				else{
					// An error is displayed if there is no mode argument with option "-m"
					if(mflag==1){
						printf("mkdir : there is no mode argument\n");
						return(-1);
					}
					// We make the directory or an error will be displayed if it already exists
					if(mkdir(argv[index],mode)!=0) printf("mkdir : repertory %s already exists\n",argv[index]);
				}
			}
			else{
				// We make the directory or an error will be displayed if it already exists
				if(mkdir(argv[index],mode)!=0) printf("mkdir : repertory %s already exists\n",argv[index]);
			}
		}
	}

	return 0;
}