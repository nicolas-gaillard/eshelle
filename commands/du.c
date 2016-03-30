/*** Author : Quentin Levavasseur ***/

#include "IS_file.c"
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

/*
	Function name : 
		recursiveDu 
	Arguments :
		char *nameFile (pathname of the file we want to know the size)
	Description : 
		displays the size of a file/directory and his content
*/
unsigned int recursiveDu(char *nameFile,unsigned int size,int aflag){
	struct stat file_stat;
	DIR *dirp;
	struct dirent *dptr;
	char path[1024] = {0};
	unsigned int fileSize=0;
	
	if(isFolder(nameFile)){
		if(stat(nameFile,&file_stat)!=0){
			printf("du : directory %s doesn't exist\n",nameFile);
			exit(1);
		}
		fileSize=file_stat.st_size;
		if ((dirp=opendir(nameFile))==NULL) printf("du : directory %s doesn't exist\n",nameFile);
		else{
			while(dptr=readdir(dirp)){
				if (!strcmp(dptr->d_name,".") || !strcmp(dptr->d_name,"..")) continue;
	
				if (nameFile[strlen(nameFile)-1]=='/') sprintf(path,"%s%s",nameFile,dptr->d_name);
				else sprintf(path,"%s/%s",nameFile,dptr->d_name);
	
				size+=recursiveDu(path,fileSize,aflag);
				if(aflag==1) printf("%d	%s\n",fileSize,path);
			}
			closedir(dirp);
		}
	}
	else if(isRegularFile(nameFile)){
		stat(nameFile,&file_stat);
		fileSize=file_stat.st_size;
		printf("%d	%s\n",fileSize,nameFile);
	}
	else{
		printf("du : file %s doesn't exist\n",nameFile);
		exit(1);
	}
	
	return size;
}

/*
	Function name : 
		main 
	Arguments : 
		int argc (number of passed arguments)
		char *argv[] (arguments)
	Description : 
		display the size of a file/directory
*/
int main(int argc, char *argv[]){
	// Declaration of variables
	unsigned int size=0;
	int aflag = 0;			// used to know what if option "-a" is required
	char *nameFile;
		
	// We check if option "-a" is required
	char c;
	while ( (c = getopt(argc, argv, "a")) != -1 ){
		switch(c){
			case 'a':
				aflag = 1;
				break;
			case '?':
				printf("rm : option %s doesn't exist for command rm\n",argv[optind-1]);
				return -1;
		}
	}
		
	// An error is displayed if there is too few arguments
	if(argc<2){
		nameFile=".";
		size+=recursiveDu(nameFile,size,aflag);
	}
	// Remove the file passed in parameters
	else{
		int index;
		// For each element we check if it's a directory which exists
		for (index = optind; index < argc; index++){
			nameFile=argv[index];
			size+=recursiveDu(nameFile,size,aflag);
		}
	}
	
	printf("%d	%s\n",size,nameFile);

	return 0;
}