#include "option_folder.c"

#include <unistd.h>
#include <string.h>

int main(int argc,char *argv[]){
   
    FILE *pFile;
    char s[100];
    char c;
		    
    if(argv[1]!=NULL){
	if(isOption(argv[1])){
	    if(strcmp(argv[1],"-n")==0){
		if(isRegularFile(argv[2])){
		    if(pFile=fopen(argv[2],"rt")){
			int numLigne=1;
			while (fgets(s, sizeof s, pFile) != NULL){
			    printf("%6d",numLigne);
			    printf("	%s", s);
			    numLigne++;
			}
			fclose(pFile);
		    }
		    else printf("Erreur d'ouverture du fichier !\n");
		}
		else{
		    printf("Le fichier %s n'existe plus ou pas !\n",argv[2]);
		}
	    }
	    else{
		printf("Error : L'option %s n'existe pas pour cette commande !\n",argv[1]);
		return -1;
	    }
	}
	else if(isRegularFile(argv[1])){
	    if(pFile=fopen(argv[1],"rt")){
		for(c=fgetc(pFile);!feof(pFile);c=fgetc(pFile)){
		    printf("%c",c);
		}
		fclose(pFile);
	    }
	    else{
		printf("Erreur d'ouverture du fichier !\n");
	    }
	}
	else{
	    printf("Le fichier %s n'existe plus ou pas !\n",argv[2]);
	    return -1;
	}
    }
    else{
	printf("Error ! L'argument %s n'est pas un fichier !\n",argv[1]);
	return -1;
    }

    return 0;
}
