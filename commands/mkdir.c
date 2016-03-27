#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

int isMode(char *s){
    int i=0;
    int isM=1;
    if(strlen(s)<3 || strlen(s)>4) isM=0;
    while(isM==1 && i<strlen(s)){
	if(s[i]<'0' || s[i]>'7') isM=0;
	i++;
    }
    return isM;
}

int main(int argc,char *argv[]){
  
    int mode=0775;
    int mflag=0;
    int index;
    int c;  
  
    if(argv[1]!=NULL){
	opterr=0;
	while ((c = getopt (argc, argv, "abc:")) != -1)
	    switch (c){
		case '?':
		    if (optopt == 'm') mflag=1;
		    break;
		default:
		    abort ();
	    }

	for (index = optind; index < argc; index++){
	    if(mflag==1){
		if(isMode(argv[index])){
		    mode=(int) strtol(argv[index],(char **)NULL, 10);
		    mflag=0;
		}
		else{
		    printf("Error ! Le mode %s n'est pas valide !\n",argv[index]);
		    return(-1);
		}
	    }
	    else{
		if(mkdir(argv[index],mode)!=0){
		    printf("Error ! Le dossier %s existe déjà à cet emplacement !\n",argv[index]);
		    return(-1);
		}
	    }
	} 
    }
    else{
	printf("Error ! L'argument %s n'est pas un nom de dossier !\n",argv[1]);
	return(-1);
    }

    return 0;
}
