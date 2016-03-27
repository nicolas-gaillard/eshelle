#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc,char *argv[]){
   
    if(argv[1]!=NULL){
	if(mkdir(argv[1],0775)!=0){
	    printf("Error ! Le dossier %s existe déjà à cet emplacement !\n",argv[1]);
	    return(-1);
	}
    }
    else{
	printf("Error ! L'argument %s n'est pas un nom de dossier !\n",argv[1]);
	return(-1);
    }

    return 0;
}
