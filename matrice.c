#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "matrice.h"

void initialiserMatrice(int matrice[NBL_MAX][NBC_MAX], unsigned nbLignes, unsigned nbColonnes) {
  time_t t;
  time(&t);
  srand((unsigned)t);
  for (unsigned indLigne = 0; indLigne < nbLignes; indLigne++)
    for (unsigned indColonne = 0; indColonne < nbColonnes; indColonne++)
      matrice[indLigne][indColonne] = rand() % nbColonnes;
}
  
void afficherMatrice(int matrice[NBL_MAX][NBC_MAX], unsigned nbLignes, unsigned nbColonnes) {
  for (unsigned indLigne = 0; indLigne < nbLignes; indLigne++){
    printf("%u:", indLigne);
    for (unsigned indColonne = 0; indColonne < nbColonnes; indColonne++)
      printf("\t%4u",matrice[indLigne][indColonne]);
    printf("\n");
  }
}

int sommeLigne(int mat[NBL_MAX][NBC_MAX], unsigned nbC, unsigned numL){
  int somme = 0;
  int *ligne = mat[numL];
  for (unsigned indCol = 0; indCol < nbC; indCol++){
    somme += ligne[indCol];
  }
  return somme;
}
