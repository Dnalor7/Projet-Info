#ifndef crash_h
#define crash_h

#include "bibli.h"

typedef struct Victime{
    int age;
    int genre;
    int gravite;
    int type;
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

#endif
