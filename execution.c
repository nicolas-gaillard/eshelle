#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define NUMBER_FONCTIONS 18
#define ERROR 1

#define SIMPLE_REDIRECTION 2
#define HERE_COMMANDS 3
#define PIPE 4
#define AND 5
#define OR 6
#define BACKGROUND 7

static char *functions[NUMBER_FONCTIONS] = 
{"ls", "mkdir", "cd", "pwd", "cat", "more", "less", "mv", "cp", "rm", "du", "chown",
"chgrp", "echo", "ps", "top", "su"};

void execute(char** commands[], int position, int inFD);

int whatsThisRedirection(char *arg[]){
	if (strcmp(arg[0],">") == 0){
		//freopen(arg[1], "w", stdout);
		return SIMPLE_REDIRECTION;
	}
	else if (strcmp(arg[0], ">>") == 0){
		//freopen(arg[1], "a", stdout);
		return SIMPLE_REDIRECTION;
	}
	else if (strcmp(arg[0], "<") == 0){
		//freopen(arg[1], "r", stdin);
		return  SIMPLE_REDIRECTION;
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
	else if (strcmp(arg[0], "&") == 0){
		return BACKGROUND;
	}
	else {
		return 0;
	}
}

int exist(char *cmd, char *functions[]){
	int i;
	for (i = 0; i < NUMBER_FONCTIONS; ++i)
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
		execute(commands, position+1, pipeFD[0]);
	}
}

void execute(char** commands[], int position, int inFD){
	// This is the last command to execute :
	if ((commands[position + 1] == NULL)) {
		//|| whatsThisRedirection(commands[position+1]) == 0
		redirectFD(inFD, STDIN_FILENO);
		execvp(commands[position][0], commands[position]);
		perror("exec failed ");
	}
	else {
		switch (whatsThisRedirection(commands[position+1]){
			case SIMPLE_REDIRECTION :
				break;

			case SIMPLE_REDIRECTION :
				break;

			case PIPE :
				pipeExecute(commands, position, inFD);
				break;

			case SIMPLE_REDIRECTION :
				break;

			case SIMPLE_REDIRECTION :
				break;

			case SIMPLE_REDIRECTION :
				break;

			case SIMPLE_REDIRECTION :
				break;

			// There isn't redirection
			default : 
				redirectFD(inFD, STDIN_FILENO);
				execvp(commands[position][0], commands[position]);
				perror("exec failed ");
		}

		pipeExecute(commands, position, inFD);
	}
}

int main(int argc, char const *argv[])
{
	char* cmd1[] = { "ls" , NULL, NULL };
	char* cmd2[] = { "grep", "sh", NULL };
	char* more[] = { "more", NULL };
	char** cmds[] = { cmd1, cmd2, more, NULL };

	execute((char***)cmds, 0, STDIN_FILENO);	
	return 0;
}