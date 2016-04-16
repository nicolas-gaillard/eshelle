#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
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
	if (strcmp(cmd[size - 1][0], "&")){
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

	getcwd(currentDir,sizeof(currentDir));
	gethostname(hostName, sizeof(hostName));

	// strcmp(currentDir,"") == *currentDir == NULL;	
	if ((strcmp(currentDir,"") == 0) && (strcmp(hostName,"") == 0)){
		printf("%s@bash:~$ ", getlogin());

	}
	else if (strcmp(currentDir,"") == 0){
		printf("%s@%s:~$ ", getlogin(), hostName);

	}
	else if (strcmp(hostName,"") == 0){
		printf("%s@bash:%s$ ", getlogin(), currentDir);	
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

	// Welcome message
	printf("Shell v1.0 \nEnter \"quit\" to leave\n");

	while(1){
		// Display the prompt :
		prompt(currentDir, hostName);

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

    			// If the command has to be executed in background :
    			if (background(command, size) == 1){
    				int pid;
					if ((pid = fork()) == -1){
						perror("fork failed ");
					}

					// Child process will execute in background
					if (pid == 0){
						execute((char***)command, 0, STDIN_FILENO);
    				}
    			}
    			else {
    				execute((char***)command, 0, STDIN_FILENO);
    			}
    		}

    	}
  
    	// strtok(chaine, mot) pour supprimer les "mot" d'une chaine de caractère

/*
    	// Checking the command :
    	else if (exist(command, functions) == 0){
    		// The command doesn't exist
    		perror("The command doesn't exist ");
    	   	exit(0);
    	}
    	else{
    		// Creation of the process that will execute the command
    		int pid = fork();
    		int *status = NULL;
    		if (pid == 0){
    			// Interpretation of the command
    			if (execv("command", argCommand) == -1) {
    				//kill(pid, SIGKILL);
    				exit(0);
    			}
    		}
    		else{
    			// The parent process waits the end of the command
    			wait(status);
    		}	
    	}
    */	
	}

	return 0;
}