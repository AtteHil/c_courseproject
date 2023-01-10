
#include "HTPerusKirjasto.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Korjataan kommentin mukaan includaukset tänne .c tiedostoon kun niin ilmeisesti pitikin olla

int printingmenu()
{ // printataan valikko ja haetaan käyttäjältä valinta, joka palautetaan inttinä.
    int Goose;
    printf("Valitse haluamasi toiminto:\n");
    printf("1) Lue tiedosto\n");
    printf("2) Analysoi tiedot\n");
    printf("3) Kirjoita tulokset\n");
    printf("0) Lopeta\n");
    printf("Anna valintasi: ");
    scanf(" %d", &Goose);
    getchar();
    return Goose;
}

char *getfilename(char *filename, int i)
{ // haetaan käyttäjältä tiedostonimi ja parametri i käytetään kumpaa kysystään eli jos i = 1 kysytään luettavaa tiedostoa ja muuten kirjoitettavaa.

    if (i == 1)
    {
        printf("Anna luettavan tiedoston nimi: ");
        fgets(filename, CN, stdin);
        filename[strlen(filename) - 1] = '\0';
    }
    else
    {
        printf("Anna kirjoitettavan tiedoston nimi: ");
        fgets(filename, CN, stdin);
        filename[strlen(filename) - 1] = '\0';
    }
    return filename;
}

void readdata(char *filename, RAWDATA **pStart)
{ // Luetaan data linkitettyyn listaan, josta sitä voidaan analysoida.
    char Line[NU];
    FILE *Myfile;
    RAWDATA *pNew = NULL;
    RAWDATA *pEnd = NULL;
    Myfile = fopen(filename, "r");
    if (Myfile == NULL)
    {
        perror("Tiedoston avaaminen epäonnistui, lopetetaan");
        exit(0);
    }

    while ((fgets(Line, sizeof(Line), Myfile)) != NULL)
    {
        pNew = Addnode(Line);

        if (*pStart == NULL)
        { // Uusi tietue lisätään linkitetyn listan jatkoksi.
            *pStart = pNew;
            pNew->pNext = NULL;
            pEnd = pNew;
        }
        else
        {
            pEnd->pNext = pNew;
            pEnd = pNew;
        }
    }
    fclose(Myfile);
    return;
}

RAWDATA *Addnode(char Line[NU])
{
    RAWDATA *pNew = NULL;
    if ((pNew = (RAWDATA *)malloc(sizeof(RAWDATA))) == NULL)
    {
        perror("Muistin varaus epäonnistui, lopetataan..");
        exit(0);
    }

    char *token = strtok(Line, ";"); // Jokainen tietokohta otetaan erilleen strtok komennolla ja laitetaan omaan tietueen jäseneen.
    strcpy(pNew->time, token);
    pNew->week = atoi(strtok(NULL, ";"));
    pNew->usage = atoi(strtok(NULL, ";"));
    pNew->sun = atoi(strtok(NULL, ";"));
    pNew->wind = atoi(strtok(NULL, ";"));
    pNew->water = atoi(strtok(NULL, ";"));
    pNew->nuclear = atoi(strtok(NULL, ";"));
    pNew->combined = atoi(strtok(NULL, ";"));
    pNew->heat = atoi(strtok(NULL, " "));
    pNew->pNext = NULL;
    return pNew;
}

