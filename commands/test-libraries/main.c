/*** Author : Quentin Levavasseur ***/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dlfcn.h>
#include <string.h>
#include <unistd.h>

#define LIB_COMMANDS "libcommands.so"


// COMMANDES A FAIRE POUR LANCER LE MAIN

//gcc -fPIC -c ../cd.c 		#génère un .o 
//gcc -fPIC -c ../ls.c 		#génère un .o 
//gcc -fPIC -c ../IS_file.c 	#génère un .o 
//gcc -shared -o libcommands.so cd.o ls.c IS_file.o

//gcc -Wall -o main main.c -ldl 
//export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/.
//./main

void *lib;

typedef void (*pfCD)(int argc,char *argv[]);
typedef void (*pfLS)(int argc,char *argv[]);

pfCD cd;
pfLS ls;

/*
	Function name : 
		main 
	Arguments : 
		int argc (number of passed arguments)
		char *argv[] (arguments)
	Description : 
    	executes the command passed in parameters
*/
int main(int argc, char *argv[]){
    
	if ((lib = dlopen(LIB_COMMANDS, RTLD_LAZY)) == NULL) {
		printf("libintrouvable\n");
		return -1; //erreur, lib introuvable
	}
	if ((cd = (pfCD) dlsym(lib, "cd")) == NULL) {
		return 1; //erreur, fonction introuvable
	}
	
	char *ligneCommandeTest[2];
	
	// Test pour "cd ../"
	ligneCommandeTest[0]="cd";
	ligneCommandeTest[1]="../";
	
	// Test pour "ls -l" --> NON FONCTIONNEL
	//ligneCommandeTest[0]="ls";
	//ligneCommandeTest[1]="-l";
	
	//Autre possbilité ? Scanf ?
	//char ***ligneCommandeTest = {{"ls","-l"}};
	
	printf("Ligne de commande : %s %s\n",ligneCommandeTest[0],ligneCommandeTest[1]);
	
	int i=0;
	for(i=0;i<=1;i++){
		if(i==1){
			if(strcmp(ligneCommandeTest[0],"cd")==0){
				if ((cd = (pfCD) dlsym(lib, "cd")) == NULL) {
					return 1; //erreur, fonction introuvable
				}
				if(2<2){
					printf("cd : too few arguments\n");
					return(-1);
				}
				// An error is displayed if there are too many arguments
				else if(2>2){
					printf("cd : too many arguments\n");
					return(-1);
				}
				// Change current repertory
				else{
					struct stat sts;  
					// We change the repertory or an error will be displayed if it doesn't exist
					if((ligneCommandeTest[1]!=NULL && stat(ligneCommandeTest[1],&sts)==0 && S_ISDIR(sts.st_mode))==1){
						printf("cd : Test : Repertoire courant avant : %s\n",get_current_dir_name());
						chdir(ligneCommandeTest[1]);
						printf("cd : Test : Repertoire courant apr�s : %s\n",get_current_dir_name());
					}
					else{
						printf("cd : directory %s doesn't exist\n",ligneCommandeTest[1]);
						return(-1);
					}
				}
			}
			else if(strcmp(ligneCommandeTest[0],"ls")==0){
				if ((ls = (pfLS) dlsym(lib, "ls")) == NULL) {
					printf("lol\n");
					return 1; //erreur, fonction introuvable
				}
				ls(2,ligneCommandeTest);
			}
			else{
				printf("Commande %s inconnue\n",ligneCommandeTest[0]);
			}
		}
	}

	return 0;
    
}