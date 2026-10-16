#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include "erreur.h"

int pingpong[2]; 
int pongping[2]; 

void pere(int N);
void fils(void);

int main(int argc, char * argv[]){
  if(argc != 2){
    erreur("Usage: ./pingpongv2 N", 1);
  }
  int N = atoi(argv[1]);
  if(N <= 0){
    erreur("Le nombre de ping pong doit etre superieur a 0", 1);
  }

  if(pipe(pingpong) == -1){
    erreur("Erreur lors de la creation du pipe pingpong", 2);
  }
  if(pipe(pongping) == -1){
    erreur("Erreur lors de la creation du pipe pongping", 3);
  }

  switch(fork()){
    case -1:
      erreur("Erreur lors du fork", 4);
    case 0:
      fils();
      exit(0);
    default:
      pere(N);
      break;
  }

  return 0;
}

void pere(int N){
  close(pingpong[0]);
  close(pongping[1]);

  int pid = getpid();
  int valeur = 0;

  for(int i = 0; i < N; i++){
    int ajout = rand() % 10;
    printf("Processus de pid %d : Ping [%d] (je vais ajouter %d)\n", pid, valeur, ajout);
    valeur += ajout;

    if(write(pingpong[1], &valeur, sizeof(valeur)) == -1){
      erreur("Erreur lors de l'ecriture dans le pipe pingpong", 5);
    }
    if(read(pongping[0], &valeur, sizeof(valeur)) <= 0){
      erreur("Erreur lors de la lecture du pipe pongping", 6);
    }
  }

  printf("Fin Ping\n");
  close(pingpong[1]);
  close(pongping[0]);
  wait(NULL);
}

void fils(void){
  close(pingpong[1]);
  close(pongping[0]);

  int pid = getpid();
  int valeur;

  while(read(pingpong[0], &valeur, sizeof(valeur)) > 0){
    int ajout = rand() % 10;
    printf("Processus de pid %d : Pong [%d] (je vais ajouter %d)\n", pid, valeur, ajout);
    valeur += ajout;

    if(write(pongping[1], &valeur, sizeof(valeur)) == -1){
      erreur("Erreur lors de l'ecriture dans le pipe pongping", 7);
    }
  }

  printf("Fin Pong\n");
  close(pingpong[0]);
  close(pongping[1]);
}
