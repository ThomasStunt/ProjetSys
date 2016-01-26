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
  const char *message_bienvenue = "Bienvenue a vous !\nVous venez de vous connecter sur notre serveur. Nous vous souhaitons de bien profiter de votre visite sur notre serveur.\nSi vous avez une reclamation ou toute autre suggestion, veuillez envoyer un email aux createurs (vous trouverez dans le fichier 'AUTHORS.md' les adresses mails)\n";
  const char *buf = malloc(128*sizeof(char));
  sleep(1);
  write(sock_client, message_bienvenue, strlen(message_bienvenue));
  while(1)
    {
      if(read(sock_client, buf, sizeof(buf)) == -1)
	{
	  perror("read client");
	}
      write(sock_client, buf, strlen(buf));
    }
  close(sock_client);
  close(sock_serveur);
}
