#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <stdio.h>
#include "socket.h"

int creer_serveur(int port)
{
  int sock_serveur = socket(AF_INET, SOCK_STREAM, 0);
  int optval = 1;
  if(sock_serveur == -1)
    {
      perror("Socket serveur");
      close(sock_serveur);
      return -1;
    }
  
  struct sockaddr_in saddr;
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(8080);
  saddr.sin_addr.s_addr = INADDR_ANY;
  
  if(setsockopt(sock_serveur, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
    {
      perror("Can not set SO_REUSEADDR option");
    }

  if(bind(sock_serveur, (struct sockaddr *)&saddr, sizeof(saddr)) == -1)
    {
      perror("bind socket_serveur");
    }
  
  if(listen(sock_serveur, 5) == -1)
    {
      perror("listen socket_serveur");
    }

  int sock_client;
  sock_client = accept(sock_serveur, NULL, NULL);
  if(sock_client == -1)
    {
      perror("accept socket_client");
      close(sock_client);
      return -1;
    }
  const char *message_bienvenue = "Bienvenue a vous !\nVous venez de vous connecter sur notre serveur. Nous vous souhaitons de bien profiter de votre visite sur notre serveur.\nSi vous avez une reclamation ou toute autre suggestion, veuillez envoyer un email aux createurs (vous trouverez dans le fichier 'AUTHORS.md' les adresses mails)\nCe serveur est pour l'instant pour un client unique.\nVous pouvez communiquer avec le serveur, il vous renverra votre propre message.\nPassez une bonne journee.\nThomas PERRIER et Benjamin DUCAUROY\n\n";
  sleep(1);char buf[128];
  
  write(sock_client, message_bienvenue, strlen(message_bienvenue));
  while(1)
    {
      read(sock_client, buf, 128*sizeof(char));
      write(sock_client, buf, 128*sizeof(char));
    }
  close(sock_client);
  close(sock_serveur);
}
