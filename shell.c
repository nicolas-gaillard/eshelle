#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define _GNU_SOURCE

/*
shebang : #! puis script, permet de lancer un script
*/

void clean(const char *buffer, FILE *fp)
{
    char *p = strchr(buffer,'\n');
    if (p != NULL)
        *p = 0;
    else
    {
        int c;
        while ((c = fgetc(fp)) != '\n' && c != EOF);
    }
}

void clear(){
  printf("\033c");
}

void prompt(char *curDir, char *hostName){
	// username@nameofthemachine:currentdirectory

	getcwd(curDir,sizeof(curDir));
	//curDir = get_current_dir_name();
	//getlogin_r(hostName, sizeof(hostName));
	// getenv("NAME");
	//getenv("PWD")
	//get_current_dir_name()

	//gethostname(hostName, sizeof(hostName));
	//printf("%s@%s:%s$ \n", ,hostName, curDir);
	printf("%s@%s:%s$ \n", getlogin(), gethostname(hostName, sizeof(hostName)), get_current_dir_name());

}

void command(char *cmd, int sizeCmd){
	fgets(cmd, sizeof(cmd), stdin);
    clean(cmd, stdin);
}


int main(int argc, char const *argv[]) {
	char curDir[30];
	char cmd[100] = "";
	char hostName[30];

	// Clear the terminal 
	clear();

	// Display the prompt :
	prompt(curDir, hostName);

	// Listenning the command :
	//command(cmd, sizeof(cmd));

  	return 0;
}
