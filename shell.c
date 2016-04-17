#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

#include "shell.h"
#include "automate.h"
#include "decoupe.h"
#include "execution.h"

#define BUFFER_KEYBOARDING 256
#define BUFFER 50
#define NUMBER_FONCTIONS 18


// Function to know if we have to execute the command in background
int background(char** cmd[], int size){
	// If the last char is '&'
	if (strcmp(cmd[size - 1][0], "&")==0){
		// We set it to NULL not to interprete it during the execution
		cmd[size - 1] = NULL;
		return 1;
	}
	else{
		return 0;
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

int main(int argc, char const *argv[]) {
	
	char currentDir[BUFFER];
	char keyboarding[BUFFER_KEYBOARDING] = "";
	char hostName[BUFFER];

	// Clear the terminal 
	clear();
	
	char PATH[]="./commands/execs/:";
	strcat(PATH,getenv("PATH"));
	setenv("PATH",PATH,1);

	setenv("PS1","user@hote:currentdirectory$ ",1);
	
	// Welcome message
	printf("Shell v1.0 \nEnter \"quit\" to leave\n");

	while(1){
		// Display the prompt :
		//prompt(currentDir, hostName);
		printf("%s",getenv("PS1"));
	
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
    			// If the command has to be executed in background :$
				if (background(command, size) == 1){
					int pid;
					if ((pid = fork()) == -1){
						perror("fork failed ");
					}

					// Child process will execute in background
					if (pid == 0){
						execute((char***)command, 0, STDIN_FILENO);
						exit(1);
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

    			free(command);
    		}

    	}
	}

	return 0;
}