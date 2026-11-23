#include "csv.h"

#define NB_COLONNES 9

static unsigned long numLigne = 1;   // pour les messages d'erreur

static void erreurLigne(const char *msg) {
    fprintf(stderr, "Erreur ligne %lu : %s\n", numLigne, msg);
    exit(1);
}

// Convertit un champ en entier. Retourne 0 si vide ou invalide.
static int versEntier(const char *s, long *res) {
    char *fin;
    if (s == NULL || *s == '\0') return 0;
    *res = strtol(s, &fin, 10);
    return *fin == '\0';
}

// Champ obligatoire : erreur si absent ou hors de [min, max].
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

    if (fgets(buf, sizeof buf, f) == NULL) return 0;
    numLigne++;

    // Ligne tronquée : pas de '\n' alors que le fichier continue
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] != '\n' && !feof(f))
        erreurLigne("ligne trop longue");
    buf[strcspn(buf, "\r\n")] = '\0';

    if (buf[0] == '\0') return lireLigne(f, l);   // ligne vide : on l'ignore

    // Découpage manuel : strtok ignorerait les champs vides
    char *p = buf;
    int n = 0;
    while (n < NB_COLONNES) {
        champs[n++] = p;
        p = strchr(p, ';');
        if (p == NULL) break;
        *p++ = '\0';
    }
    if (n < NB_COLONNES) erreurLigne("nombre de colonnes insuffisant");

    // Champs obligatoires (stricts)
    l->annee       = (uint16_t)champObligatoire(champs[0], 1900, 2100, "year");
    l->collisionId = (uint32_t)champObligatoire(champs[1], 0, 99999999, "collision_id");
    l->vehiculeId  = (uint32_t)champObligatoire(champs[2], 0, 4294967295L, "vehicule_id");
    l->typeUsager  = (uint8_t) champObligatoire(champs[4], 1, 3, "user_type");
    l->gravite     = (uint8_t) champObligatoire(champs[5], 1, 4, "severity");

    // Champs tolérants (valeur par défaut si vide ou non relevé)
    l->emplacement = versEntier(champs[3], &v) && v >= 0 && v <= 255 ? (uint8_t)v : 0;
    l->genre = (versEntier(champs[6], &v) && (v == 1 || v == 2)) ? (uint8_t)v : 0;
    l->anneeNaissance = (versEntier(champs[7], &v) && v > 1850 && v <= l->annee)
                        ? (uint16_t)v : 0;
    l->trajet = (versEntier(champs[8], &v) && v >= -1 && v <= 127) ? (int8_t)v : 0;

    return 1;
}
