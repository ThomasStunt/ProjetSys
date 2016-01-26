#include <stdio.h>
#include <string.h>
#include "socket.h"

int main(int argc, char **argv)
{
  if(creer_serveur(8080) == -1)
    {
      perror("creation serveur");
    }
  return 0;
}
