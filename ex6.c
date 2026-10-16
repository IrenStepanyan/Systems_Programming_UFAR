#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "erreur.h"

sem_t A_est_fait;
sem_t D_est_fait_pour_T1;
sem_t D_est_fait_pour_T2;
sem_t C_est_fait;

int nbCycles;

void P(sem_t *sem) {
    if(sem_wait(sem) == -1)
        erreur("Echec sem_wait", 1);
}

void V(sem_t *sem) {
    if(sem_post(sem) == -1 )
        erreur("Echec sem_post", 2);
}

void initialiser(sem_t *sem, unsigned valInit){
    if(sem_init(sem, 0, valInit) == -1)
        erreur("Echec sem_init", 3);
}

void afficher(char *s){
    printf("%s", s);
    fflush(stdout);
}

void initialisation(void) {
    initialiser(&A_est_fait, 0);
    initialiser(&D_est_fait_pour_T1, 0);
    initialiser(&D_est_fait_pour_T2, 0);
    initialiser(&C_est_fait, 0);
}

void *threadT1(void *arg){
    (void)arg;
    for(int i = 0; i < nbCycles; i++) {
        afficher("A");
        V(&A_est_fait);
        P(&D_est_fait_pour_T1);
        afficher("B");
        P(&C_est_fait);
        afficher("\n");
    }
    
    return NULL;
}

void *threadT2(void *arg){
    (void)arg;
    for(int i = 0; i < nbCycles; i++) {
        P(&D_est_fait_pour_T2);
        afficher("C");
        V(&C_est_fait);
    }
    
    return NULL;
}

void *threadT3(void *arg){
    (void)arg;
    for(int i = 0; i < nbCycles; i++) {
        P(&A_est_fait);
        afficher("D");

        if(rand() % 2){
            V(&D_est_fait_pour_T1);
            V(&D_est_fait_pour_T2);
        }else{
            V(&D_est_fait_pour_T1);
            V(&D_est_fait_pour_T2);
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if(argc != 2)
        erreur("Usage: ./ex6 nbCycles", 4);

    nbCycles = atoi(argv[1]);
    if(nbCycles <= 0)
        erreur("nombre de cycles doit etre > 0", 1);

    initialisation();

    pthread_t t1, t2, t3;
    
    if (pthread_create(&t1, NULL, threadT1, NULL) != 0) 
        erreur("Echec pthread_create T1", 2);
    if (pthread_create(&t2, NULL, threadT2, NULL) != 0) 
        erreur("Echec pthread_create T2", 3);
    if (pthread_create(&t3, NULL, threadT3, NULL) != 0) 
        erreur("Echec pthread_create T3", 4);

    int code = 0;
    code = pthread_join(t1, NULL);
    if(code != 0)
        erreur("Echec pthread_join T1", 5);

    code = pthread_join(t2, NULL);
    if (code != 0) 
        erreur("Echec pthread_join T2", 6);
    code =pthread_join(t3, NULL);
    if (code != 0) 
        erreur("Echec pthread_join T3", 7);

    return 0;
}



