
#include "HTPerusKirjasto.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void)
{

    char readfile[CN], writefile[CN];
    RAWDATA *pRaw = NULL;
    RESULT result = {0, 0, 0.0, NULL, NULL, {{0}}};

    while (1)
    {
        int Goose = printingmenu();
        switch (Goose)
        {
        case 1:
            freememory(&pRaw); // jos jo aiemmin luettu joku tiedosto linkitettyyn listaan se tyhjennetään ja alku laitetaan NULL:iksi.
            pRaw = NULL;
            getfilename(readfile, 1);
            readdata(readfile, &pRaw);
            printf("Tiedosto '%s' luettu.\n", readfile);

            break;

        case 2:
            if (strlen(readfile) == 0)
            { // Katsotaan onko tiedosto nimeen otettu vielä nimea eli onko 1 vaihtoehtoa käytetty.
                printf("Ei analysoitavaa, lue tiedosto ennen analyysiä.\n");
            }
            else
            {
                result = (RESULT){0, 0, 0.0, NULL, NULL, {{0}}}; // Nollataan result tietue ennen kuin sitä käytetään. Tiedot ei mene päällekkäin useasti kutsuessa vaan aina laitetaan tyhjään listaan.
                ReadtoResult(pRaw, &result);
            }
            break;

        case 3:
            if (result.sum == 0)
            { // Katsotaan onko result tietueessa vielä summalla mitään dataaa, jos ei ole ei ole dataa analysoitu, jos on niin voidaan mennä tiedoston kirjoittamiseen.
                printf("Ei kirjoitettavia tietoja, analysoi tiedot ennen tallennusta.\n");
            }
            else
            {
                getfilename(writefile, 2);
                printAll(writefile, &result);
                printf("Tiedosto '%s' kirjoitettu.\n", writefile);
            }
            break;

                case 0:
            printf("\nKiitos ohjelman käytöstä.\n"); // Lopeta valinta kiitetään ohjelman käytöstä ja vapautetaan muisti ennen kuin lopetetaan ohjelma return(0); käskyllä.
            freememory(&pRaw);
            return (0);

        default:
            printf("Tuntematon valinta, yritä uudestaan.\n");
            break;
        }
        printf("\n");
    }
}