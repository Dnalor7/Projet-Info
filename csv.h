#ifndef csv_h
#define csv_h
#include <bibli.h>


typedef struct Ligne {
    int collisionId;
    int vehiculeId;
    int annee;
    int age;
    int trajet;           // purpose
    int emplacement;      // location
    int type[3];       // 1 conducteur, 2 passager, 3 piéton
    int gravite[4];          // 1 à 4
    int genre[2];            // 1 masculin, 2 féminin, 0 non relevé
} Ligne;

void sauterEntete(FILE *f);
int lireLigne(FILE *f, Ligne *l);



#endif

