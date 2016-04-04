#define SUCCESS 0
#define ERROR 1
#define ERROR_EXEC -1
#define SIZE_BUFFER 30

int hereCommandsExecute(char* redirection[], char *argCmd[]);

int execute(char* argCmd[]);

int backgroundExecute(char *argCmd[]);

int andExecute(char *cmdBeforeAnd[], char *cmdAfterAnd[]);

int orExecute(char *cmdBeforeAnd[], char *cmdAfterAnd[]);

int whatsThisRedirection(char *arg[]);

int hereCommands(char* redirection[]);

int pipeExecute(char *cmdBeforePipe[], char *cmdAfterPipe[]);