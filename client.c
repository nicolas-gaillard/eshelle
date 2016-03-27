#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

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

int main(int argc, char const *argv[]) {

  SOCKET sockClient;
  sockaddr_in sockClientIn;

  if ((sockClient = socket(AF_INET, SOCK_STREAM, 0)) != SOCKET_ERROR){
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


  // Attemps to connect to the server :
  close(sockClient);
  return 0;
}
