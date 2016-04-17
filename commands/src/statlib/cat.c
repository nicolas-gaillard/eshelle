/*** Author : Quentin Levavasseur ***/

#include "../../lib/IS_file.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TAILLE 100

/*
	Function name : 
		cat
	Arguments : 
		int argc (number of passed arguments)
		char *argv[] (arguments)
	Description : 
		displays the file's content
*/
int cat(int argc,char *argv[]){
	// An error is displayed if there is too few arguments
	if(argc<2){
		printf("cat : too few arguments\n");
		return(-1);
	}
	else{
		// Declaration of variables
		FILE *pFile;	// the file passed in parameter
		char s[TAILLE];	// a tab of char to display line by line
		int nflag=0;	// used to know if option "-n" is required or not
		
		// We check if option "-n" is required
		int c;
		opterr=0;
		while ((c = getopt (argc, argv, "n")) != -1){
			switch (c){
				case 'n':
					nflag=1;
					break;
				case '?':
					printf("cat : option %s doesn't exist for command cat\n",argv[optind-1]);
					return(-1);
			}
		}
		// For each element we check if it's a file name
		int index;
		for (index = optind; index < argc; index++){
			// If the argument is a regular file we display it or an error will be displayed
			if(isRegularFile(argv[index])){
				if((pFile=fopen(argv[index],"rt"))){
					// If option "-n" is required we display lines numbers before the content
					if(nflag==1){
						int numLigne=1;
						while (fgets(s, sizeof s, pFile) != NULL){
							printf("%6d",numLigne);
							numLigne++;
							printf("	%s", s);
						}
					}
					else{
						while (fgets(s, sizeof s, pFile) != NULL){
							printf("%s", s);
						}
					}
					fclose(pFile);
				}
				else printf("cat : opening file error\n");
			}
			else{
				printf("cat : file %s doesn't exist\n",argv[2]);
			}
		}
	}

	return 0;
}
