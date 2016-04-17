#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>


#include "automate.h"
#include "decoupe.h"
#include "execution.h"

#define BUFFER_KEYBOARDING 256
#define BUFFER 50
#define NUMBER_FONCTIONS 18

/*
shebang : #! puis script, permet de lancer un script
*/

int background(char** cmd[], int size){
	if (strcmp(cmd[size - 1][0], "&")==0){
		cmd[size - 1] = NULL;
		return 1;
	}
	else{
		return 0;
	}
}

/*
Plus propre : 
tu fork
dans le fils tu fais exécuter le programme
tu stockes le pid du fils dans le père
tu fous un pthread pour faire un waitpid avec le pid du fils
des que le fils termine, il balance l'exitcode dans le waitpid du pthread
et à chaque commande dans ton shell tu check l'ensemble de tes programmes en bg, si y'en a un qui est terminé tu balances un affichage comme ça [iddubg] (EXITCODE) ...
Et tu l'enlèves de la liste des procs en bg
*/

void clear(){
  printf("\033c");
}

void prompt(char *currentDir, char *hostName){
	//getenv("NAME");
	//getenv("PWD");

	// username@nameofthemachine:currentdirectory

	getcwd(currentDir,BUFFER*sizeof(char));
	gethostname(hostName, BUFFER*sizeof(char));

	// strcmp(currentDir,"") == *currentDir == NULL;	
	if ((strcmp(currentDir,"") == 0) && (strcmp(hostName,"") == 0)){
		printf("%s@bash:~$ ", getlogin());

	}
	else if (strcmp(currentDir,"") == 0){
		printf("%s@%s:~$ ", getlogin(), hostName);

	}
	else if (strcmp(hostName,"") == 0){
		printf("%s@bash:%s$ ", getlogin(),currentDir);	
	}
	else {
		printf("%s@%s:%s$ ", getlogin(), hostName, currentDir);
	}

	/*
	OR : 
	
	if (strcmp(hostName,"") == 0)
	{
		printf("%s@bash:%s$ \n", getlogin(), get_current_dir_name());	
	}
	else{
		printf("%s@%s:%s$ \n", getlogin(), hostName,get_current_dir_name());	
	}
	*/

	fflush(stdout);
}

/*
int exist(char *c, char *t[]){
	for (int i = 0; i < NUMBER_FONCTIONS; ++i)
	{
		if (t[i] == c){
			return 1;
		}
	}
	return 0;
}
*/

// FILE  *freopen  (const  char *path, const char *mode, FILE *stream);

int main(int argc, char const *argv[]) {
	
	char currentDir[BUFFER];
	char keyboarding[BUFFER_KEYBOARDING] = "";
	char hostName[BUFFER];
	
	// Clear the terminal 
	clear();
	
	getcwd(currentDir,BUFFER*sizeof(char));
	char PATH[]="";
	strcat(PATH,currentDir);
	strcat(PATH,"/commands/bin/");
	setenv("PATH",PATH,1);

	//setenv("PS1","user@hote:currentdirectory$ ",1);
	
	// Welcome message
	printf("Shell v1.0 \nEnter \"quit\" to leave\n");

	while(1){
		// Display the prompt :
		prompt(currentDir, hostName);
		//printf("%s",getenv("PS1"));
	
		// Listenning the command :
		fgets(keyboarding, sizeof(keyboarding), stdin);
    	clean(keyboarding, stdin);

    	// User wants to quit 
    	if (strcmp(keyboarding,"quit") == 0){
    		exit(0); // A modifier en fonction des sockets     		
    	}

    	else{

    		// Checking the command :
    		if (automate(keyboarding) == 1){
    			// If the command is ok :
    			int size;
    			// We cut the command
    			char *** command = decoupe(keyboarding, &size);
    			char *** commandBackground = command;
    			
    			if(strcmp(command[0][0],"cd")==0){
    				struct stat sts;
    				if(command[0][1]!=NULL && stat(command[0][1],&sts)==0 && S_ISDIR(sts.st_mode)){
						chdir(command[0][1]);
					}
					else{
						printf("cd : directory %s doesn't exist\n",command[0][1]);
					}
    			}
    			else{
    				// If the command has to be executed in background :$
					if (background(command, size) == 1){
						int pid;
						if ((pid = fork()) == -1){
							perror("fork failed ");
						}
	
						// Child process will execute in background
						if (pid == 0){
							execute((char***)commandBackground, 0, STDIN_FILENO);
							free(commandBackground);
	    				}
	    			}
	    			else {
	    				int pid;
						if ((pid = fork()) == -1){
							perror("fork failed ");
						}
	
						// Child process will execute in background
						if (pid == 0){
							execute((char***)command, 0, STDIN_FILENO);
	    				}
	    				else{
	    					wait(NULL);
	    				}
	    			}
    			}
    			
    			free(command);
    		}

    	}
    	
	}

	return 0;
}