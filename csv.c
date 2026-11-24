#include <bibli.h>
#include "csv.h"

#define NB_COLONNES 9
#define TAILLE_LIGNE 256

static unsigned long numLigne = 1;   // numéro de ligne, pour les messages d'erreur

static void erreurLigne(const char *msg) {
    fprintf(stderr, "Erreur ligne %lu : %s\n", numLigne, msg);
    exit(1);
}

// Convertit un texte en entier. Retourne 1 si OK, 0 si vide ou invalide.
static int versEntier(const char *s, long *res) {
    char *fin;
    if (s == NULL || *s == '\0') return 0;
    *res = strtol(s, &fin, 10);
    return *fin == '\0';
}

// Champ obligatoire : arrête le programme si absent ou hors de [min, max]
static long champObligatoire(const char *s, long min, long max, const char *nom) {
    long v;
    char msg[64];
    if (!versEntier(s, &v) || v < min || v > max) {
        snprintf(msg, sizeof msg, "valeur invalide pour %s", nom);
        erreurLigne(msg);
    }
    return v;
}

void sauterEntete(FILE *f) {
    char buf[TAILLE_LIGNE];
    if (fgets(buf, sizeof buf, f) == NULL) {
        fprintf(stderr, "Erreur : fichier vide\n");
        exit(1);
    }
}

int lireLigne(FILE *f, Ligne *l) {
    char buf[TAILLE_LIGNE];
    char *champs[NB_COLONNES];
    long v;
    size_t len;

    // Lit la ligne suivante en ignorant les lignes vides
    do {
        if (fgets(buf, sizeof buf, f) == NULL) return 0;   // fin de fichier
        numLigne++;
        len = strlen(buf);
        if (len > 0 && buf[len - 1] != '\n' && !feof(f))
            erreurLigne("ligne trop longue");
        buf[strcspn(buf, "\r\n")] = '\0';
    } while (buf[0] == '\0');

    // Découpage au ';' (strtok sauterait les champs vides et décalerait tout)
    char *p = buf;
    int n = 0;
    while (n < NB_COLONNES) {
        champs[n++] = p;
        p = strchr(p, ';');
        if (p == NULL) break;
        *p++ = '\0';
    }
    if (n < NB_COLONNES) erreurLigne("nombre de colonnes insuffisant");

    // Tout remettre à zéro (les tableaux type/gravite/genre démarrent vides)
    memset(l, 0, sizeof *l);

    // Champs obligatoires
    l->annee       = (int)champObligatoire(champs[0], 1900, 2100, "year");
    l->collisionId = (int)champObligatoire(champs[1], 0, 99999999, "collision_id");
    l->vehiculeId  = (int)champObligatoire(champs[2], 0, 2147483647L, "vehicule_id");

    int type    = (int)champObligatoire(champs[4], 1, 3, "user_type");
    int gravite = (int)champObligatoire(champs[5], 1, 4, "severity");

    // Les tableaux fonctionnent comme des "interrupteurs" : une seule case vaut 1
    l->type[type - 1] = 1;
    l->gravite[gravite - 1] = 1;

    // Champs tolérants (valeur par défaut si vide ou non relevé)
    l->emplacement = (versEntier(champs[3], &v) && v >= 0 && v <= 255) ? (int)v : 0;
    l->trajet      = (versEntier(champs[8], &v) && v >= -1 && v <= 127) ? (int)v : 0;

    if (versEntier(champs[6], &v) && (v == 1 || v == 2))
        l->genre[v - 1] = 1;               // sinon : les deux cases restent à 0

    // Âge = année de l'accident - année de naissance, -1 si inconnu
    if (versEntier(champs[7], &v) && v > 1850 && v <= l->annee && l->annee - v <= 120)
        l->age = l->annee - (int)v;
    else
        l->age = -1;

    return 1;
}
