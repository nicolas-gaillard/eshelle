/*** Author : Quentin Levavasseur ***/

#include "../../lib/IS_file.h"
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

/*
	Function name : 
		recursiveRmDir
	Arguments : 
		char *nameDir (name of the directory we want to remove)
	Description : 
		removes recursively directory (if "-r")
*/
int recursiveRmDir(char *nameDir){
	// Declaration of variables	
	DIR *directory;           	// the directory we want to remove
	struct dirent *entry;     	// used to know the information of the directory
	struct stat file_stat;    	// used to know the information of the content file
	char buffer[1024] = {0};	// a tab which contains the path of a file containing in the directory

	// Directory opening
	if ((directory=opendir(nameDir))==NULL){
		printf("rm : cannot open the directory %s\n",nameDir);
		return -1;
	}

	// For each content file of the directory we remove it
	while ((entry=readdir(directory))!=NULL) {
		// We ignore the "." and ".." directories
		if (strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..")==0) continue;
		// We make the path of the content file
		snprintf(buffer, 1024, "%s/%s", nameDir, entry->d_name);
		// We collect the information of the content file
		stat(buffer, &file_stat);
		// If the content file is a regular file we unlink it
		if (S_ISREG(file_stat.st_mode)) unlink(buffer);
		// Else it's a directory and we throw the recursive function on it
		else if ( S_ISDIR(file_stat.st_mode) ) recursiveRmDir(buffer);
	}

	// Directory closure
	closedir(directory);

	// The directory is now empty, we can remove it normaly
	if(rmdir(nameDir)!=0){
		errno=rmdir(nameDir);
		if(errno==ENOTEMPTY) printf("rm : directory %s is not empty\n",nameDir);
		else if(errno==EACCES) printf("rm : you don't have rights on the directory %s\n",nameDir);
		else printf("rm : directory %s doesn't exist\n",nameDir);
	}

	return 0;
}

/*
	Function name : 
		rm
	Arguments : 
		int argc (number of passed arguments)
		char *argv[] (arguments)
	Description : 
		removes a file/directory
*/
int rm(int argc, char *argv[]){
	// An error is displayed if there is too few arguments
	if(argc<2){
		printf("rm : too few arguments\n");
		return(-1);
	}
	// Remove the file passed in parameters
	else{
		// Declaration of variables
		int dflag = 0,iflag = 0,rflag = 0;	// used to know what options are required

		// We check if option "-d", "-i" or "r" is required
		char c;
		while ( (c = getopt(argc, argv, "dir")) != -1 ){
			switch(c){
				case 'd':
					dflag = 1;
					break;
				case 'i':
					iflag = 1;
					break;
				case 'r':
					rflag = 1;
					break;
				case '?':
					printf("rm : option %s doesn't exist for command rm\n",argv[optind-1]);
					return -1;
			}
		}
		// We check if option "-d" is required, if dflag==1 it means we want to remove a directory
		int index;
		errno=0;
		if(dflag==1){
			// For each element we check if it's a directory which exists
			for (index = optind; index < argc; index++){
				if(isFolder(argv[index])){
					// If option "-i" is required we ask user if he's sure to remove this directory
					if(iflag==1){
						char yesOrNo;
						printf("Do you really want to remove the directory %s ? [y/n] ",argv[index]);
						while(yesOrNo!='y' && yesOrNo!='n'){
							scanf("%c",&yesOrNo);
							while(getchar()!='\n');
							// If he's sure we remove it
							if(yesOrNo=='y'){
								if(rmdir(argv[index])!=0){
									// If the directory is empty we check if option "-r" is required
									if(errno==ENOTEMPTY){
										// If it is we remove recursively the directory
										if(rflag==1){
											if(recursiveRmDir(argv[index])!=0){
												return -1;
											}
										}
										else printf("rm : directory %s is not empty\n",argv[index]);
									}
									// If the user don't have the right to remove this directory an error message is displayed
									else if(errno==EACCES) printf("rm : you don't have rights on the directory %s\n",argv[index]);
									else printf("rm : directory %s doesn't exist\n",argv[index]);
								}
							}
							else if(yesOrNo=='n') printf("rm : the directory %s hasn't been removed\n",argv[index]);
						}
						yesOrNo='0';
					}
					// Else we remove it normaly
					else if(rmdir(argv[index])!=0){
						// If the directory is empty we check if option "-r" is required
						if(errno==ENOTEMPTY){
							// If it is we remove recursively the directory
							if(rflag==1){
								if(recursiveRmDir(argv[index])!=0){
									return -1;
								}
							}
							else printf("rm : directory %s is not empty\n",argv[index]);
						}
						// If the user don't have the right to remove this directory an error message is displayed
						else if(errno==EACCES) printf("rm : you don't have rights on the directory %s\n",argv[index]);
						else printf("rm : directory %s doesn't exist\n",argv[index]);
					}
				}
				else printf("rm : directory %s doesn't exist\n",argv[index]);
			}
		}
		else{
			// For each element we check if it's a file which exists
			for (index = optind; index < argc; index++){
				if(isRegularFile(argv[index])){
					// If option "-i" is required we ask user if he's sure to remove this file
					if(iflag==1){
						char yesOrNo;
						printf("Do you really want to remove the file %s ? [y/n] ",argv[index]);
						while(yesOrNo!='y' && yesOrNo!='n'){
							scanf("%c",&yesOrNo);
							while(getchar()!='\n');
							// If he's sure we remove it
							if(yesOrNo=='y'){
								if(unlink(argv[index])!=0) printf("rm : file %s doesn't exist\n",argv[index]);
							}
							else if(yesOrNo=='n') printf("rm : the file %s hasn't been removed\n",argv[index]);
						}
						yesOrNo='0';
					}
					// Else we remove it normaly
					else if(unlink(argv[index])!=0) printf("rm : file %s doesn't exist\n",argv[index]);
				}
				else printf("rm : file %s doesn't exist\n",argv[index]);
			}
		}
	}

	return 0;
}