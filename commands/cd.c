#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int isOption(char* s){
    if(s!=NULL && s[0]=='-') return 1;
    else return 0;
}

int isFolder(char* s){
    struct stat sts;  
    if(s!=NULL && stat(s,&sts)==0 && S_ISDIR(sts.st_mode)) return 1;
    else return 0;
}

int main(int argc,char *argv[]){
   
    if(argv[1]!=NULL){
	if(isFolder(argv[1])){
	    printf("Repertoire courant avant : %s\n",get_current_dir_name());
	    chdir(argv[1]);
	    printf("Repertoire courant après : %s\n",get_current_dir_name());
	}
	else{
	    printf("Error ! Le dossier %s n'existe pas ou plus !\n",argv[1]);
	    return(-1);
	}
    }
    else{
	printf("Error ! L'argument %s n'est pas un dossier !\n",argv[1]
	return(-1);
    }

    return 0;
}
