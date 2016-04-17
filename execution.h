#define NUMBER_FONCTIONS 18
#define NUMBER_REDIRECTION 7
#define ERROR 1
#define SIZE_BUFFER 256

#define SIMPLE_OUT_REDIRECTION 2
#define HERE_COMMANDS 3
#define PIPE 4
#define AND 5
#define OR 6
#define OUT_REDIRECTION 7
#define IN_REDIRECTION 8

static char *functions[NUMBER_FONCTIONS];
static char *redirections[NUMBER_REDIRECTION];

void clean(const char *buffer, FILE *fp);

int whatsThisRedirection(char *arg[]);

int exist(char *cmd, char *functions[]);
void closePipe(int closed);
void redirectFD(int oldFd, int newFd);


int hereCommands(char* redirection[]);
void hereExecute(char** commands[], int position);
void pipeExecute(char** commands[], int position, int inFD);
void inExecute(char** commands[], int position);
void andExecute(char** commands[], int position, int inFD);
void orExecute(char** commands[], int position, int inFD);
void execute(char** commands[], int position, int inFD);
void outExecute(char** commands[], int position, int inFD);
void outSimpleExecute(char** commands[], int position, int inFD);
void execute(char** commands[], int position, int inFD);