#define _GNU_SOURCE
#define BUFFER_KEYBOARDING 100
#define BUFFER_PROMPT 30
#define NUMBER_FONCTIONS 18
#define SIZE_ARG 95
#define SIZE_CMD 5

char *functions[NUMBER_FONCTIONS];

void clean(const char *buffer, FILE *fp);

void clear();

void prompt(char *currentDir, char *hostName);

int exist(char *c, char *t[]);

