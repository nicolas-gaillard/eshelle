#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PORT 23
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

void doprocessing (int sock) {
   int n;
   char buffer[256];
   bzero(buffer,256);
   //On lit le message envoyé par le client sur le socket
   n = read(sock,buffer,255);
   
   //Si on ne peut pas lire depuis le socket
   if (n < 0) {
      perror("Impossible de lire depuis le socket");
      exit(1);
   }
   
   // On ecrit dans le socket "message recu", donc le client recevra ce message en utilisant recv
   printf("Message : %s\n",buffer);
   n = write(sock,"Message reçu",18);
   
   if (n < 0) {
      perror("Impossible d'écrire vers le socket");
      exit(1);
   }
    
}


int main(void)
{
 
    SOCKET sock;
    SOCKADDR_IN sin;
    SOCKET csock;
    SOCKADDR_IN csin;
    char buffer[32] = "";
    socklen_t recsize = sizeof(csin);
    int sock_err, pid;   

    sock = socket(AF_INET, SOCK_STREAM, 0);

    /* Si la socket est valide */
    if(sock != INVALID_SOCKET)
    {
        printf("La socket %d est maintenant ouverte en mode TCP/IP\n", sock);

        /* Configuration */
        sin.sin_addr.s_addr    = htonl(INADDR_ANY);   /* Adresse IP automatique */
        sin.sin_family         = AF_INET;             /* Protocole familial (IP) */
        sin.sin_port           = htons(PORT);         /* Listage du port */
        sock_err = bind(sock, (SOCKADDR*)&sin, sizeof(sin));

        /* Si la socket fonctionne */
        if(sock_err != SOCKET_ERROR)
        {
            /* Démarrage du listage (mode server) */
            sock_err = listen(sock, 5);
            printf("Listage du port %d...\n", PORT);

            /* Si la socket fonctionne */
            if(sock_err != SOCKET_ERROR)
            {
                /* Attente pendant laquelle le client se connecte */
                printf("Patientez pendant que le client se connecte sur le port %d...\n", PORT);        

                //boucle infini pour attendre les connexions clients
                while(1){

                    // on accepte de se connecter avec un client
                    csock = accept(sock, (SOCKADDR*)&csin, &recsize);
                    printf("Un client se connecte avec la socket %d de %s:%d\n", csock, inet_ntoa(csin.sin_addr), htons(csin.sin_port));

                    // on créé un processus pour ce client
                    pid = fork();

                    if(pid < 0){
                        printf("Erreur de fork");
                        exit(1);
                    }

                    // ce processus fils va appeler la méthode de lecture puis renvoie d'un message de "bonne reception"
                    if(pid == 0){
                        //on forme l'ancien socket, maintenant on utilise le csock issue de accept()
                        close(sock);
                        doprocessing(csock);
                        exit(0);
                    }
                    else{
                        close(csock);
                    }

                    // Il ne faut pas oublier de fermer la connexion (fermée dans les deux sens)
                    shutdown(csock, 2);
                }
            }
        }

        /* Fermeture de la socket */
        printf("Fermeture de la socket...\n");
        closesocket(sock);
        printf("Fermeture du serveur terminee\n");
    }
    
    //Non utilisé dans le cas de la boucle while(1) 
    //On attend que l'utilisateur tape sur une touche, puis on ferme
    //getchar();
 
    return EXIT_SUCCESS;
}