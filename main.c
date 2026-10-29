#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 100
#define INPUTFILE "zavodnici.txt"
#define OUTPUTFILE "vysledkova_listina.txt"
#define DELIMITERS " :."

typedef struct {
    int minuty;
    int sekundy;
    int setiny;
} CAS;

typedef struct {
    int cislo;
    char prijmeni[15];
    char jmeno[15];
    CAS cas_jedna;
    CAS cas_dva;
} ZAVODNIK;

ZAVODNIK *readFile(ZAVODNIK *zavodnik, int *pocet) {
    FILE *fr = fopen(INPUTFILE, "r");
    if (fr == NULL) {
        printf("Soubor %s nebyl otevren.\n", INPUTFILE);
        return NULL;
    } else {
        char radek[MAX];
        int i = 0;
        int j;
        char * token = NULL;
        ZAVODNIK *pomocny = NULL;
        while (fgets(radek, MAX, fr) != NULL) {
            if (i != 0) {
                pomocny = (ZAVODNIK *) realloc(zavodnik, sizeof(ZAVODNIK) * i);
                if (pomocny == NULL) {
                    printf("Nebyl uspesne realokovan pametovy blok.\n");
                    free(zavodnik);
                } else {
                    zavodnik = pomocny;
                    token = strtok(radek, DELIMITERS);
                    j = 0;
                }
                while (token != NULL) {
                    switch (j) {
                        case 0:
                            zavodnik[i - 1].cislo = atoi(token);
                            break;
                        case 1:
                            strcpy(zavodnik[i - 1].prijmeni, token);
                            break;
                        case 2:
                            strcpy(zavodnik[i - 1].jmeno, token);
                            break;
                        case 3:
                            zavodnik[i - 1].cas_jedna.minuty = atoi(token);
                            break;
                        case 4:
                            zavodnik[i - 1].cas_jedna.sekundy = atoi(token);
                            break;
                        case 5:
                            zavodnik[i - 1].cas_jedna.setiny = atoi(token);
                            break;
                        case 6:
                            zavodnik[i - 1].cas_dva.minuty = atoi(token);
                            break;
                        case 7:
                            zavodnik[i - 1].cas_dva.sekundy = atoi(token);
                            break;
                        case 8:
                            zavodnik[i - 1].cas_dva.setiny = atoi(token);
                            break;
                    }
                    j++;
                    token = strtok(NULL, DELIMITERS);
                }
            }
            i++;
        }
        *pocet = i - 1;
    }
    if (fclose(fr) == EOF) {
        printf("Soubor %s se nepodarilo zavrit", INPUTFILE);
    }
    return zavodnik;
}

void vypis(ZAVODNIK *zavodnik, int pocet) {
    printf("S T A R T O V N I  L I S T I N A  -  S L A L O M  Z E N Y\n");
    printf("---------------------------------------------------------------------\n");
    printf("startovni cislo|       prijmeni|       jmeno|cas 1. kola|cas 2. kola|\n");
    printf("---------------------------------------------------------------------\n");
    for (int i = 0; i < pocet; i++) {
        printf("%14d.|%15s|%12s|   %02d:%02d.%02d|   %02d:%02d.%02d|\n", zavodnik[i].cislo, zavodnik[i].prijmeni,
               zavodnik[i].jmeno,
               zavodnik[i].cas_jedna.minuty, zavodnik[i].cas_jedna.sekundy, zavodnik[i].cas_jedna.setiny,
               zavodnik[i].cas_dva.minuty,
               zavodnik[i].cas_dva.sekundy, zavodnik[i].cas_dva.setiny);
    }
    printf("---------------------------------------------------------------------\n");
}

int prevod(CAS cas) {
    return cas.minuty * 6000 + cas.sekundy * 100 + cas.setiny;
}

void swap(ZAVODNIK * zavodnik1, ZAVODNIK * zavodnik2) {
    ZAVODNIK temp = *zavodnik1;
    *zavodnik1 = *zavodnik2;
    *zavodnik2 = temp;
}