void ReadtoResult(RAWDATA *pRaw, RESULT *result)
{ // analysoidaan tietoja mikä on pienin ja suurin kulutus ja niiden pointterit tallennetaan.
    RAWDATA *ptr;
    RAWDATA *biggestptr;
    RAWDATA *smallestptr;

    ptr = pRaw;
    ptr = ptr->pNext;

    int smallest = ptr->usage;
    int biggest = ptr->usage;

    double usageSum = 0;
    int counter = 0;
    char monthnumber[2]; // Otetaan kuukaudelle oma väliaikainen paikka.
    unsigned long combinedproduce = 0;

    while (ptr != NULL)
    {

        monthnumber[0] = ptr->time[3];
        monthnumber[1] = ptr->time[4];
        usageSum = usageSum + ptr->usage;

        if (biggest < ptr->usage)
        {
            biggest = ptr->usage;
            biggestptr = ptr;
        }
        if (smallest > ptr->usage)
        {
            smallest = ptr->usage;
            smallestptr = ptr;
        }

        combinedproduce = ptr->sun + ptr->water + ptr->heat + ptr->nuclear + ptr->wind + ptr->combined;

        for (int i = 0; i < 12; i++)
        { // täytetään tietueen kk jokaisen kk numerolla
            result->ar[i].monthnum = i + 1;
        }

        result->ar[atoi(monthnumber) - 1].produce += combinedproduce; // jokaisen kk arvoon laitetaan yhteenlaskettuna tuotanto

        ptr = ptr->pNext;
        counter++;
    }
    result->sum = usageSum; // Laitetaan result jäseniin arvot
    result->count = counter;
    result->average = (usageSum / counter);
    result->least = smallestptr; // Pointterit tietueen alkioon, jossa on suurin ja pienin kulutus, että saadaan ne myöhemmin tulostettua niiden time jäsen ja usage jäsen.
    result->most = biggestptr;
    printf("Analysoitu %d mittaustulosta.\n", result->count);
    printf("Kokonaiskulutus oli yhteensä %lu kWh.\n", result->sum);
    printf("Kuukausittaiset tuotannot analysoitu.\n");
    return;
}

void printAll(char *filename, RESULT *result)
{ // printataan tiedostoon ja ennen sitä myös näytölle samat tiedot, jotka analysoitu jo aiemmin ja nyt vaan otetaan ne result tietueen jäsenistä.

    char printdata[LI][NU] = {0};

    snprintf(printdata[0], NU, "Tilastotiedot %d mittaustuloksesta:\nKulutus oli yhteensä %lu kWh, ja keskimäärin %.1f kWh.\n", result->count, result->sum, result->average);
    snprintf(printdata[1], NU, "Suurin kulutus, %d kWh, tapahtui %s.\n", result->most->usage, result->most->time);
    snprintf(printdata[2], NU, "Pienin kulutus, %d kWh, tapahtui %s.\n", result->least->usage, result->least->time);
    snprintf(printdata[3], NU, "\n");
    snprintf(printdata[4], NU, "Pvm;Tuotanto (GWh)\n"); // Kirjoitetaan tiedot bufferiin ja siitä ne saadaan tulosteltua näytölle sekä tiedostoon.
    for (int i = 0; i < 12; i++)
    {
        snprintf(printdata[5 + i], NU, "Kk %d;%.2f\n", result->ar[i].monthnum, result->ar[i].produce / 1000000.f);
    }
    printToUser(printdata);
    printToFile(printdata, filename);

    return;
}
void printToUser(char printdata[LI][NU])
{
    for (int i = 0; i < 17; i++)
    {
        printf("%s", printdata[i]);
    }
    return;
}
void printToFile(char printdata[LI][NU], char *filename)
{
    FILE *Writefile;
    Writefile = fopen(filename, "w");
    if (Writefile == NULL)
    {
        perror("Tiedoston avaaminen epäonnistui, lopetetaan");
        exit(0);
    }
    for (int i = 0; i < 17; i++)
    {
        fprintf(Writefile, "%s", printdata[i]);
    }
    fclose(Writefile);
    return;
}

void freememory(RAWDATA **pStart)
{ // vapautetaan aiemmin mallocilla varattu muisti.
    RAWDATA *ptr;
    RAWDATA *New;
    ptr = *pStart;
    while (ptr != NULL)
    {
        New = ptr->pNext;
        free(ptr);
        ptr = New;
    }
    return;
}