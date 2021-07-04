/*******************************************************
 * Jméno autora: David Hudák
 * Login autora: xhudak03
 * Projekt: telefonní seznam a vyhledávání v něm
 * Testováno: Windows, Merlin, Linux (Ubuntu 18.04 s prostředím KDE)
 *******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#define MAXN 100 // Konstantní hodnota pro maxima polí
/***
* Položky v seznamu kontaktů (číslo a jméno) nemohou být delší než MAXN
* Počet vyhledaných nemůže být větší než MAXN
***/

enum criticals {EOK, EBAD, EOVER, PRIDEJ, SPATNY};

typedef struct //Struktura obsahující vyhledané kontakty
/***
* char cislo - proměnná připravená pro uložení čísla kontaktu
* char jmeno - proměnná připravená pro uložení jména kontaktu
***/
{
    char cislo[MAXN];
    char jmeno[MAXN];
} seznam;

void vypisVsechnyNalezenePrvky(seznam List[], int n) // Tiskne všechny prvky seznamu
/***
* seznam List[] - vstupní parametr, pole, v němž jsou uložené kontakty, které se mají vytisknout
* int n - počet prvků seznamu
***/
{
    for(int i=0; i<n; i++)
    {
        printf("%s, %s\n", List[i].jmeno, List[i].cislo);
    }
}

int nactiRadek(char pole[]); // Nacita radek do pole
/***
* char pole[] - slouží k ukládání načtených dat
* vrací EOVER, pokud data přetekla
* vrací EOK, pokud byl řádek v pořádku
* vrací EBAD, pokud načítání skončilo EOF
***/

void chyby(int err); // Procedura sloužící k tisknutí typických chyb na výstup stderr
/***
* int err - číslo ekvivalentní k nějaké enum hodnotě
* funkce se dá dále doplňovat
* v případě, že chyba nebyla definována, případně, že funkce byla zavolána s neznámou hodnotou, nic se neděje.
***/

void prepisSouborNaStdout() // Prepisuje stdin na stdout - procedura pro případ, že uživatel žádá vypsat všechny kontakty
{
    int err1, err2, i=0;
    seznam Kontakt;
    while((err1=nactiRadek(Kontakt.jmeno))==EOK&&(err2=nactiRadek(Kontakt.cislo))==EOK) // Čte řádky, dokud se nedojde na konec souboru
    {
        i++;
        printf("%s, %s\n", Kontakt.jmeno, Kontakt.cislo);
    }
    if(i==0)
    {
        printf("Not found\n");
    }
    if(err1==EOVER || err2==EOVER)
    {
        chyby(EOVER);
    }
}

int hledejNecisla(char znaky[])// Hledá počet znaků, která nejsou čísla
/***
* char znaky[] - pole znaků, jehož obsah se kontroluje
* vrací počet znaků různých od <'0', '9'>
***/
{
    int i=0;
    int wrong=0;
    while(znaky[i]!='\0')
    {
        if(znaky[i]<'0' && znaky[i]>'9')
        {
            wrong++;
        }
        i++;
    }
    return wrong;
}

bool najdi(char znak, char pole[]) // Hledá znak v poli znaků
/***
* char znak - hledaný znak
* const char pole[] - konstantní pole, ve kterém se znak hledá
* vrací true, nebo false, pokud byl znak v poli nalezen, nebo nebyl
***/
{
    int i=0;
    while(pole[i]!='\0')
    {
        if(pole[i]==znak)
        {
            return true;
        }
        i++;
    }
    return false;
}

int porovnejKontakt(char parametr[], seznam kontakt) // Zjišťuje, jestli se v daném kontaktu nachází parametr
/***
* char parametr[] - pole znaku obsahujici retezec, kterym se vyhledava v kontaktu
* seznam kontakt - jiz drive inicializovany kontakt, ktery se porovnava s retezcem parametru (obsahuje cislo a jmeno)
* vraci PRIDEJ, pokud došlo ke shodě
* vrací SPATNY, pokud nedošlo ke shodě
***/
{
    char hodnoty[10][6]= {{"0+"}, {"1"}, {"2abc"}, {"3def"}, {"4ghi"}, {"5jkl"}, {"6mno"}, {"7pqrs"}, {"8tuv"}, {"9wxyz"}}; // Tabulka hodnot sloužící k převodu čísel na znaky
    int pom;
    int index;
    pom=0;
    int i=0;
    while(kontakt.jmeno[i]!='\0' && parametr[pom]!='\0' && i<MAXN && pom<MAXN) // Cyklus sloužící k vyhledání kontaktu - cyklus končí nenalezení správného jména, nalezení, nebo přetečení parametru či indexu
    {
        index=(int)parametr[pom]-'0'; // Dochází k převodu znaku čísla na index pro pole (znak se převed
        if(najdi(kontakt.jmeno[i], hodnoty[index])) // funkce najdi hledá znak ze jména v poli hodnoty
        {
            pom++;
        }
        i++;
    }
    if(parametr[pom]=='\0') // Pokud index pom sloužící k indexování parametru ukazuje na hodnotu konce řetězce, pak byl kontakt vyhledán.
    {
        return PRIDEJ;
    }
    i=0;
    pom=0;
    while(kontakt.cislo[i]!='\0' && parametr[pom]!='\0' && pom<MAXN && i<MAXN) // Podobný cyklus jako výše - hledá kontakty podle čísla
    {
        if(kontakt.cislo[i]==parametr[pom])
        {
            pom++;
        }
        i++;
    }
    if(parametr[pom]=='\0') // Pokud index pom sloužící k indexování parametru ukazuje na hodnotu konce řetězce, pak byl kontakt vyhledán. (Stejne jako výše
    {
        return PRIDEJ;
    }
    return SPATNY;
}

