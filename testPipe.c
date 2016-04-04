#include <stdio.h>
#include <memory.h>
#include <unistd.h>

// Pour les execs : 
// https://openclassrooms.com/courses/la-programmation-systeme-en-c-sous-unix/executer-un-programme-1

int testPipe(char *cmdBeforePipe, char *cmdAfterPipe, char *argv1, char *argv2){
	// Create the pipe which will "connect" the two commands
	int interim[2];

	if (pipe(interim) == -1){
		printf("Impossible to create the pipe");
		return 1;
	}
	
	int pid;
	if ((pid = fork()) == -1){
		printf("Fork failed");
		return 1;
	}

	if (pid == 0){ // Child process
		// We close the write side
		close(interim[1]);
		// We connect the read side to stdin
		dup2(interim[0], 0);
		close(interim[0]);

		// Execution of the command after the pipe
		// Faire une condition d'erreur 
		execlp(cmdAfterPipe, argv2);
	}
	else { // Parent process
		// We close the read side
		close(interim[0]);
		// We connect the write side to stdout
		dup2(interim[1],1);
		close(interim[1]);

		// Execution of the command before the pipe
		execlp(cmdBeforePipe, argv1);

	}
	return 0;
}

int main(int argc, char const *argv[])
{
	//char *test[] = {"-doc"};
	//char *test = "-doc";
	char *c1 = "/bin/ls";
	char *c2 = "wc";
	testPipe(c1, c2, "NULL", "NULL");
	return 0;
}