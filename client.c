#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "shell.h"

#define PORT 1560
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

int client(){
    SOCKET sock;
    SOCKADDR_IN sin;
    char buffer[32] = "";
    int sock_err;
    char currentDir[BUFFER];
    char hostName[BUFFER];

    /* Creation of the socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);

    /* Socket's configuration */
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);

    // If we connect
    if(connect(sock, (SOCKADDR*)&sin, sizeof(sin)) != SOCKET_ERROR)
    {
        clear();
        printf("Connection à %s sur le port %d\n", inet_ntoa(sin.sin_addr), htons(sin.sin_port));
        // Welcome message
        printf("Shell v1.0 \nEnter \"quit\" to leave\n");

        // Displaying the prompt
        prompt(currentDir, hostName);
        // The client writes his command
        fgets(buffer, sizeof buffer, stdin);

        // We send it to the socket
        sock_err = send(sock, buffer, 32, 0);

        // Acknowledgement of receipt
        if(recv(sock, buffer, 32, 0) != SOCKET_ERROR){
            printf("Recu : %s\n", buffer);
        }

    }
    // If not : we can't connect
    else
    {
        printf("Impossible de se connecter\n");
    }

    // Closing the socket
    closesocket(sock);

    // We wait the user write a char, then we close
    getchar();

    return EXIT_SUCCESS;
}

int main(void)
{
    //while(1){
        client();
    //}
}
