/********************************************************
* Autor: David Hudák
* Login: xhudak03
* Projekt: proj3, hledáni cesty v bludišti
* Testováno na: Linux, Merlin
********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
  int rows;
  int cols;
  unsigned char *cells;
} Map;

enum errors{EOK, EINIT, EFILE};

int loadAndInitMap(Map *m, FILE *in)
/******
* Funkce inicializující mapu a načítající mapu (předpokládám, že obě činnosti se budou vykonávat zároveň, jelikož
k inicializaci mapy je potřeba jak samotné struktury, tak i počet řádkù a sloupců, které se získávají při načítání mapy.
* Map *m - struktura, do které se mapa ukládá
* FILE *in - vstupní soubor
* Vrací ENUM chybové hodnoty - EFILE pro špatná data v souboru, EINIT pro nemožnost alokovat paměť. EOK pak pro správnou inicializaci a načtení.
******/
{
    int cislo;
    int i=0;
    int err=fscanf(in, "%d %d", &m->rows, &m->cols);
    if(err!=2)
    {
        return EFILE;
    }
    m->cells=malloc(sizeof(unsigned char)*m->rows*m->cols);
    if(m->cells==NULL)
    {
        m->rows=0;
        m->cols=0;
        return EINIT;
    }
    while(i<(m->rows*m->cols) && fscanf(in, "%d", &cislo)==1)
    {
        m->cells[i]=(unsigned char)(cislo&7);
        i++;
    }
    if(i<m->rows*m->cols)
    {
        free(m->cells);
        m->rows=0;
        m->cols=0;
        return EFILE;
    }
    return EOK;
}

void destroyMap(Map *m)
/***
* Dealokuje mapu a změní hodnoty řádkù a sloupcù na 0.
* Vstup Map *m - alokovaná mapa.
***/
{
    free(m->cells);
    m->rows=0;
    m->cols=0;
}

bool isborder(Map *map, int r, int c, int border)
/***
* Map *map - inicializovaná mapa
* int r a int c - poloha dle řádku a sloupce
* int border - dotazovaná hranice - 0 dolní/horní, 1 pravá, 2 levá (modulované hodnoty)
* vrací true, pokud se na dotazované hranice nachází zeď. False, když ne.
***/
{
    if(border%3==0)
    {
        return ((map->cells[(r*map->cols)+c]&4)==4);
    }
    if(border%3==1)
    {
        return ((map->cells[(r*map->cols)+c]&2)==2);
    }
    if(border%3==2)
    {
        return ((map->cells[(r*map->cols)+c]&1)==1);
    }
    return false;
}

