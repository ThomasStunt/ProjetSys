#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <signal.h>

const char *message_bienvenue = "Bienvenue a vous !\nVous venez de vous connecter sur notre serveur. Nous vous souhaitons de bien profiter de votre visite sur notre serveur.\nSi vous avez une reclamation ou toute autre suggestion, veuillez envoyer un email aux createurs (vous trouverez dans le fichier 'AUTHORS.md' les adresses mails)\nCe serveur est pour l'instant pour un client unique.\nVous pouvez communiquer avec le serveur, il vous renverra votre propre message.\nPassez une bonne journee.\nThomas PERRIER et Benjamin DUCAUROY\n\n";

/* Traitement des signaux, affiche la valeur du signal */
void traitement_signal(int sig)
{
  printf("signal : %d\n", sig);
  waitpid(-1, NULL, 0);
}

/* Gestion des signaux du serveur : SIGCHLD (pour le moment) */
void initialiser_signaux(void) {
  struct sigaction sa ;
  sa.sa_handler = traitement_signal ;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART ;
  if (sigaction(SIGCHLD, &sa, NULL) == -1){
    perror("sigaction(SIGCHLD)");
  }
}

/* Création du serveur (création d'une socket, paramétrage et écoute des connexions */
int creer_serveur(int port)
{
  /* Création sockets */
  int sock_serveur = socket(AF_INET, SOCK_STREAM, 0);
  if(sock_serveur == -1)
    {
      perror("Socket serveur");
      close(sock_serveur);
      return -1;
    }

  int sockClient;
  
  /* Paramétrage serveur */ 
  struct sockaddr_in saddr;
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(8080);
  saddr.sin_addr.s_addr = INADDR_ANY;
  
  int optval = 1;
  if(setsockopt(sock_serveur, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
    {
      perror("Can not set SO_REUSEADDR option");
      return -1;
    }

  initialiser_signaux();

  if(bind(sock_serveur, (struct sockaddr *)&saddr, sizeof(saddr)) == -1)
    {
      perror("bind socket_serveur");
      return -1;
    }

  /* Ecoute des connexions */
  if(listen(sock_serveur, 10) == -1)
    {
      perror("listen socket_serveur");
      return -1;
    }
  
  /* Création processus fils pour chaque client */
	while(1)
{
    if((sockClient = accept(sock_serveur, NULL, NULL)) == -1)
      {
	perror("Accept socket_client");
	return -1;
      }
    
    if(fork() == 0)
      {
	write(sockClient, message_bienvenue, strlen(message_bienvenue));
	close(sock_serveur);
	int readed;
	char buf[1024];
	while(1)
	  {
	    readed = read(sockClient, buf, 1024*sizeof(char));
	    if(readed == 0)
	      {
		perror("Read client");
		return 0;
	      }
	    else
	      {
		//printf("%s\n", buf);
		write(sockClient, buf, readed*sizeof(char));
	      }
	  }
      }
  close(sockClient);
}
  close(sock_serveur);
  return sock_serveur;
}


