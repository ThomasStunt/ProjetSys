#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <stdio.h>
#include "socket.h"
#include <signal.h>

const char *message_bienvenue = "Bienvenue a vous !\nVous venez de vous connecter sur notre serveur. Nous vous souhaitons de bien profiter de votre visite sur notre serveur.\nSi vous avez une reclamation ou toute autre suggestion, veuillez envoyer un email aux createurs (vous trouverez dans le fichier 'AUTHORS.md' les adresses mails)\nCe serveur est pour l'instant pour un client unique.\nVous pouvez communiquer avec le serveur, il vous renverra votre propre message.\nPassez une bonne journee.\nThomas PERRIER et Benjamin DUCAUROY\n\n";

void traitement_signal(int sig)
{
  printf("signal : %d\n", sig);
  waitpid(-1,NULL,0);
}

void initialiser_signaux(void)
{

  struct sigaction sa;
  sa.sa_handler = traitement_signal;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_NOCLDWAIT;
  if(signal(SIGCHLD, SIG_IGN) == SIG_ERR)
    {
      perror("SIGCHLD");
    }
}

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

  initialiser_signaux();

  if(bind(sock_serveur, (struct sockaddr *)&saddr, sizeof(saddr)) == -1)
    {
      perror("bind socket_serveur");
    }
  
  if(listen(sock_serveur, 5) == -1)
    {
      perror("listen socket_serveur");
    }

  return sock_serveur;
}

int accept_client(int socketServ) {

  int sockClient = 0;
  
  if((sockClient = accept(socketServ, NULL, NULL)) == -1)
    {
      perror("Accept socket_client");
    }

  FILE * client = fdopen(sockClient,"w+");
  
  if(fork() == 0)
    {
      char buf[128];
      fprintf(client, message_bienvenue);
      char *c;
      while(1)
	{ 
	  if((c = fgets(buf,sizeof(buf),client)) == NULL)
	    {
	      perror("fgets");
	      return -1;
	    }
	  fprintf(client,"ProjetSys : %s\n", buf);
	}
      fclose(client);
      exit(0);
    }
  close(sockClient);

  return 0;
}


