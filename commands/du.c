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
unsigned int recursiveDu(char *nameFile,int aflag,int bflag,int hflag,int kflag,int mflag){
	struct stat file_stat;
	DIR *dirp;
	struct dirent *dptr;
	char path[1024] = {0};
	unsigned int size=0;
	
	if(isFolder(nameFile)){
		if ((dirp=opendir(nameFile))==NULL) printf("du : directory %s doesn't exist\n",nameFile);
		else{
			if(stat(nameFile,&file_stat)!=0){
				printf("du : directory %s doesn't exist\n",nameFile);
				exit(1);
			}
			size+=file_stat.st_size;
				
			while(dptr=readdir(dirp)){
				if (!strcmp(dptr->d_name,".") || !strcmp(dptr->d_name,"..")) continue;
	
				if (nameFile[strlen(nameFile)-1]=='/') sprintf(path,"%s%s",nameFile,dptr->d_name);
				else sprintf(path,"%s/%s",nameFile,dptr->d_name);
				
				if(aflag==1){
					if(hflag==1){
						if(bflag==1) printf("%db	%s\n",size,path);
						else if(mflag==1) printf("%dM	%s\n",(((size/8)/1024)/1024),path);
						else if(kflag==1) printf("%dK	%s\n",((size/8)/1024),path);
						else printf("%do	%s\n",(size/8),path);
					}
					else{
						if(bflag==1) printf("%d	%s\n",size,path);
						else if(mflag==1) printf("%d	%s\n",(((size/8)/1024)/1024),path);
						else if(kflag==1) printf("%d	%s\n",((size/8)/1024),path);
						else printf("%d	%s\n",(size/8),path);
					}
				}
				size+=recursiveDu(path,aflag,bflag,hflag,kflag,mflag);
			}
			closedir(dirp);
		}
	}
	else if(isRegularFile(nameFile)){
		if(stat(nameFile,&file_stat)!=0){
			printf("du : file %s doesn't exist\n",nameFile);
			exit(1);
		}
		size+=file_stat.st_size;
	}
	else{
		printf("du : file or directory %s doesn't exist\n",nameFile);
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
	int aflag = 0, bflag=0, hflag=0, kflag=0, mflag=0;			// used to know what if option "-a" or "-k" is required
	char *nameFile;
		
	// We check if option "-a" is required
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
		
	// An error is displayed if there is too few arguments
	if(argc<2 || (argc==2 && argv[1][0]=='-')){
		nameFile=".";
		size+=recursiveDu(nameFile,aflag,bflag,hflag,kflag,mflag);
	}
	// Remove the file passed in parameters
	else{
		int index;
		// For each element we check if it's a directory which exists
		for (index = optind; index < argc; index++){
			nameFile=argv[index];
			/*if(stat(nameFile,&file_stat)!=0){
				printf("du : directory %s doesn't exist\n",nameFile);
				exit(1);
			}
			size=file_stat.st_size;*/
			size+=recursiveDu(nameFile,aflag,bflag,hflag,kflag,mflag);
		}
	}
	
	if(hflag==1){
		if(bflag==1) printf("%db	%s\n",size,nameFile);
		else if(mflag==1) printf("%dM	%s\n",(((size/8)/1024)/1024),nameFile);
		else if(kflag==1) printf("%dK	%s\n",((size/8)/1024),nameFile);
		else printf("%do	%s\n",(size/8),nameFile);
	}
	else{
		if(bflag==1) printf("%d	%s\n",size,nameFile);
		else if(mflag==1) printf("%d	%s\n",(((size/8)/1024)/1024),nameFile);
		else if(kflag==1) printf("%d	%s\n",((size/8)/1024),nameFile);
		else printf("%d	%s\n",(size/8),nameFile);
	}

	return 0;
}