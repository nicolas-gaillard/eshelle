#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

#define _GNU_SOURCE
#define BUFFER_KEYBOARDING 100
#define BUFFER_PROMPT 30
#define NUMBER_FONCTIONS 18

// WARNING : 

char *functions[NUMBER_FONCTIONS] = 
{"ls", "mkdir", "cd", "pwd", "cat", "more", "less", "mv", "cp", "rm", "du", "chown",
"chgrp", "echo", "ps", "top", "su"};

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

void prompt(char *currentDir, char *hostName){
	// username@nameofthemachine:currentdirectory

	//getcwd(currentDir,sizeof(currentDir));
	gethostname(hostName, sizeof(hostName));

	// TEST : 
	//currentDir = get_current_dir_name();
	//getlogin_r(hostName, sizeof(hostName));
	// getenv("NAME");
	//getenv("PWD")
	//get_current_dir_name()

	printf("Interpreteur de commande v1.0 \nTaper \"quit\" pour quitter\n");

	// strcmp(currentDir,"") == *currentDir == NULL;
	/*
	if ((strcmp(currentDir,"") == 0) && (strcmp(hostName,"") == 0)){
		printf("%s@bash:~$ \n", getlogin());

	}
	else if (strcmp(currentDir,"") == 0){
		printf("%s@%s:~$ \n", getlogin(), hostName);

	}
	else if (strcmp(hostName,"") == 0){
		printf("%s@bash:%s$ \n", getlogin(), currentDir);	
	}
	else {
		printf("%s@%s:%s$ \n", getlogin(), hostName, currentDir);
	}
	*/
	if (strcmp(hostName,"") == 0)
	{
		printf("%s@bash:%s$ \n", getlogin(), get_current_dir_name());	
	}
	else{
		printf("%s@%s:%s$ \n", getlogin(), hostName,get_current_dir_name());	
	}
	fflush(stdout);
}

int exist(char *c, char *t[]){
	for (int i = 0; i < NUMBER_FONCTIONS; ++i)
	{
		if (t[i] == c){
			return 1;
		}
	}
	return 0;
}


// Rajouter la libération de ressources

int main(int argc, char const *argv[]) {
	//char currentDir[BUFFER_PROMPT];
	char keyboarding[BUFFER_KEYBOARDING] = "";
	char hostName[BUFFER_PROMPT];

	// Name of the command
	// 5 is the max number of letters of a command
	char command[5];
	// Arguments of the command
	char *argCommand[95];

	// Clear the terminal 
	clear();

	while(1){
		// Display the prompt :
		prompt(currentDir, hostName);

		// Listenning the command :
		fgets(keyboarding, sizeof(keyboarding), stdin);
    	clean(keyboarding, stdin);

    	// Automate qui sépare la commande en :
    	// command = automate(keyboarding, argCommand);
    	// strtok(chaine, mot) pour supprimer les "mot" d'une chaine de caractère

    	// Checking the command :
    	if (strcmp(command,"quit") == 0){
    		// User wants to quit :
    		exit(0); // A modifier en fonction des sockets 
    		
    	}
    	else if (exist(command, functions) == 0){
    		// The command doesn't exist
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
	}

	//free(currentDir);
	free(keyboarding);
	free(hostName);
	free(command);
	free(argCommand);

  	return 0;
}