int start_border(Map *m, int r, int s, int leftright)
/**
* Map *m - mapa
* int r - vstupní řádek
* int s - vstupní sloupec
* int leftright
* vrací první hranici, která se má následovat (když ještě není znám směr)
* pokud se do buňky nedá vstoupit, vrací -1
**/
{
    if(s==0 && r==0) // Slozite vyjmenovavání všech možností. Viz dokumentace.
    {
        if(isborder(m, r, s, 0), isborder(m, r, s, 2)) return -1;
        if(isborder(m, r, s, 0)) return 1-leftright;
        else return 2-leftright;
    }
    if(s==0 && r==m->rows-1 && r%2==1)
    {
        if(isborder(m, r, s, 0)) return 0+leftright;
        else return 1+leftright;
    }
    if(r==0 && s==m->cols-1 && (r+s)%2==0)
    {
        if(isborder(m, r, s, 0), isborder(m, r, s, 1)) return -1;
        if(isborder(m, r, s, 0)) return 0+(2*leftright);
        else return 2-leftright;
    }
    if(r==m->rows-1 && s==m->cols-1 && (r+s)%2==1)
    {
        if(isborder(m, r, s, 0), isborder(m, r, s, 1)) return -1;
        if(isborder(m, r, s, 0)) return 2+leftright;
        else return 1+leftright;
    }
    if(s==0)
    {
        if(isborder(m, r, s, 2)) return -1;
        if(r%2==0)
        {
            return 1-leftright;
        }
        else
        {
            return 0+leftright;
        }
    }
    if(r==0)
    {
        if(isborder(m, r, s, 0) || s%2==1) return -1;
        return 2-leftright;
    }
    if(r==m->rows-1)
    {
        if(isborder(m, r, s, 0) || s%2==0) return -1;
        return 1+leftright;
    }
    if(s==m->cols-1)
    {
        if(isborder(m, r, s, 1)) return -1;
        if((r+s)%2==0)
        {
            return 0+(2*leftright);
        }
        else
        {
            return 2+leftright;
        }
    }
    return -1;
}
/**
* smer 1 - nahoru
* smer -1 - dolu
* smer 2 - doprava
* smer -2 - doleva
**/
int najdi_hranici(int r, int s, int smer, int leftright)
/**
* int r - aktuální řádek
* int s - aktuální sloupec
* int smer - směr vstupu do buňky
* int leftright - levý/pravý přístup
* vrací hodnotu hranice, která se má následovat
**/
{
    if((r+s)%2==0) // Určení, jestli se jedá o sudou buňku
    {
        if(smer==1) // Směrem nahoru do sudé buňky vstupovat nejde
        {
            return -1;
        }
        if(smer==2)
        {
            return 4-leftright; // leftright nabývá 0 pro pravý přístup, 1 pro levý. Pokud se tedy jedná o pravý přístup, tak všechny leftright jsou 0, pokud ne, tak se dopočítá s pomocí jedničky v leftright.
        }
        if(smer==-2)
        {
            return 3-leftright; // Analogicky jako výše (objasnění viz dokumentace)
        }
        if(smer==-1)
        {
            return 5-leftright;
        }
    }
    else // Větev pro lichou buňku
    {
        if(smer==1)
        {
            return 1+leftright;
        }
        if(smer==2)
        {
            return 0+leftright;
        }
        if(smer==-2)
        {
            return 2+leftright;
        }
        if(smer==-1)
        {
            return -1;
        }
    }
    return -1;
}

int posunSouradniceAHranici(int hranice, int *r, int *s, int leftright)
/**
* int hranice - hranice, přes kterou se přechází
* Map *m - mapa
* int *r - adresa řádku - mění se jako vedlejší efekt
* int *s - adresa sloupce - mění se jako vedlejší efekt (při každém posunu se mění jen jedna souřadnice)
* int leftright - rozlišuje mezi pravým a levým postupem (0/1)
* vrací novou hodnotu hranice
**/
{
    int smer;
    if((*r+*s)%2==0) // Komplikovaně vyjmenovány všechny možnosti, které mohou nastat
    {
        if(hranice%3==0)
        {
            *r=*r-1; // Posunutí o řádek výš
            smer=1; // Smer nahoru (ostatní směry - 1 nahoru, -1 dolů, 2 doprava, -2 doleva
        }
        else if(hranice%3==1)
        {
            *s=*s+1;
            smer=2;
        }
        else if(hranice%3==2)
        {
            *s=*s-1;
            smer=-2;
        }
    }
    else
    {
        if(hranice%3==0)
        {
            *r=*r+1;
            smer=-1;
        }
        else if(hranice%3==1)
        {
            *s=*s+1;
            smer=2;
        }
        else if(hranice%3==2)
        {
            *s=*s-1;
            smer=-2;
        }
    }
    return najdi_hranici(*r, *s, smer, leftright); // Používá funkci start_border ke zjištění přístupu
}

