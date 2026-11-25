#ifnedf year_h
#define year_h
#define NB_GRAVITES 4
#include "bibli.h"

typedef struct YearData{
    int accident; 
    int gravite[NB_GRAVITES];
}YearData;

void modeYearData(const char *cheminCsv);

#endif