void bubbleSort(ZAVODNIK * zavodnik, int pocet) {
    for (int i = 0; i < pocet - 1; i++) {
            for (int j = 0; j < pocet - 1 - i; j++) {
                if (prevod(zavodnik[j].cas_jedna) + prevod(zavodnik[j].cas_dva) > prevod(zavodnik[j + 1].cas_jedna) + prevod(zavodnik[j + 1].cas_dva)) {
                    swap(&zavodnik[j], &zavodnik[j + 1]);
                }
            }
        }
    }

void doSouboru(ZAVODNIK *zavodnik, int pocet) {
    FILE * fw = fopen(OUTPUTFILE, "w");
    if (fw == NULL) {
        printf("Nepodarilo se otevrit soubor %s.\n", OUTPUTFILE);
    } else {
        printf("Byl vytvoren soubor %s.\n", OUTPUTFILE);
        bubbleSort(zavodnik, pocet);
        fprintf(fw, "V Y S L E D K O V A  L I S T I N A\n");
        fprintf(fw, "-----------------------------------\n");
        fprintf(fw, "poradi|cislo |  prijmeni     |   jmeno  | 1.kolo | 2.kolo | celkem |  ztrata\n");
        fprintf(fw, "---------------------------------------------------------------------------\n");
        for (int i = 0; i < pocet; i++) {
            if (i == 0) {
                fprintf(fw,"%6d|%6d|%15s|%10s|%02d:%02d.%02d|%02d:%02d.%02d|%02d:%02d.%02d|\n", i, zavodnik[i].cislo,
               zavodnik[i].prijmeni, zavodnik[i].jmeno, zavodnik[i].cas_jedna.minuty, zavodnik[i].cas_jedna.sekundy, zavodnik[i].cas_jedna.setiny,
               zavodnik[i].cas_dva.minuty, zavodnik[i].cas_dva.sekundy, zavodnik[i].cas_dva.setiny, (prevod(zavodnik[i].cas_jedna) + prevod(zavodnik[i].cas_dva)) / 6000,
               (prevod(zavodnik[i].cas_jedna) + prevod(zavodnik[i].cas_dva)) % 6000 / 100, (prevod(zavodnik[i].cas_jedna) + prevod(zavodnik[i].cas_dva)) % 100);
            } else {
                fprintf(fw,"%6d|%6d|%15s|%10s|%02d:%02d.%02d|%02d:%02d.%02d|%02d:%02d.%02d|+%02d:%02d.%02d\n", i, zavodnik[i].cislo,
               zavodnik[i].prijmeni, zavodnik[i].jmeno, zavodnik[i].cas_jedna.minuty, zavodnik[i].cas_jedna.sekundy, zavodnik[i].cas_jedna.setiny,
               zavodnik[i].cas_dva.minuty, zavodnik[i].cas_dva.sekundy, zavodnik[i].cas_dva.setiny, (prevod(zavodnik[i].cas_jedna) + prevod(zavodnik[i].cas_dva)) / 6000,
               (prevod(zavodnik[i].cas_jedna) + prevod(zavodnik[i].cas_dva)) % 6000 / 100, (prevod(zavodnik[i].cas_jedna) + prevod(zavodnik[i].cas_dva)) % 100,
               ((prevod(zavodnik[i].cas_jedna) + prevod(zavodnik[i].cas_dva)) - (prevod(zavodnik[0].cas_jedna) + prevod(zavodnik[0].cas_dva))) / 6000,
               ((prevod(zavodnik[i].cas_jedna) + prevod(zavodnik[i].cas_dva)) - (prevod(zavodnik[0].cas_jedna) + prevod(zavodnik[0].cas_dva))) % 6000 / 100,
               ((prevod(zavodnik[i].cas_jedna) + prevod(zavodnik[i].cas_dva)) - (prevod(zavodnik[0].cas_jedna) + prevod(zavodnik[0].cas_dva))) % 100);
            }

        }
    }
    if (fclose(fw) == EOF) {
        printf("Soubor %s nebyl zavren.\n", OUTPUTFILE);
    }
}

int main(void) {
    ZAVODNIK *zavodnik = NULL;
    int pocet = 0;
    zavodnik = readFile(zavodnik, &pocet);
    vypis(zavodnik, pocet);
    doSouboru(zavodnik, pocet);
    return 0;
}