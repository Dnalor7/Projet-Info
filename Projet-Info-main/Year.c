#include "bibli.h"
#include "csv.h"
#include "year.h"

// Tableau des statistiques : une case par année
// annees[0] = 2005, annees[1] = 2006, ..., annees[19] = 2024
YearData annees[NB_ANNEES];


// Met toutes les statistiques à 0 avant de commencer
void initialiser(void) {
    int i, k;

    for (i = 0; i < NB_ANNEES; i++) {
        annees[i].accident = 0;
        for (k = 0; k < 4; k++) {
            annees[i].gravite[k] = 0;
        }
    }
}


// Lit tout le fichier et remplit le tableau annees[]
void calculer(FILE *f) {
    Ligne l;
    int i, k;
    int anneePrec = 0;       // année de la ligne précédente
    int collisionPrec = 0;   // collision de la ligne précédente

    sauterEntete(f);

    while (lireLigne(f, &l) == 1) {
        i = l.annee - ANNEE_DEBUT;   // indice de l'année dans le tableau

        // Une collision a plusieurs lignes (une par usager).
        // On ne la compte qu'une fois : quand on change de collision.
        if (l.annee != anneePrec || l.collisionId != collisionPrec) {
            annees[i].accident = annees[i].accident + 1;
            anneePrec = l.annee;
            collisionPrec = l.collisionId;
        }

        // Chaque usager ajoute 1 dans sa case de gravité
        for (k = 0; k < 4; k++) {
            annees[i].gravite[k] = annees[i].gravite[k] + l.gravite[k];
        }
    }
}


// Affiche les statistiques d'une année
void afficher(int annee) {
    int i = annee - ANNEE_DEBUT;

    printf("\n--- Statistiques %d ---\n", annee);
    printf("Nombre d'accidents : %d\n", annees[i].accident);
    printf("Gravite 1 : %d\n", annees[i].gravite[0]);
    printf("Gravite 2 : %d\n", annees[i].gravite[1]);
    printf("Gravite 3 : %d\n", annees[i].gravite[2]);
    printf("Gravite 4 : %d\n", annees[i].gravite[3]);
}


// Vide le clavier si l'utilisateur a tapé n'importe quoi
void viderBuffer(void) {
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}


// Menu : on demande une année tant que l'utilisateur ne tape pas 0
void menu(void) {
    int annee = -1;
    int resultat;

    while (annee != 0) {
        printf("\nAnnee a consulter (%d a %d), 0 pour quitter : ", ANNEE_DEBUT, ANNEE_FIN);
        resultat = scanf("%d", &annee);

        if (resultat != 1) {
            printf("Saisie invalide.\n");
            viderBuffer();
            annee = -1;                      // on reste dans la boucle
        }
        else if (annee == 0) {
            printf("Au revoir.\n");
        }
        else if (annee < ANNEE_DEBUT || annee > ANNEE_FIN) {
            printf("Annee hors de l'intervalle.\n");
        }
        else {
            afficher(annee);
        }
    }
}


// Fonction appelée par le main quand on choisit le mode YEAR_DATA
void modeYearData(const char *cheminCsv) {
    FILE *f = fopen(cheminCsv, "r");

    if (f == NULL) {
        printf("Impossible d'ouvrir le fichier %s\n", cheminCsv);
        exit(1);
    }

    initialiser();
    calculer(f);
    fclose(f);

    menu();
}
