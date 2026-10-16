#ifndef MATRICE_H
#define MATRICE_H

#define NBL_MAX 12
#define NBC_MAX 24

extern void initialiserMatrice(int matrice[NBL_MAX][NBC_MAX], unsigned nbLignes, unsigned nbColonnes);
extern void afficherMatrice(int matrice[NBL_MAX][NBC_MAX], unsigned nbLignes, unsigned nbColonnes);
extern int sommeLigne(int mat[NBL_MAX][NBC_MAX], unsigned nbC, unsigned numL);

#endif