void chyby(int err) // Procedura sloužící k tisknutí typických chyb na výstup stderr
/***
* int err - číslo ekvivalentní k nějaké enum hodnotě
* funkce se dá dále doplňovat
* v případě, že chyba nebyla definována, případně že funkce byla zavolána s neznámou hodnotou, nic se neděje.
***/
{
    switch (err)
    {
    case EBAD:
        fprintf(stderr, "Vstup je zrejme spatny.\n");
	break;
    case EOVER:
        fprintf(stderr, "Prilis mnoho dat na vstupu.\n");
	break;
    default:
	fprintf(stderr, "Chyba jeste nebyla definovana.\n");
        break;
    }
}

int nactiRadek(char pole[]) // Nacita radek do pole
/***
* char pole[] - slouží k ukládání načtených dat
* vrací EOVER, pokud data přetekla
* vrací EOK, pokud byl řádek v pořádku
* vrací EBAD, pokud načítání skončilo EOF
***/
{
    int i=0, err;
    while((err=getchar())!=EOF && err!='\n' && i<MAXN)
    {
        pole[i]=tolower(err); // Projekt má být řešen case insensitive, tudíž je třeba znormovat všechny znaky na stejnou velikost (velké A->malé a)
        i++;
    }
    pole[i]='\0';
    if(i==MAXN) // Série hlášení pro uživatele
    {
        return EOVER;
    }
    if(err=='\n')
    {
        return EOK;
    }
    if(err==EOF)
    {
        return EBAD;
    }
    return SPATNY;
}

void push(seznam *List, seznam kontakt)  // Přidává do seznamu kontaktů další kontakt (v projektu se nepoužívají abstraktní datové struktury, takže se jedná pouze o zapsání dat do statického pole
/***
* seznam *List - ukazatel na prvek struktury, do které se mají přepisovat údaje
* seznam kontakt - kontakt, který se má zapsat do *List
***/
{
    int s=0;
    while(kontakt.jmeno[s]!='\0')
    {
        List->jmeno[s]=kontakt.jmeno[s];
        s++;
    }
    List->jmeno[s]='\0';
    s=0;
    while(kontakt.cislo[s]!='\0')
    {
        List->cislo[s]=kontakt.cislo[s];
        s++;
    }
    List->cislo[s]='\0';
}

int nacitejAHledej(seznam List[], char parametr[], int *pocetNalezenych) // Funkce vykonávající načítání dat ze vstupu a jejich vyhodnocování
/***
* seznam List[] - pole struktur, kam se mají zapisovat vyhledané kontakty
* char parametr[] - řetězec, dle kterého se vyhledává ve vstupu
* int *pocetNalezenych - ukazatel vracející počet nalezených kontaktů
* vrací EOK, pokud vše proběhlo jak má
* vrací EOVER, pokud někde došlo k přetečení
* vrací EBAD, pokud došlo k chybě vstupních dat (kontakty končící jménem, nikoliv číslem, je špatný)
***/
{
    int err, kontrola, r=0;
    seznam kontakt;
    bool jedem=true;
    while(jedem)
    {
        err=nactiRadek(kontakt.jmeno); // Načtení jména ze vstupu
        if(err==EOVER)
        {
            return err;
        }
        err=nactiRadek(kontakt.cislo); // Nactení čísla ze vstupu
        if(err==EBAD)
        {
            jedem=false;
        }
        if(err==EOVER)
        {
            return err;
        }
        kontrola=porovnejKontakt(parametr, kontakt); // Zde dochází k porovnání načteného kontaktu a parametru
        if(kontrola==PRIDEJ)
        {
            push(&List[r], kontakt); // Přidává kontakt do seznamu kontaktů
            r++;
        }
        if(r==MAXN) // V případě, že bylo nalezeno MAXN kontaktů, vrací se EOK. Vypíše se pak jen prvních MAXN nalezených.
        {
            *pocetNalezenych=r;
            return EOK;
        }
    }
    *pocetNalezenych=r; // Zaslání skrze ukazatel počtu kontaktů volající funkci
    return EOK;
}

int main(int argc, char *argv[])
{
    seznam List[MAXN];
    int err, pocetNalezenych=0;
    if(argc==1) // Vypsání všech kontaktů
    {
        prepisSouborNaStdout();
        return 0;
    }
    if(hledejNecisla(argv[1])>0) // Pokud se v parametru pro vyhledávání čísel nachází jiné znaky než čísla, tak dojde k ukončení
    {
        chyby(EBAD);
        return EBAD;
    }
    else // Argument vyhledávání byl zadán správně - volá se hledání
    {
        err=nacitejAHledej(List, argv[1], &pocetNalezenych); // Do proměnné pocetNalezenych se vrací počet nalezených kontaktů
        if(err!=EOK)
        {
            chyby(err);
        }
    }
    if(pocetNalezenych==0) // Pokud byl nalezen aspoň jeden kontakt, tak se tisknou všechny nalezené. Pokud ne, tiskne se Not found
    {
        printf("Not found\n");
    }
    else
    {
        vypisVsechnyNalezenePrvky(List, pocetNalezenych);
    }
    return 0;
}

