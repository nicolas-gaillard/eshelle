#include "option_folder.c"

#include <unistd.h>

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
	printf("Error ! L'argument %s n'est pas un dossier !\n",argv[1]);
	return(-1);
    }

    return 0;
}
