/*** Author : Quentin Levavasseur ***/

#include "option_folder.c"
#include <unistd.h>
#include <string.h>

/*
	Function name : 
		main 
	Arguments : 
		int argc (number of passed arguments)
		char *argv[] (arguments)
	Description : 
		displays the file's content
*/
int main(int argc,char *argv[]){
	// An error is displayed if there is too few arguments
	if(argc<2){
		printf("mkdir : too few arguments\n");
		return(-1);
	}
	// An error is displayed if there are too many arguments
	else if(argc>4){
		printf("mkdir : too many arguments\n");
		return(-1);
	}
	else{
		// Declaration of variables
		FILE *pFile;	// the file passed in parameter
		char s[100];	// a tab of char to display line by line
		int nflag=0;	// used to know if option "-n" is required or not
		
		// We check if option "-n" is required
		int c;
		opterr=0;
		while ((c = getopt (argc, argv, "abc:")) != -1){
			switch (c){
				case '?':
					if (optopt == 'n') nflag=1;
					break;
				default:
					abort ();
			}
		}
		// For each elements we check if it's a file name
		int index;
		for (index = optind; index < argc; index++){
			// If the argument is a regular file we display it or an error will be displayed
			if(isRegularFile(argv[index])){
				if(pFile=fopen(argv[index],"rt")){
					int numLigne=1;
					while (fgets(s, sizeof s, pFile) != NULL){
						if(nflag==1){
							printf("%6d",numLigne);
							numLigne++;
						}
						printf("	%s", s);
					}
					fclose(pFile);
				}
				else printf("Erreur d'ouverture du fichier !\n");
			}
			else{
				printf("Le fichier %s n'existe plus ou pas !\n",argv[2]);
			}
		}
	}

	return 0;
}

