#include "bibli.h"
#include "year.h"

// Usage : ./accidents fichier.csv
int main(int argc, char *argv[]) {
    char buf[64];
    int mode = -1;

    if (argc < 2) {
        fprintf(stderr, "Usage : %s fichier.csv\n", argv[0]);
        return 1;
    }

    printf("Modes disponibles :\n  1 - YEAR_DATA\n  0 - Quitter\nChoix : ");
    if (fgets(buf, sizeof buf, stdin) != NULL)
        sscanf(buf, "%d", &mode);

    switch (mode) {
        case 1:
            modeYearData(argv[1]);
            break;
        case 0:
            break;
        default:
            printf("Mode inconnu.\n");
    }
    return 0;
}
