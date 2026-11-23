#ifndef CSV_H
#define CSV_H

#include "commun.h"

typedef struct {
    uint32_t collisionId;
    uint32_t vehiculeId;
    uint16_t annee;
    uint16_t anneeNaissance;   // 0 si non renseignée
    int8_t   trajet;           // purpose
    uint8_t  emplacement;      // location
    uint8_t  typeUsager;       // 1 conducteur, 2 passager, 3 piéton
    uint8_t  gravite;          // 1 à 4
    uint8_t  genre;            // 1 masculin, 2 féminin, 0 non relevé
} Ligne;

// Lit la ligne suivante. Retourne 1 si OK, 0 si fin de fichier.
// Quitte avec un message d'erreur si la ligne est invalide.
int lireLigne(FILE *f, Ligne *l);

// Saute la ligne d'en-tête (à appeler une fois après fopen).
void sauterEntete(FILE *f);

#endif

