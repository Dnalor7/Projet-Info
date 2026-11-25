#ifndef age_h
#define age_h

#include "bibli.h"

typedef struct ChainonAge{
  int age;
  struct ChainonAge *suivant;
  int genre;
  int type;
}ChainonAge;

#endif
