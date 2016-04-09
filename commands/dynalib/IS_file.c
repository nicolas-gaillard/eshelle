/*** Author : Quentin Levavasseur ***/

#include "IS_file.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

// Declaration of a "global" stat struct
struct stat sts;  

/*
	Function name : 
		isFolder 
	Argument : 
		char *testedFolder
	Description : 
		returns 1 if the argument is a directory or 0
*/
int isFolder(char* testedFolder){
	if(testedFolder!=NULL && stat(testedFolder,&sts)==0 && S_ISDIR(sts.st_mode)) return 1;
	else return 0;
}

/*
	Function name : 
		isRegularFile 
	Argument : 
		char *testedFile
	Description : 
		returns 1 if the argument is a regular file or 0
*/
int isRegularFile(char* testedFile){
	if(testedFile!=NULL && stat(testedFile,&sts)==0 && S_ISREG(sts.st_mode)) return 1;
	else return 0;
}