void Ruka(Map *m, int r, int s, int leftright)
/**
* Provádí algoritmus hledání cesty podle pravidla pravé, respektive levé ruky
* Map *m - inicializovaná mapa
* int r a s - souřadnice počátečního řádku a sloupce
* int leftright - 0 pravý přístup, 1 levý přístup.
**/
{
    int hranice=start_border(m, r, s, leftright); // Určení počáteční hranice
    if(hranice==-1)
    {
        printf("Tady nejde vstoupit!\n");
        return;
    }
    while(r<m->rows && r>=0 && s<m->cols && s>=0) // Jede se, dokud nějaký parametr řádku nebo sloupce neskočí mimo bludiště.
    {
        if(isborder(m, r, s, hranice)) // Pokud je na dané hranici zeď, tak se vykonává změna zdi podle pravidla levé, respektive pravé ruky.
        {
            if(((r+s)%2==0 && leftright==0) || ((r+s)%2==1 && leftright==1)) // Posouvání hranic v případě, že se tou současnou nedá projít.
            {
                hranice--;
            }
            else
            {
                hranice++;
            }
        }
        else // Pokud na dané hranici není zeď, dochází k posunu.
        {
            printf("%d,%d\n", r+1, s+1);
            hranice=posunSouradniceAHranici(hranice, &r, &s, leftright);
        }
    }
}

void help()
{
    printf("\nVitam vas v programu na hledani cesty v mape\n\n\n");
    printf("Zadej jako parametry --test a nazev souboru, pokud chces otestovat dany soubor na pouzitelnost v programu.\n");
    printf("Zadej jako prvni parametr --lpath nebo --rpath, pokud chces hledat cestu dle pravidla leve/prave ruky. Pak zadej cislo startovniho radku a sloupce. Nakonec zadej nazev validniho souboru.\n\n");
    printf("Moznost --shortest zatim nebyla implementovana.");
}

void test(char *argv)
{
    FILE *f=fopen(argv, "r");
    if(f==NULL)
    {
        fprintf(stderr, "Prislusny soubor nejde otevrit\n");
        return;
    }
    Map m;
    int err=loadAndInitMap(&m, f);
    if(err==EINIT)
    {
        fprintf(stderr, "Doslo k chybe alokace.\n");
        return;
    }
    if(err==EFILE)
    {
        printf("Invalid\n");
        return;
    }
    for(int i=0; i<m.rows-1; i++) // Projíždí se celé pole buněk dané mapy. Porovnávají se vždy dvě sousední (zleva zprava a shora zdola). Jede se do m.cols-1 apod., protože se vždy kontroluje i+1 apod.
    {
        for(int j=0; j<m.cols-1; j++)
        {
            if((isborder(&m, i, j, 1) && !isborder(&m, i, j+1, 2)) || (!isborder(&m, i, j, 1) && isborder(&m, i, j+1, 2)))
            {
                printf("Invalid\n");
                return;
            }
            if((i+j)%2==1)
            {
                if((isborder(&m, i, j, 0) && !isborder(&m, i+1, j, 0)) || (!isborder(&m, i, j, 0) && isborder(&m, i+1, j, 0)))
                {
                    printf("Invalid\n");
                    return;
                }
            }
        }
    }
    printf("Valid\n");
}

int main(int argc, char *argv[])
{
    if(argc==1 || strcmp("--help", argv[1])==0 )
    {
        help();
        return 0;
    }
    if(argc==3 && strcmp("--test", argv[1])==0)
    {
        test(argv[2]);
        return 0;
    }
    if(argc==5 && ((strcmp("--rpath", argv[1])==0)||(strcmp("--lpath", argv[1])==0)))
    {
        char *pom;
        int r=(int)strtol(argv[2], &pom, 10);
        if(*pom!='\0')
        {
            fprintf(stderr, "Parametr radku byl zadan spatne.\n");
            return EINIT;
        }
        int s=(int)strtol(argv[3], &pom, 10);
        if(*pom!='\0')
        {
            fprintf(stderr, "Parametr radku byl zadan spatne.\n");
            return EINIT;
        }
        FILE *f=fopen(argv[4], "r");
        if(f==NULL)
        {
            fprintf(stderr, "Nepovedlo se otevrit soubor.\n");
            return EFILE;
        }
        Map m;
        int err=loadAndInitMap(&m, f);
        fclose(f);
        if(err==EINIT)
        {
            fprintf(stderr, "Nepovedlo se alokovat pamet.\n");
            return EINIT;
        }
        if(err==EFILE)
        {
            fprintf(stderr,"Soubor ma zrejme zakladni chyby.\n");
            return EFILE;
        }
        if(strcmp("--rpath", argv[1])==0)
        {
            Ruka(&m, r--, s--, 0);
        }
        else
        {
            Ruka(&m, r--, s--, 1);
        }
        destroyMap(&m);
        return 0;
    }
    printf("Zrejme nebylo neco zadano spravne. Tisknu napovedu.\n\n");
    help();
    return 0;
}

