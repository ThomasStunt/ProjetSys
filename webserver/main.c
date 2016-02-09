#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "socket.h"

int main(int argc, char **argv)
{
  int sockServ = creer_serveur(8080);
  
  if(sockServ == -1)
    {
      perror("creation serveur");
    }

  while(1)
    {
      if(accept_client(sockServ) == -1)
	{
	  perror("Accept");
	  exit(1);
	}
    }

  return 0;
}
