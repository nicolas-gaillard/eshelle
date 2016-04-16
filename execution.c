#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include "execution.h"

static char *functions[NUMBER_FONCTIONS] = 
{"ls", "mkdir", "cd", "pwd", "cat", "more", "less", "mv", "cp", "rm", "du", "chown",
"chgrp", "echo", "ps", "top", "su", "wc"};

static char *redirections[NUMBER_REDIRECTION] =
{"<", ">", ">>", "<<", "&&", "||", "|"};



void clean(const char *buffer, FILE *fp){
    char *p = strchr(buffer,'\n');
    if (p != NULL)
        *p = 0;
    
    else
    {
        int c;
        while ((c = fgetc(fp)) != '\n' && c != EOF);
    }
}

int whatsThisRedirection(char *arg[]){
	if (strcmp(arg[0],">") == 0){
		//freopen(arg[1], "w", stdout);
		return SIMPLE_OUT_REDIRECTION;
	}
	else if (strcmp(arg[0], ">>") == 0){
		//freopen(arg[1], "a", stdout);
		return OUT_REDIRECTION;
	}
	else if (strcmp(arg[0], "<") == 0){
		//freopen(arg[1], "r", stdin);
		return  IN_REDIRECTION;
	}
	else if (strcmp(arg[0], "<<") == 0){
		return HERE_COMMANDS;
		// On écrit notre truc dans un fichier tant qu'on a pas le delimiter
		// Une fois le delimiter coupé,
		// On ouvre le fichier en reading
	}
	else if (strcmp(arg[0], "|") == 0){
		return PIPE;
	}
	else if (strcmp(arg[0], "||") == 0){
		return OR;
	}
	else if (strcmp(arg[0], "&&") == 0){
		return AND;
	}
	else {
		return 0;
	}
}

int exist(char *cmd, char *functions[]){
	int i;
	for (i = 0; i < NUMBER_REDIRECTION; ++i)
	{
		if (strcmp(cmd, functions[i]) == 0){
			return 0;
		}
	}
	return ERROR;
}

// This function will close a descriptor 
void closePipe(int closed){
	//do{
		// Closing failed
		if (close(closed) != 0){
			perror("Closing failed ");
		}
	//}
	//while(0);
}

// This function move the old file descriptor into the new
void redirectFD(int oldFd, int newFd){
	if (newFd != oldFd){
		if (dup2(oldFd, newFd) == -1){
			perror("Redirection failed ");
		}
		else{
			// the old is useless now, we can close it
			closePipe(oldFd);
		}
	}
}


// <<
int hereCommands(char* redirection[]){
	// This is the buffer which will contain the text 
	char buffer[SIZE_BUFFER] = {""};

	// We open a temporary file 
	FILE* tmp = tmpfile();
	while(1){
		printf("> ");
		fgets(buffer, sizeof(buffer), stdin);
		clean(buffer, stdin);
		if (strcmp(buffer, redirection[1]) == 0){
   			break;
   		}
   		// clean() removes \n, we have to add it to have a good display
   		strcat(buffer, "\n");
   		fputs(buffer, tmp);
	}
	// We go back to the beginning of the file
	rewind(tmp);
	// Fileno returns the file descriptor of the temporary file
	return fileno(tmp);
}

void hereExecute(char** commands[], int position){
	int in = hereCommands(commands[position+1]);

	// Creation of a pipe
	int pipeFD[2];
	if (pipe(pipeFD) != 0){
		perror("pipe failed ");
	}
	// Creation of a child process
	int pid;
	if ((pid = fork()) == -1){
		perror("fork failed ");
	}
	// Child process :
	if (pid == 0){
		closePipe(pipeFD[0]);
		redirectFD(in, STDIN_FILENO);

		if (commands[position+2] != NULL){
			redirectFD(pipeFD[1], STDOUT_FILENO);
		}

		execvp(commands[position][0], commands[position]);
		perror("exec failed ");
	}
	// Parent process
	else{
		wait(NULL);
		closePipe(pipeFD[1]);
		closePipe(in);
		execute(commands, position+2, pipeFD[0]);
	}

}

// |
void pipeExecute(char** commands[], int position, int inFD){
	// Creation of a pipe
	int pipeFD[2];
	if (pipe(pipeFD) != 0){
		perror("pipe failed ");
	}
	// Creation of a child process
	int pid;
	if ((pid = fork()) == -1){
		perror("fork failed ");
	}
	// Child process :
	if (pid == 0){
		closePipe(pipeFD[0]);
		redirectFD(inFD, STDIN_FILENO);
		redirectFD(pipeFD[1], STDOUT_FILENO);
		execvp(commands[position][0], commands[position]);
		perror("exec failed ");
	}
	// Parent process
	else{
		wait(NULL);
		closePipe(pipeFD[1]);
		closePipe(inFD);
		execute(commands, position+2, pipeFD[0]);
	}
}

// >
void outSimpleExecute(char** commands[], int position, int inFD){
	// We can replace F_OK by W_OK to check if we can write in the file ???
	if (access(commands[position+1][1], F_OK) != -1){
		// The file exists, we remove it 
		unlink(commands[position+1][1]);
	}
	outExecute(commands, position, inFD);
}

// >>
void outExecute(char** commands[], int position, int inFD){
	// Creation of a pipe
	int pipeFD[2];
	if (pipe(pipeFD) != 0){
		perror("pipe failed ");
	}
	// Creation of a child process
	int pid;
	if ((pid = fork()) == -1){
		perror("fork failed ");
	}

	// Child process :
	if (pid == 0){
		closePipe(pipeFD[0]);
		redirectFD(inFD, STDIN_FILENO);

		// First, we open the file
		int fdOUT;
		if ((fdOUT = open(commands[position+1][1], O_RDWR|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR)) == -1){
			perror("Can't open the file ");
		}
		// We 
		redirectFD(fdOUT, STDOUT_FILENO);
		execvp(commands[position][0], commands[position]);
		perror("exec failed ");
		redirectFD(pipeFD[1], STDOUT_FILENO);
		if (fdOUT != -1){
			close(fdOUT);
		}

		// Then we
	}
	// Parent process
	else{
		wait(NULL);
		closePipe(pipeFD[1]);
		closePipe(inFD);
		execute(commands, position+2, pipeFD[0]);
	}	
}

