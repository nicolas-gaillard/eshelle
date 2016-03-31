#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define PORT 2000
// We begin at 2000, because port 1 to 1024 are reserved (2000 is a random choice)

// To close a socket, use the function close()
// WARNING, socket is only working on Linux

// WARNING, how to manage port

typedef int SOCKET;
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

int main(int argc, char const *argv[]) {
  // Serveur socket :

  /*
  * AF_INET for TCP protocol
  * SOCK_STREAM for TCP protocol
  * 0 for TCP protocol
  */

  SOCKET sock;
  char buffer[100] = "";
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET){
    printf("Error while creating the socket. \n");
    exit(0);
  }
  //signal(SIGINT, fin);

  // Socket's settings :
  sockaddr_in sockServIn;             // Automatic adress
  sockServIn.sin_family = AF_INET;    // TCP protocol
  /*
  INADDR_ANY = allowed us to work without knowing the IP adress of the machine
  If we want to specify an adress : inet_addr("127.0.0.1")
  */
  sockServIn.sin_addr.s_addr = htonl(INADDR_ANY);
  sockServIn.sin_port = htons(PORT);

  // Client socket :
  SOCKET clientSock;
  sockaddr_in clientSockAddr;
  socklen_t clientSize = sizeof(clientSockAddr);

  // Linking the socket and the struct :
  if (bind(sock, (sockaddr*)&sockServIn, sizeof(sockServIn)) != SOCKET_ERROR){
    printf("Waiting a connection with the client on the port : %d \n", PORT);
    listen(sock, 5);
    // We are listenning 5 computers at the same time
  }
  else{
    perror("Error while binding");
    close(sock);
    exit(0);
  }

  // Connection with a client :
  if ((clientSock = accept(sock, (sockaddr*)&clientSockAddr, &clientSize)) != SOCKET_ERROR){
    printf("Client is connecting from %s on port : %d \n", inet_ntoa(clientSockAddr.sin_addr), htons(clientSockAddr.sin_port));

    if (recv(sock, buffer, sizeof(buffer), 0) != SOCKET_ERROR){
      printf("Command received");
        // Traitement des commandes
      }
    else{
      perror("Command not reveived");
      shutdown(clientSock, 2);
      close(clientSock);
      close(sock);
      exit(0);
      }
  }
  /*
  Est ce qu'on remplace par :
  while((clientSock = accept(sock, (sockaddr*)&clientSockAddr, &clientSize)) != SOCKET_ERROR))
  ou while() <-- chercher une bonne condition d'arrêt, ex quand le client envoie quit, on change
  un boolean et qui nous fait sortir de la boucle et qui donc quitte tout le protocole
  */
  else {
    perror("Unable to connect");
    close(sock);
    exit(0);
  }

shutdown(clientSock, 2);
close(clientSock);
close(sock);
exit(0);
}