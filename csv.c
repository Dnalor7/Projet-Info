#include "bibli.h"
#include "csv.h"

#define NB_COLONNES 9
#define TAILLE_LIGNE 256

static int numLigne = 1;   // pour afficher la ligne fautive dans les erreurs

// Affiche une erreur et arrête le programme
static void erreur(const char *message) {
    fprintf(stderr, "Erreur ligne %d : %s\n", numLigne, message);
    exit(1);
}

// Convertit convertir une chaîne de caractères en nombre entier. Si le champ est vide, renvoie la valeur par défaut.
static int nombre(const char *champ, int defaut) {
    if (champ[0] == '\0') return defaut;
    return atoi(champ);
}

// Coupe la ligne à chaque ';' et range le début de chaque morceau dans champs[].
// Renvoie le nombre de morceaux trouvés.
static int decouper(char *ligne, char *champs[]) {
    int n = 0;
    char *p = ligne;

    while (n < NB_COLONNES) {
        champs[n] = p;
        n++;
        p = strchr(p, ';');      // cherche le prochain ';'
        if (p == NULL) break;    // plus de ';' : c'était le dernier morceau
        *p = '\0';               // remplace ';' par une fin de texte
        p++;                     // passe au morceau suivant
    }
    return n;
}

// Lit une ligne du fichier dans buf. Renvoie 0 à la fin du fichier.
// Ignore les lignes vides.
static int lireTexte(FILE *f, char *buf) {
    do {
        if (fgets(buf, TAILLE_LIGNE, f) == NULL) return 0; // Vérifie si la ligne récupérer est nul  
        numLigne++;
        buf[strcspn(buf, "\n")] = '\0';   // enlève le retour à la ligne
    } while (buf[0] == '\0');
    return 1;
}

//Lit et ignore la première ligne du fichier si le fichier est vide sa affiche une erreur et sa quitte.
void sauterEntete(FILE *f) {
    char buf[TAILLE_LIGNE];
    if (fgets(buf, TAILLE_LIGNE, f) == NULL) {
        fprintf(stderr, "Erreur : fichier vide\n");
        exit(1);
    }
}

int lireLigne(FILE *f, Ligne *l) {
    char buf[TAILLE_LIGNE];
    char *champs[NB_COLONNES];

    if (!lireTexte(f, buf)) return 0;

    if (decouper(buf, champs) < NB_COLONNES)
        erreur("il manque des colonnes");

    // On repart d'une structure vide : tous les tableaux sont à 0
    memset(l, 0, sizeof(Ligne));

    // --- Champs obligatoires : on vérifie qu'ils sont valides ---
    type    = nombre(champs[4], -1);
    gravite = nombre(champs[5], -2);
    genre   = nombre(champs[6], -3);
    naiss   = nombre(champs[7], -4);

    l->annee       = nombre(champs[0], 0);
    l->collisionId = nombre(champs[1], -5);
    l->vehiculeId  = nombre(champs[2], -6);
    l->trajet      = nombre(champs[8], 0);
    
    if (l->annee < 2005 || l->annee > 2024) erreur("année invalide");
    if (l->collisionId < 1)                 erreur("collision_id invalide");
    if (l->vehiculeId < 1 ||l->vehiculeId > 2)  erreur("vehicule_id invalide");
    if (type < 1 || type > 3)               erreur("user_type invalide");
    if (gravite < 1 || gravite > 4)         erreur("severity invalide");
    if (trajet < -1)                        erreur("trajets  ivalide ");
    
    // --- Champs non obligatoires : valeur par défaut si vide ---
    l->emplacement = nombre(champs[3], 0);

    // Tableaux "interrupteurs" : on met 1 dans la bonne case
    l->type[type - 1] = 1;
    l->gravite[gravite - 1] = 1;
    if (genre == 1 || genre == 2)
        l->genre[genre - 1] = 1;     // sinon : genre non relevé, les deux cases restent à 0

    // Âge = année de l'accident - année de naissance (-1 si inconnu)
    if (naiss > 1900 && naiss <= l->annee)
        l->age = l->annee - naiss;
    
    else{
        l->age = -1;

    }
    return 1;
}