// <
void inExecute(char** commands[], int position){
	int fdIN;
	if ((fdIN = open(commands[position+1][1], O_RDONLY)) == -1){
		perror("Can't open the file ");
		// ????
		exit(0);
	}

	// Creation of a pipe
	int pipeFD[2];
	if (pipe(pipeFD) != 0){
		perror("pipe failed ");
	}
	// Creation of a child process
	int pid;
	if ((pid = fork()) == -1){
		perror("fork failed ");
	}
	// Child process :
	if (pid == 0){
		closePipe(pipeFD[0]);
		redirectFD(fdIN, STDIN_FILENO);

		if (commands[position+2] != NULL){
			redirectFD(pipeFD[1], STDOUT_FILENO);
		}

		execvp(commands[position][0], commands[position]);
		perror("exec failed ");
	}
	// Parent process
	else{
		wait(NULL);
		closePipe(pipeFD[1]);
		close(fdIN);
		execute(commands, position+2, pipeFD[0]);
	}
}

// &&
void andExecute(char** commands[], int position, int inFD){

	// Creation of a pipe
	int pipeFD[2];
	if (pipe(pipeFD) != 0){
		perror("pipe failed ");
	}
	// Creation of a child process
	int pid;
	if ((pid = fork()) == -1){
		perror("fork failed ");
	}

	// Child process :
	if (pid == 0){
		closePipe(pipeFD[0]);
		redirectFD(inFD, STDIN_FILENO);
		if (commands[position+2] != NULL){
			redirectFD(pipeFD[1], STDOUT_FILENO);
		}

		// If the command failed
		if (execvp(commands[position][0], commands[position]) == -1){
			perror("exec failed ");
		}
	}
	// Parent process
	else{
		int status;
		wait(&status);
		
		closePipe(pipeFD[1]);
		closePipe(inFD);

		// The command wasn't a success, we execute the next one
		if (status > 0){
			execute(commands, position+2, STDIN_FILENO);

		}
		// The command wasn't a success, we do not execute the next after &&
		else{
			execute(commands, position+3, STDIN_FILENO);

		}
	}	
}

// ||
void orExecute(char** commands[], int position, int inFD){

	// Creation of a pipe
	int pipeFD[2];
	if (pipe(pipeFD) != 0){
		perror("pipe failed ");
	}
	// Creation of a child process
	int pid;
	if ((pid = fork()) == -1){
		perror("fork failed ");
	}

	// Child process :
	if (pid == 0){
		closePipe(pipeFD[0]);
		redirectFD(inFD, STDIN_FILENO);
		if (commands[position+2] != NULL){
			redirectFD(pipeFD[1], STDOUT_FILENO);
		}

		// If the command failed
		if (execvp(commands[position][0], commands[position]) == -1){
			perror("exec failed ");
		}
		
	}
	// Parent process
	else{
		int status;
		wait(&status);
		
		closePipe(pipeFD[1]);
		closePipe(inFD);

		// The command wasn't a success, we execute the next one
		if (status == 0){
			execute(commands, position+2, STDIN_FILENO);
		}
		// The command was a success, we do not execute the next after ||
		else{
			execute(commands, position+3, pipeFD[0]);
		}
	}	
}


void execute(char** commands[], int position, int inFD){
	// There is no command, we exit
	if (commands[position] == NULL){
		exit(0);
	}
	// This is a redirection
	else if (exist(commands[position][0], redirections) == 0){
		execute(commands, position+1, inFD);
	}
	// This is the last command to execute :
	else if (commands[position + 1] == NULL) {
		//|| whatsThisRedirection(commands[position+1]) == 0
		redirectFD(inFD, STDIN_FILENO);
		
		char final[]="./commands/execs/";
		strcat(final,commands[position][0]);
		int i=1;
		while(commands[position][i]!=NULL){
		    strcat(final," ");
		    strcat(final,commands[position][i]);
		    i++;
		}
		system(final);
		
		//execvp(commands[position][0], commands[position]);
		//perror("exec failed ");
	}
	// This is a commmand
	else {
		switch (whatsThisRedirection(commands[position+1])){
			case SIMPLE_OUT_REDIRECTION :
				outSimpleExecute(commands, position, inFD);
				break;

			case OUT_REDIRECTION :
				outExecute(commands, position, inFD);
				break;

			case PIPE :
				pipeExecute(commands, position, inFD);
				break;

			case AND :
				andExecute(commands, position, inFD);
				break;

			case OR :
				orExecute(commands, position, inFD);
				break;

			case HERE_COMMANDS :
				hereExecute(commands, position);
				//execute(commands, position, hereCommands(commands[position+1]));
				break;

			case IN_REDIRECTION :
				inExecute(commands, position);
				break;

			// There isn't redirection
			default :
				redirectFD(inFD, STDIN_FILENO);
				char *final="./commands/execs/";
        		strcat(final,commands[position][0]);
        		int i=1;
        		while(commands[position][i]!=NULL){
        		    strcat(final," ");
        		    strcat(final,commands[position][i]);
        		    i++;
        		}
        		printf("final = %s\n",final);
        		system(final);
				//execvp(commands[position][0], commands[position]);
				//perror("exec failed ");
				break;
		}

	}
}