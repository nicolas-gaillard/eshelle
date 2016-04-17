#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "execution.h"
#include "automate.h"
#include "decoupe.h"

#define SIZE_ANSWER 512
#define PORT 1560
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

int background(char** cmd[], int size){
  if (strcmp(cmd[size - 1][0], "&")==0){
    cmd[size - 1] = NULL;
    return 1;
  }
  else{
    return 0;
  }
}

void sendToClient(char*** command, int sock){

  // Command's result will be writtent in a temporary file : tmp.txt
  FILE* ff = freopen("tmp.txt", "a+", stdout);
  // Execution of th ecommand
  execute((char***)command, 0, STDIN_FILENO);

  if (ff != NULL)
  {
    // Each line can contain 512 char
    char line[SIZE_ANSWER];

    // Read a line
    while (fgets (line, sizeof(line), ff) != NULL ) 
    {
      // Write the line to the client
      write(sock, line, sizeof(line));
    }
    fclose (ff);
    unlink("tmp.txt");
  }
  else
  {
    perror("Can't open the file "); 
  }
}

void doprocessing (int sock) {
  int n;
  char buffer[256];
  bzero(buffer,256);
  // We read the message sent by the client 
  n = read(sock,buffer,255);

  // If we can't read
  if (n < 0) {
    perror("Impossible de lire depuis le socket");
    exit(0);
  }

  if (automate(buffer) == 1){
    // If the command is ok :
    int size;
    // We cut the command
    char *** command = decoupe(buffer, &size);

    int pid;
    // Creation of a child process to execute the command
    if ((pid = fork()) == -1){
      perror("fork failed ");
    }

    // Child process :
    if (pid == 0){
      sendToClient(command, sock);
      exit(0);
    }

    // Parent process :
    else{
      // If the command has to be executed in background : &
      if (background(command, size) == 0){
        // The command has not to be executed in background
        // We wait the end of the child process
        wait(NULL);
      }
      close(sock);
    }

    // We release the space
    free(command);

  /*
  if (n < 0) {
    perror("Impossible d'écrire vers le socket");
    exit(0);
    }
  */
  }
}


int main(void){

  SOCKET sock;
  SOCKADDR_IN sin;
  SOCKET csock;
  SOCKADDR_IN csin;
  char buffer[32] = "";
  socklen_t recsize = sizeof(csin);
  int sock_err, pid;

  sock = socket(AF_INET, SOCK_STREAM, 0);

  // The socket is valid
  if(sock != INVALID_SOCKET)
  {
    printf("La socket %d est maintenant ouverte en mode TCP/IP\n", sock);

    /* Configuration */
    sin.sin_addr.s_addr    = htonl(INADDR_ANY);   /* @IP automatic */
    sin.sin_family         = AF_INET;             /* IP Protocol */
    sin.sin_port           = htons(PORT);         /* Listing the port */
    sock_err = bind(sock, (SOCKADDR*)&sin, sizeof(sin));

    // If the socket is OK
    if(sock_err != SOCKET_ERROR)
    {
      /* Listing's start (server mode) */
      sock_err = listen(sock, 5);
      printf("Listage du port %d...\n", PORT);

      /* The socket is OK */
      if(sock_err != SOCKET_ERROR)
      {
        /* Waiting, while the client is connecting */
        printf("Patientez pendant que le client se connecte sur le port %d...\n", PORT);

        // We wait connexions from clients
        while(1){

          // We accept to connect
          csock = accept(sock, (SOCKADDR*)&csin, &recsize);
          printf("Un client se connecte avec la socket %d de %s:%d\n", csock, inet_ntoa(csin.sin_addr), htons(csin.sin_port));
          close(sock);
          doprocessing(csock);

          // We close the connection (both way)
          shutdown(csock, 2);
        }
      }
    }

    /* Fermeture de la socket */
    printf("Fermeture de la socket...\n");
    closesocket(sock);
    printf("Fermeture du serveur terminee\n");
  }

  return EXIT_SUCCESS;
}
