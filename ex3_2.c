#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#include "erreur.h"
#include "matrice.h"

static void erreurThread(int code, const char *mess, int valExit)
{
    errno = code;
    erreur(mess, valExit);
}

//what le thread  principal indique a chaque thread sous-traitant
//sur quelle matrix travailler
//combien de colones elle a 
//quelle ligne traiter

typedef struct {
    int(*matrix)[NBC_MAX]; //pointeur vers matrice du thread principal
    unsigned nbColonnes;
    unsigned numLigne;
} ArgST; //sous traitant


void *codeSousTraitant(void *arg){
    ArgST *param = (ArgST *)arg;

    int *res = malloc(sizeof(int));
    if(res == NULL)
        erreur("Echec malloc", 4);

    *res = sommeLigne(param->matrix, param->nbColonnes, param->numLigne);

    printf("Je suis le thread (%lu) arrache a la ligne %u, sa somme vaut %d\n",
            (unsigned long)pthread_self(), param->numLigne, *res);

    pthread_exit(res); //transmet -> thread principal
}

int main(int argc, char *argv[]) {
    if(argc != 3){
        fprintf(stderr, "Usage: %s nbLignes nbColonnes\n", argv[0]);
        exit(1);
    }

    int nbLignes = atoi(argv[1]);
    int nbColonnes = atoi(argv[2]);
    if(nbLignes <= 0 || nbLignes > NBL_MAX || nbColonnes <= 0 || nbColonnes > NBC_MAX)
        erreur("invalide nbLignes et nbColonnes", 2);

    int matrice[NBL_MAX][NBC_MAX];
    initialiserMatrice(matrice, nbLignes, nbColonnes);
    afficherMatrice(matrice, nbLignes, nbColonnes);

    pthread_t tids[NBL_MAX];
    ArgST args[NBL_MAX]; //1 parametre -> thread
                             
    for(int numL = 0; numL < nbLignes; numL++){
        args[numL].matrix = matrice;
        args[numL].nbColonnes = nbColonnes;
        args[numL].numLigne = numL;
        int code = pthread_create(&tids[numL], NULL, codeSousTraitant, &args[numL]);
        if(code !=0)
            erreurThread(code, "Echec pthread_create", 5);
    }

    int somme = 0;
    for(int numL = 0; numL < nbLignes; numL++){
        int *res;
        int code = pthread_join(tids[numL], (void**)&res);
        if(code !=0)
            erreurThread(code, "Echec pthread_join", 6);
        somme += *res;
        free(res);
    }

    printf("Je suis le thread (%lu) initial, la somme des valeurs recues est %d\n",
            (unsigned long)pthread_self(), somme);
    return 0;

}


