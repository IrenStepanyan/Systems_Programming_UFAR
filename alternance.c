#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdbool.h>

#include "../erreur.h"
#include "alternance.h"

bool tourRecu = false; // mise à jour par handler 
sigset_t ens; // pour sigsuspend en attente de SIGUSR1

void traiterSignal(int sig) {
  tourRecu = true;
}

void attendreTour() {
  while (!tourRecu)
    sigsuspend(&ens);   
}

void donnerTour(pid_t aQui) {
  tourRecu = false;
  if (kill(aQui, SIGUSR1) != 0)
    erreur("Echec kill", 100);
}
  
void traitementPremier(pid_t pidSecond, const char *mess) {
  pid_t myPid = getpid();
  while(1) {
    printf("Processus %s de pid %d s'execute\n", mess, myPid);
    sleep(1);
    printf("Processus %s de pid %d donne le tour a %d\n", mess, myPid, pidSecond);
    donnerTour(pidSecond);
    attendreTour();
  }
}

void traitementSecond(pid_t pidPremier, const char *mess) {
  pid_t myPid = getpid();
  
  while(1) {
    attendreTour();
    printf("Processus %s de pid %d s'execute\n", mess, myPid);
    sleep(1);
    printf("Processus %s de pid %d donne le tour a %d\n", mess, myPid, pidPremier);
    donnerTour(pidPremier);
  }
}
  
void initialiserAlternance() {
  struct sigaction action;
  
  sigprocmask(SIG_SETMASK, NULL, &ens);         /* Recuperation masque */
  sigaddset(&ens, SIGUSR1);                /* Preparation masque temporaire*/
  if (sigprocmask(SIG_SETMASK, &ens, NULL) != 0)
    erreur ("Echec sigprocmak", 1);             /* Installation */
  sigdelset(&ens, SIGUSR1);                 

  action.sa_handler = traiterSignal;
  action.sa_flags = 0;
  sigemptyset(&action.sa_mask);
  if (sigaction(SIGUSR1, &action, NULL) != 0)
    erreur("echec sigaction", 2);
}

