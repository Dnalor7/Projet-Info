#include "bibli.h"
#include "csv.h"
#include "year.h"

static YearData annees[NB_ANNEES];

// À adapter selon la définition exacte de "severity" dans ton sujet.
static const char *LIBELLES_GRAVITE[NB_GRAVITES] = {
    "Severite 1",
    "Severite 2",
    "Severite 3",
    "Severite 4"
};

// Parcourt tout le CSV et remplit annees[].
// Une collision est comptée une seule fois, même si elle a plusieurs lignes
// (usagers) : on suppose les lignes d'une même collision consécutives.
static void calculer(FILE *f) {
    Ligne l;
    int derniereAnnee = 0;
    int derniereCollision = 0;

    sauterEntete(f);
    while (lireLigne(f, &l)) {
        int i = l.annee - ANNEE_DEBUT;

        if (l.annee != derniereAnnee || l.collisionId != derniereCollision) {
            annees[i].accident++;
            derniereAnnee = l.annee;
            derniereCollision = l.collisionId;
        }
        for (int k = 0; k < NB_GRAVITES; k++)
            annees[i].gravite[k] += l.gravite[k];
    }
}

static void afficherAnnee(int annee) {
    YearData *d = &annees[annee - ANNEE_DEBUT];

    printf("\n===== Statistiques %d =====\n", annee);
    printf("Nombre d'accidents : %d\n", d->accident);
    for (int k = 0; k < NB_GRAVITES; k++)
        printf("  %-12s : %d\n", LIBELLES_GRAVITE[k], d->gravite[k]);
    printf("\n");
}

// Demande une année à l'utilisateur. Renvoie 0 pour quitter.
static int demanderAnnee(void) {
    char buf[64];
    int annee;

    for (;;) {
        printf("Annee a consulter (%d-%d), 0 pour quitter : ", ANNEE_DEBUT, ANNEE_FIN);
        if (fgets(buf, sizeof buf, stdin) == NULL) return 0;   // fin de saisie
        if (sscanf(buf, "%d", &annee) != 1) {
            printf("Saisie invalide.\n");
            continue;
        }
        if (annee == 0) return 0;
        if (annee >= ANNEE_DEBUT && annee <= ANNEE_FIN) return annee;
        printf("Annee hors de l'intervalle.\n");
    }
}

void modeYearData(const char *cheminCsv) {
    FILE *f = fopen(cheminCsv, "r");
    int annee;

    if (f == NULL) {
        fprintf(stderr, "Impossible d'ouvrir %s : %s\n", cheminCsv, strerror(errno));
        exit(1);
    }

    memset(annees, 0, sizeof annees);
    calculer(f);
    fclose(f);

    while ((annee = demanderAnnee()) != 0)
        afficherAnnee(annee);
}



