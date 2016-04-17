/*** Author : Quentin Levavasseur ***/

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#define LIB_COMMANDS "libcommands.so"

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
	
	char **ligneCommandeTest =[["cd","../"]];
	//char **ligneCommandeTest =[["ls","-l"]];
	
	int i=;
	for(i=0;i<=1;i++0){
        if(i==1){
            if(ligneCommandeTest[0][0]=="cd"){
                if ((cd = (pfCD) dlsym(lib, "cd")) == NULL) {
            		return 1; //erreur, fonction introuvable
            	}
            	
            }
            else if(ligneCommandeTest[0][0]=="ls"){
                if ((ls = (pfLS) dlsym(lib, "ls")) == NULL) {
            		return 1; //erreur, fonction introuvable
            	}
            }
            else{
                printf("Commande %s inconnue\n",ligneCommandeTest[0][0]);
            }
        }	    
	}

    return 0;
    
}