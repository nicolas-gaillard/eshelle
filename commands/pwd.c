#include <unistd.h>

#define TAILLE 100

int main(int argc,char *argv[]){

    char path[TAILLE];
    getcwd (path, TAILLE-1);
    printf("%s\n",path);

    return 0;
}
