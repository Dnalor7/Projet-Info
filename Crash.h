#include<bibli.h>
#include <bibli.h>

typedef struct Victime{
    int age;
    int genre[2];
    int gravite [4];
    int type[3];
    struct Victime *suivant;
}Victime;

typedef struct Vehicule{
    Victime *car;
    int trajet;
    int id;
    struct Vehicule *suivant;
}Vehicule;

typedef struct AvlCrash{
    int hauteur;
    int cle;
    Vehicule *vehicule;
    struct AvlCrash *gauche;
    struct AvlCrash *droit;
}AvlCrash;
