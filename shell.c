#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

#define BUFFER_KEYBOARDING 256
#define BUFFER 50
#define NUMBER_FONCTIONS 18
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

int exist(char *c, char *t[]){
	for (int i = 0; i < NUMBER_FONCTIONS; ++i)
	{
		if (t[i] == c){
			return 1;
		}
	}
	return 0;
}

// Pour le pipe, passé par un fichier temporaire tmpfile()

// FILE  *freopen  (const  char *path, const char *mode, FILE *stream);

// Rajouter la libération de ressources
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

    	// Automate qui sépare la commande en :
    	// command = automate(keyboarding, argCommand);
    	// strtok(chaine, mot) pour supprimer les "mot" d'une chaine de caractère

    	// Checking the command :
    	
/*
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