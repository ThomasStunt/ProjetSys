#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "socket.h"

int creer_serveur(int port)
{
  int sock_serveur = socket(AF_INET,SOCK_STREAM,0);
  if(sock_serveur == -1)
    {
      perror("Socket serveur");
    }

  struct sockaddr_in saddr;
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(8080);
  saddr.sin_addr.s_addr = INADDR_ANY;

  if(bind(sock_serveur,(struct sockaddr *)&saddr, sizeof(saddr)) == -1)
    {
      perror("bind socket_serveur");
    }

  if(listen(sock_serveur,10) == -1)
    {
      perror("listen socket serveur");
    }
}
