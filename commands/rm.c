/*** Author : Quentin Levavasseur ***/

#include "IS_file.c"
#include <unistd.h>
#include <string.h>
#include <errno.h>

/*
	Function name : 
		main 
	Arguments : 
		int argc (number of passed arguments)
		char *argv[] (arguments)
	Description : 
		removes a file/directory
*/
int main(int argc, char *argv[]){
	// An error is displayed if there is too few arguments
	if(argc<2){
		printf("rm : too few arguments\n");
		return(-1);
	}
	else{
		// Declaration of variables
		int dflag = 0,iflag = 0;	// used to know what options are required

		// We check if option "-d" or "-i" is required
		char c;
		while ( (c = getopt(argc, argv, "di")) != -1 ){
			switch(c){
				case 'd':
					dflag = 1;
					break;
				case 'i':
					iflag = 1;
					break;
				case '?':
					printf("rm : option %s doesn't exist for command rm\n",argv[optind-1]);
					return -1;
			}
		}
		// For each element we check if it's a file/directory
		int index;
		errno=0;
		if(dflag==1){
			for (index = optind; index < argc; index++){
				if(isFolder(argv[index])){
					if(iflag==1){
						char yesOrNo;
						printf("Do you really want to remove the directory %s ? [y/n] ",argv[index]);
						while(yesOrNo!='y' && yesOrNo!='n'){
							scanf("%c",&yesOrNo);
							while(getchar()!='\n');
							if(yesOrNo=='y'){
								if(rmdir(argv[index])!=0){
									errno=rmdir(argv[index]);
									if(errno==ENOTEMPTY) printf("rm : directory %s is not empty\n",argv[index]);
									else if(errno!=0) printf("rm : directory %s doesn't exist\n",argv[index]);
								}
							}
							else if(yesOrNo=='n') printf("rm : the directory %s hasn't been removed\n",argv[index]);
						}
						yesOrNo='0';
					}
					
					else{
						int res=rmdir(argv[index]);
						if(res==ENOTEMPTY && errno != 0) printf("rm : directory %s is not empty\n",argv[index]);
						else if(errno!=0) printf("rm : directory %s doesn't exist\n",argv[index]);
					}
					strerror(errno);
				}
				else printf("rm : directory %s doesn't exist\n",argv[index]);
			}
		}
		else{
			for (index = optind; index < argc; index++){
				if(isRegularFile(argv[index])){
					if(iflag==1){
						char yesOrNo;
						printf("Do you really want to remove the file %s ? [y/n] ",argv[index]);
						while(yesOrNo!='y' && yesOrNo!='n'){
							scanf("%c",&yesOrNo);
							while(getchar()!='\n');
							if(yesOrNo=='y'){
								if(unlink(argv[index])!=0) printf("rm : file %s doesn't exist\n",argv[index]);
							}
							else if(yesOrNo=='n') printf("rm : the file %s hasn't been removed\n",argv[index]);
						}
						yesOrNo='0';
					}
					else if(unlink(argv[index])!=0) printf("rm : file %s doesn't exist\n",argv[index]);
				}
				else printf("rm : file %s doesn't exist\n",argv[index]);
			}
		}
	}

	return 0;
}