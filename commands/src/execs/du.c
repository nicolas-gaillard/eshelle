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
		displayFileSize 
	Arguments :
		char *nameFile (pathname of the file we want to know the size)
		int ?flag (flags which represent if options are required or not)
	Description : 
		displays the size of a file/directory passed in parameter
*/
void displayFileSize(char *nameFile,unsigned int size,int hflag,int bflag,int kflag,int mflag){
	// If option "-h" is required we display the file size its unit
	if(hflag==1){
		// If option "-b" is required we display the file size in Bits
		if(bflag==1) printf("%dB	%s\n",(size*8),nameFile);
		// If option "-k" is required we display the file size in KiloBytes
		else if(kflag==1) printf("%dK	%s\n",(size/1024),nameFile);
		// If option "-m" is required we display the file size in MegaBytes
		else if(mflag==1) printf("%dM	%s\n",((size/1024)/1024),nameFile);
		// Else we display the file size in Bytes
		else printf("%dO	%s\n",size,nameFile);
	}
	else{
		if(bflag==1) printf("%d	%s\n",(size*8),nameFile);
		else if(kflag==1) printf("%d	%s\n",((size/8)/1024),nameFile);
		else if(mflag==1) printf("%d	%s\n",(((size/8)/1024)/1024),nameFile);
		else printf("%d	%s\n",size,nameFile);
	}
}

/*
	Function name : 
		recursiveDu 
	Arguments :
		char *nameFile (pathname of the file we want to know the size)
	Description : 
		displays the size of a file/directory and his content
*/
unsigned int recursiveDu(char *nameFile,int aflag,int bflag,int hflag,int kflag,int mflag){
	// Declaration of variables	
	DIR *dirp;					// the directory we want to know the size
	struct dirent *dptr;		// used to know the information of the content directory
	struct stat file_stat;		// used to know the information of the content file/directory
	char path[1024] = {0};		// a tab which contains the path of a file containing in the directory
	unsigned int size=0;		// the size of the file/directory
	
	// If it's a directory we collect the size of its content files/directories recursively
	if(isFolder(nameFile)){
		if ((dirp=opendir(nameFile))==NULL) printf("du : directory %s doesn't exist\n",nameFile);
		else{
			if(stat(nameFile,&file_stat)!=0){
				printf("du : directory %s doesn't exist\n",nameFile);
				exit(1);
			}
			size+=file_stat.st_size;
				
			while((dptr=readdir(dirp))){
				// We ignore the "." and ".." directories
				if (!strcmp(dptr->d_name,".") || !strcmp(dptr->d_name,"..")) continue;
				// We make the path of the content file/directory
				if (nameFile[strlen(nameFile)-1]=='/') sprintf(path,"%s%s",nameFile,dptr->d_name);
				else sprintf(path,"%s/%s",nameFile,dptr->d_name);
				// If option "-a" is required we display the size of the content files/directories
				if(aflag==1){
					displayFileSize(path,size,hflag,bflag,kflag,mflag);
				}
				// We get the size of the content file/directory to add it to the main file size
				size+=recursiveDu(path,aflag,bflag,hflag,kflag,mflag);
			}
			// Directory closure
			closedir(dirp);
		}
	}
	// Else if it's just a file we get its size
	else if(isRegularFile(nameFile)){
		if(stat(nameFile,&file_stat)!=0){
			printf("du : file %s doesn't exist\n",nameFile);
			exit(1);
		}
		size+=file_stat.st_size;
	}
	// Or an error will be displayed
	else{
		printf("du : file or directory %s doesn't exist\n",nameFile);
		exit(1);
	}
	
	// Finally we return the file size
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
	unsigned int size=0;								// the size of the file/directory
	int aflag = 0, bflag=0, hflag=0, kflag=0, mflag=0;	// used to know what options are required
	char *nameFile;										// the of the file/directory we want to know the size
		
	// We check if option "-a", "-b", "-h", "-k" or "-m" is required
	char c;
	while ( (c = getopt(argc, argv, "abhkm")) != -1 ){
		switch(c){
			case 'a':
				aflag = 1;
				break;
			case 'b':
				bflag = 1;
				break;
			case 'h':
				hflag = 1;
				break;
			case 'k':
				kflag = 1;
				break;
			case 'm':
				mflag = 1;
				break;
			case '?':
				printf("rm : option %s doesn't exist for command rm\n",argv[optind-1]);
				return -1;
		}
	}
	
	int index = optind;
	// If there is no argument we take automatically the current directory
	if(argv[index]==NULL){
		nameFile=".";
		// We get the size of the current directory
		size+=recursiveDu(nameFile,aflag,bflag,hflag,kflag,mflag);
	}
	// Else for each arguments we get and and display its size
	else{
		for (index = optind; index < argc; index++){
			nameFile=argv[index];
			// We get the size of the file/directory passed in arguments
			size+=recursiveDu(nameFile,aflag,bflag,hflag,kflag,mflag);
		}
	}
	
	// We display the size of the file/directory
	displayFileSize(nameFile,size,hflag,bflag,kflag,mflag);

	return 0;
}