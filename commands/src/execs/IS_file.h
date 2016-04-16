#ifndef _ISFILE_H
#define _ISFILE_H 1

struct stat sts;  

/*
	Function name : 
		isFolder 
	Argument : 
		char *testedFolder
	Description : 
		returns 1 if the argument is a directory or 0
*/
extern int isFolder(char* __testedFolder);

/*
	Function name : 
		isRegularFile 
	Argument : 
		char *testedFile
	Description : 
		returns 1 if the argument is a regular file or 0
*/
extern int isRegularFile(char* __testedFile);

#endif