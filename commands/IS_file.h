/*** Author : Quentin Levavasseur ***/

#ifndef _IS_FILE_H
	#define _IS_FILE_H

/*
	Function name : 
		isFolder 
	Argument : 
		char *testedFolder
	Description : 
		returns 1 if the argument is a directory or 0
*/
int isFolder(char* testedFolder);

/*
	Function name : 
		isRegularFile 
	Argument : 
		char *testedFile
	Description : 
		returns 1 if the argument is a regular file or 0
*/
int isRegularFile(char* testedFile);

#endif