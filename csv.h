#ifndef CSV_H
#define CSV_H
#include <bibli.h>


typedef struct {
    int collisionId;
    int vehiculeId;
    int annee;
    int age;
    int trajet;           // purpose
    int emplacement;      // location
    int type;       // 1 conducteur, 2 passager, 3 piéton
    int gravite;          // 1 à 4
    int genre;            // 1 masculin, 2 féminin, 0 non relevé
    int naiss;
} Ligne;

// Lit la ligne suivante. Retourne 1 si OK, 0 si fin de fichier.
// Quitte avec un message d'erreur si la ligne est invalide.
int lireLigne(FILE *f, Ligne *l);

// Saute la ligne d'en-tête (à appeler une fois après fopen).
void sauterEntete(FILE *f);

#endif

