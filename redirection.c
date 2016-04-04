#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>


// Ne pas oublier de define des constantes pour chaque commandes
// Ex : 
// define "path de ls avec librarie" ls 

// Voir comment récupérer la taille du tableau de char en C

// Faire une fonction qui rajoute un null dans un tableau ou voir l'automate

// Faire une fonction qui retourne le path de la fonction (déduis en fonction
// des libs)pour appeler proprement la fonction execute

//#define INDEX_TO_SHIFT 0

// Free besoin que dans les mallocs

#define SUCCESS 0
#define ERROR 1
#define ERROR_EXEC -1
#define SIZE_BUFFER 30

int hereCommands(char* redirection[]); 

int executeHereCommands(char* redirection[], char *argCmd[]){
	// On utilise write dans une zone mémoire
	// Une fois le délimiteur atteint
	// On connecte la sortie avec le read du processus fils responsable
	// de l'execution de la commande 

	int input = hereCommands(redirection);

	char *cmd = argCmd[0];

	int pid = fork();

  	if (pid == 0){
 
  		dup2(input, STDIN_FILENO);
		if (execv(cmd, argCmd) != ERROR_EXEC){
			exit(0);	
		}
		else {
			printf("Can't execute the command\n");
			return ERROR_EXEC;  	
		}
	}
	else{
		wait(NULL);
	}
	return 0;

	// Dans le processus père
	// Creating a pipe 
	
	// Dans le fils : 
	// dup2(descripteur[1], STDIN_FILENO);
}

int execute(char* argCmd[]){
	// Appel de la fonction qui va chercher dans les libraries
	// cmd = fonction()

	// Saving the command 
	char *cmd = argCmd[0];

	/*
	Si val a pas mis le null :
	char **argCmdComplete = malloc(sizeof(argCmd)+sizeof(NULL))
	Copy of the array argCmd in argCmdComplete
	memmove(&argCmdComplete[0], &argCmd[0], size);
	Addition of the (char*)NULL to use execv
	argCmdComplete[size] = (char*)NULL;
	*/

	// Executing the command

	int pid = fork();
  	if (pid == 0){
		if (execv(cmd, argCmd) != -1){
			exit(0);
		}
		else {
			printf("Can't execute the command\n");
			return ERROR_EXEC;
		}
	}
  	else {
    	wait(NULL);
  	}
  	return 0;
}

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



// Background redirection : 
// The father process doesn't wait the end of the child process to treat other commands
// "&" is always at the end of the command :
int backgroundExecute(char *argCmd[]){
	char *cmd = argCmd[0];

	int pid = fork();
  	if (pid == 0){
		if (execv(cmd, argCmd) != ERROR_EXEC){
			exit(0);	
		}
		else {
			printf("Can't execute the command\n");
			return ERROR_EXEC;  	
		}
	}
	return 0;
}

int andRedirection(char *cmdBeforeAnd[], char *cmdAfterAnd[]){
	if (execute(cmdBeforeAnd) == SUCCESS){
		return execute(cmdAfterAnd);
	}
	else {
		return ERROR;
	}
}

int orRedirection(char *cmdBeforeAnd[], char *cmdAfterAnd[]){
	if (execute(cmdBeforeAnd) == ERROR_EXEC){
		return execute(cmdAfterAnd);
	}
	else{
		printf("Command 1 has been executed \n");
		return SUCCESS;
	}
}



int whatsThisRedirection(char *arg[]){
	// On teste si c'est une redirection
	// Sinon on execute simplement la commande
	if (strcmp(arg[0],">") == 0){
		freopen(arg[1], "w", stdout);
		return SUCCESS;
	}
	else if (strcmp(arg[0], ">>") == 0){
		freopen(arg[1], "a", stdout);
		return SUCCESS;
	}
	else if (strcmp(arg[0], "<") == 0){
		freopen(arg[1], "r", stdin);
		return SUCCESS;
	}
	else if (strcmp(arg[0], "<<") == 0){
		int input = hereCommands(arg);
		return input;
		// On écrit notre truc dans un fichier tant qu'on a pas le delimiter
		// Une fois le delimiter coupé,
		// On ouvre le fichier en reading
	}
	// else if (strcmp(arg[i], "||") == 0)
	// else if (strcmp(arg[i], "&&") == 0)
	// else if (strcmp(arg[i], "&") == 0)
	// else if (strcmp(arg[i], ";") == 0)
	else {
		return ERROR;
	}
}

int hereCommands(char* redirection[]){
	int descriptor[2];
	if (pipe(descriptor) != 0){
		perror("Can't create the pipe");
	}

	char delimiter[SIZE_BUFFER] = {""};

	while(delimiter != redirection[1]){
		printf("> ");
		fgets(delimiter, sizeof(delimiter), stdin);
   		clean(delimiter, stdin);
   		if (delimiter != redirection[1]){
   			write(descriptor[1], delimiter, SIZE_BUFFER);
   		}
	}
	return descriptor[1];
}

int pipeRedirection(char *cmdBeforePipe[], char *cmdAfterPipe[]){
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
		if (execute(cmdAfterPipe) == ERROR_EXEC){
			printf("Can't execute the command\n");
			return -1;
		}
	}
	else { // Parent process
		// We close the read side
		close(interim[0]);
		// We connect the write side to stdout
		dup2(interim[1],1);
		close(interim[1]);

		// Execution of the command before the pipe
		if (execute(cmdBeforePipe) == ERROR_EXEC){
			printf("Can't execute the command\n");
			return -1;
		}

	}
	return 0;
}

int main(int argc, char const *argv[])
{
	
	char *cmd[] = {"ls", "-l", (char*)NULL};
	//char *fic = "toto.txt";
	//char const *truc[] = {"-l"};

	//char *arg[] = {c, optC};
	//char *test[] = {">", fic};

	//whatsThisRedirection(test);
	//execute(cmd, 2);

	execvp("ls", cmd);

	return 0;
}