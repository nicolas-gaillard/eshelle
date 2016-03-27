#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

struct stat sts;  
    
int isOption(char* s){
    if(s!=NULL && s[0]=='-') return 1;
    else return 0;
}

int isFolder(char* s){
    if(s!=NULL && stat(s,&sts)==0 && S_ISDIR(sts.st_mode)) return 1;
    else return 0;
}

int isRegularFile(char* s){
    if(s!=NULL && stat(s,&sts)==0 && S_ISREG(sts.st_mode)) return 1;
    else return 0;
}