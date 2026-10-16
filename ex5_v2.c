#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>

#include "erreur.h"

void P(sem_t *sem) {
    if(sem_wait(sem) != 0)
        erreur("Echec sem_wait", 7);
}

void V(sem_t *sem) {
    if(sem_post(sem) !=0)
        erreur("Echec sem_post", 8);
}

void initialiser(sem_t *sem, unsigned valInit){
    if(sem_init(sem, 0, valInit) != 0)
        erreur("Echec sem_init", 9);
}

void detruire(sem_t *sem){
    if(sem_destroy(sem) == -1)
        erreur("Echec sem_destory", 10);
}

static unsigned NT;   // afficheur
static unsigned NM;
static unsigned NL;  

static sem_t *semTour;

void *codeAfficheur(void *arg) {
  unsigned numAfficheur = *((unsigned *)arg);
  unsigned suivant = (numAfficheur + 1) % NT;
  pthread_t tid = pthread_self();

  for (unsigned numMessage = 1; numMessage <= NM; numMessage++) {
    P(&semTour[numAfficheur]);
    for (unsigned numLigne = 1; numLigne <= NL; numLigne++) {
      printf("Afficheur %u (%lu), j'affiche ligne %u/%u du message %u/%u\n",
             numAfficheur, (unsigned long)tid, numLigne, NL, numMessage, NM);
      fflush(stdout);
    }
    
    if (numMessage == NM) {
      printf("Afficheur %u (%lu), je me termine\n", numAfficheur, (unsigned long)tid);
      fflush(stdout);
    }
    V(&semTour[suivant]);
  }

  return NULL;
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    fprintf(stderr, "Usage: %s NT NM NL\n", argv[0]);
    exit(1);
  }
  
  int nt = atoi(argv[1]);
  int nm = atoi(argv[2]);
  int nl = atoi(argv[3]);
  
  if (nt <= 0 || nm <= 0 || nl <= 0)
    erreur("invalide NT, NM, ou NL", 2);
  
  NT = nt; 
  NM = nm; 
  NL = nl;

  semTour = malloc(NT * sizeof(sem_t));
  pthread_t *tids = malloc(NT * sizeof(pthread_t));
  unsigned *numeros = malloc(NT * sizeof(unsigned));
  if (semTour == NULL || tids == NULL || numeros == NULL)
    erreur("Echec malloc", 3);

  for (unsigned num = 0; num < NT; num++)
    initialiser(&semTour[num], (num == 0) ? 1 : 0); 
  //jeton -> | afficheur

  for (unsigned num = 0; num < NT; num++) {
    numeros[num] = num;
    int code = pthread_create(&tids[num], NULL, codeAfficheur, &numeros[num]);
    if (code != 0)
      erreur("Echec pthread_create", 5);
  }

  for (unsigned num = 0; num < NT; num++) {
    int code = pthread_join(tids[num], NULL);
    if (code != 0)
      erreur("Echec pthread_join", 6);
  }

  for (unsigned num = 0; num < NT; num++)
    detruire(&semTour[num]);

  free(numeros);
  free(tids);
  free(semTour);
  
  printf("\nFin de l'execution du thread principal\n");
  
  return 0;
}

