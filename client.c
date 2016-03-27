#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

// Note : we have to launch server before client.

// Besoin de <netinet/in.h> ? --> definitions of structures needed for sockets

// recv pour envoyer des messages au serveur

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define IP "127.0.0.1"
#define PORT 2000

typedef int SOCKET;
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

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

int main(int argc, char const *argv[]) {

  SOCKET sockClient;
  sockaddr_in sockClientIn;
  char buffer[100]="";

  if ((sockClient = socket(AF_INET, SOCK_STREAM, 0)) != INVALID_SOCKET){
    sockClientIn.sin_addr.s_addr = inet_addr(IP);
    sockClientIn.sin_family = AF_INET;
    sockClientIn.sin_port = htons(PORT);
  }
  else{
    perror("Error while creating the socket");
    exit(0);
  }

  if ((connect(sockClient, (sockaddr*)&sockClientIn, sizeof(sockClientIn)) != SOCKET_ERROR)){
    printf("Client is connecting from %s on port : %d \n", inet_ntoa(sockClientIn.sin_addr), htons(sockClientIn.sin_port));
  }
  else {
    perror("Unable to connect");
    close(sockClient);
    exit(0);
  }

  while (strcmp(buffer,"quit") != 0){
    // #!./shell <-- shell launcher
    fgets(buffer, sizeof(buffer), stdin);
    clean(buffer, stdin);

    send(sockClient, buffer, sizeof(buffer), 0);
  }


  // Attemps to connect to the server :
  close(sockClient);
  return 0;
}
