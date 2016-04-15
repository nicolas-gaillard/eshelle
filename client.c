#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define PORT 23
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
 
int main(void)
{
 
    SOCKET sock;
    SOCKADDR_IN sin;
    char buffer[32] = "";
    int sock_err;
 
    /* Création de la socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);

    /* Configuration de la connexion */
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);

    /* Si l'on a réussi à se connecter */
    if(connect(sock, (SOCKADDR*)&sin, sizeof(sin)) != SOCKET_ERROR)
    {
        printf("Connection à %s sur le port %d\n", inet_ntoa(sin.sin_addr), htons(sin.sin_port));
        
        /* Si l'on reçoit des informations : on les affiche à l'écran */
        
        printf("Tapez une phrase : \n");
        fgets(buffer, sizeof buffer, stdin);
        
        sock_err = send(sock, buffer, 32, 0);
        /*
        if(recv(sock, buffer, 32, 0) != SOCKET_ERROR)
            printf("Recu : %s\n", buffer);
        */

    }
    /* sinon, on affiche "Impossible de se connecter" */
    else
    {
        printf("Impossible de se connecter\n");
    }

    /* On ferme la socket */
    closesocket(sock);
 
    /* On attend que l'utilisateur tape sur une touche, puis on ferme */
    getchar();
 
    return EXIT_SUCCESS;
}