/*** rozpracované a zdaleka nestihnuté řešení pro variantu shortest s užitím ternárního stromu
typedef struct strom{
    int r, s;
    struct strom *levy;
    struct strom *pravy;
    struct strom *prostredni;
    struct strom *zpet;
}Strom;

void init(Strom *s)
{
    s->levy=NULL;
    s->prostredni=NULL;
    s->pravy=NULL;
    s->zpet=NULL;
}

void vynuluj(int pom[], int r, int s)
{
    for(int i=0; i<=(r*s); i++)
    {
        pom[i]=0;
    }
}

void printint(int pom[],int r, int s)
{
    for(int i=0; i<=(r*s); i++)
    {
        if(i%s==0)
        {
            printf("\n");
        }
        printf("%d ", pom[i]);
    }
}

void rekurze(Map *m, int r, int s, int lhranice, int phranice, int pommap[])
{
    int i=0;
    int pomr, poms, pomr2,poms2;
    if(pommap[(r*m->cols)+s]==1) return; // Zde by melo nasledovat dealokovani vetve stromu, ktera byla slepa a nikam nevedla.
    pommap[(r*m->cols)+s]=1; // Pomocná mapa pro uchovávání informace, kde uz bylo policko navstiveno
    if(r<m->rows && r>=0 && s<m->cols && s>=0) // Jede se, dokud nějaký parametr řádku nebo sloupce neskočí mimo bludiště.
    {
        while(isborder(m, r, s, lhranice))
        {
            if((r+s)%2==0)
            {
                lhranice++;
            }
            else
            {
                lhranice--;
            }
        }
        while(isborder(m, r, s, phranice))
        {
            if((r+s)%2==0)
            {
                phranice--;
            }
            else
            {
                phranice++;
            }
        }
        if(lhranice==phranice) // Pokud je jen jeden směr, tak by se mělo jet na strom->prostredni, pokud dva, tak na strom->levy a strom->pravy
        {
            pomr=r;
            poms=s;
            // Zde by mela byt alokace noveho ve stylu Strom *novy=malloc(sizeof(novy*));
            lhranice=posunSouradniceAHranici(lhranice, m, &r, &s, 1);
            phranice=posunSouradniceAHranici(phranice, m, &pomr, &poms, 0);
            rekurze(m, r, s, lhranice, phranice, pommap);
        }
        else
        {
            pomr=r;
            poms=s;
            pomr2=r;
            poms2=s;
            // Zde by mely byt alokace Strom *novylevy a Strom *novypravy
            int hranice=lhranice;
            int hranice2=phranice;
            lhranice=posunSouradniceAHranici(hranice, m, &pomr2, &poms2, 1);
            phranice=posunSouradniceAHranici(hranice, m, &pomr, &poms, 0);
            rekurze(m, pomr, poms, lhranice, phranice, pommap);
            pomr=r;
            poms=s;
            pomr2=r;
            poms2=s;
            lhranice=posunSouradniceAHranici(hranice2, m, &pomr2, &pomr, 1);
            phranice=posunSouradniceAHranici(hranice2, m, &pomr, &poms, 0);
            rekurze(m, pomr, poms, lhranice, phranice, pommap);
        }
    }
}

void rekurence(Map *m, int r, int s) // Pomocna funkce
{
    int phranice=start_border(m, r, s, 0);
    int lhranice=start_border(m, r, s, 1);
    int pommap[m->rows*m->cols];
    vynuluj(pommap, m->rows, m->cols);
    rekurze(m, r, s, lhranice, phranice, pommap);
    printint(pommap, m->rows, m->cols);
}***/
