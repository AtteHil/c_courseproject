
#ifndef LIBRARY_H // Täällä korjattu pelkästään define jää.
#define LIBRARY_H
#define CN 50
#define NU 200
#define LI 20

typedef struct rawdata
{
    char time[CN];
    int week;
    int usage;
    int sun;
    int wind;
    int water;
    int nuclear;
    int combined;
    int heat;
    struct rawdata *pNext;
} RAWDATA;

typedef struct monthresult
{
    int monthnum;
    unsigned long produce;
} MONTHRESULT;

typedef struct result
{
    int count;
    unsigned long sum;
    double average;
    RAWDATA *most;
    RAWDATA *least;
    MONTHRESULT ar[12];
} RESULT;

int printingmenu();

char *getfilename(char *filename, int i);

void readdata(char *filename, RAWDATA **pStart);
RAWDATA *Addnode(char Line[NU]);

void ReadtoResult(RAWDATA *pRaw, RESULT *result);

void printAll(char *filename, RESULT *result);
void printToUser(char printdata[LI][NU]);
void printToFile(char printdata[LI][NU], char *filename);

void freememory(RAWDATA **pStart);

#endif