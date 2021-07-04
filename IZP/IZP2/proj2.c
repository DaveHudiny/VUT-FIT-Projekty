/************************************************************************
* Projekt - Projekt 2, bisekční výpočet pracovního bodu na diodě
* Jméno: David Hudák
* Login: xhudak03
* Testováno na - Windows, Linux (Fedora), Merlin
* Nalezený problém - někde na cestě Merlin, Wis, Windows, Linux se rozbila diakritika. Pokusil jsem se vše opravit, ale možná někde něco zůstalo.
************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <errno.h>

#define I_0 1.0E-12
#define U_T 0.0258563
/***
* Konstanty:
* * I_0 - saturační proud
* * U_T - konstanta vodivosti diody za normální teploty (souvisí s polovodiči)
* * MAXTIME - Maximální počet korkù bisekce v případě, že uživatel má nerealistické požadavky
***/

double Xfunction(double U_p) // returns current I_p
/***
* double U_p - napětí na diodì
* return double - vypočítaný proud dle vzorce s konstantami a dosazením hodnoty napětí
* tzv. Shockleyhova rovnice
***/
{
    return I_0*(exp(U_p/U_T)-1);
}

enum {EOK, EINPUT, EINPUT1, EINPUT2, EINPUT3, ENAN}; // Výètové hodnoty pro ošetøení chybových hlášení

double diode2(double U_0, double R, double eps, double (*function)(double)) // returns voltage Up on diode
/***
* double U_0 - vstupní napětí ze zdroje
* double R - odpor rezistoru
* double eps - požadovaná přesnost
* double (*function)(double) - ukazatel na funkci (předpokládá se funkce na výpočet proudu na diodě)
* return double - vrací hodnotu Up na diodě
* Způsob řešení pomocí rozdílů proudů je podle všeho neplatný, tak tato funkce zbývá pouze jako bonusová.
***/
{
    double U_p=U_0; // Počáteèní hodnota napětí U_p
    double I_p=function(U_p); // Počáteční hodnota proudu I_p na diodě
    double I_R=0; // Počáteèní proud na rezistoru (předpokládá se 0, jelikož platí druhý Kirchhofùv zákon)
    double step=U_p/2, dev; // Počáteční hodnota kroku
    double steptest;
    while(fabs(dev=(I_p-I_R))>eps) // Cyklus končí při dosažení dostatečně přesného výsledku
    {
        if(dev>0) // Pokud je rozdíl (dev) větší než 0, pak je proud na diodě větší než na rezistoru => napětí na diodě se musí snížit o krok (step)
        {
            U_p-=step; // Odhalená chyba - tento postup zřejmě způsobí, že se pro napěti inf se nespočítá výsledek inf, ale -nan
        }
        else// Opačná situace oproti situaci výše
        {
            U_p+=step;
        }
        I_p=function(U_p); // Nový výpočet proudù a kroku
        I_R=(U_0-U_p)/R;
        steptest=step;
        step/=2;
        if(steptest==step)
            return U_p;
    }
    return U_p;
}



double bisekce(double xl, double xr, double U_0, double R, double (*function)(double), double eps)
/***
* double xl - levé ohraničení intervalu
* double xr - pravé ohraničení intervalu
* double U_0 - vstupní napìtí ze zdroje
* double R - odpor rezistoru
* double eps - požadovaná pøesnost
* double (*function)(double) - ukazatel na funkci (pøedpokládá se funkce na výpoèet proudu na diodì)
* return double - vrací hodnotu Up na diodě
* Správné řešení tohoto projektu
***/
{
    double U_p=(xl + xr)/2;
    double U_ptest=0;
    double I_p=function(U_p); // Poèáteèní hodnota proudu I_p na diodě
    double I_R=(U_0-U_p)/R; // Počátečbí hodnota proudu I_R na rezistoru
    while(fabs(U_p-U_ptest)>eps) // Cyklus provádějící se do doby, než rozdíl U_p a předchozího U_p není menší než přesnost eps.
    {
        if(I_p>I_R) // Pokud je na diodě větší proud než na rezistoru, pravé ohraničení se posouvá. Pokud je to naopak, tak levé.
        {
            xr=U_p;
        }
        else
        {
            xl=U_p;
        }
        U_ptest=U_p;
        U_p=(xr+xl)/2; // Určení nového středu
        if(U_ptest==U_p) // Test bránící zacyklení z důvodu přílišného půlení intervalu
        {
            return U_p;
        }
        I_p=function(U_p); // Iterovaný výpočet proudů
        I_R=(U_0-U_p)/R;
    }
    return U_p;
}

void help() // Funkce volaná tehdy, když uživatel neví, co dělat
{
    printf("./proj2\n");
    printf("param 1 - voltage of source; unit - Volts\n");
    printf("param 2 - value of resistor; unit - Ohms\n");
    printf("param 3 - absolute accuracy (e.g. 0.0001)\n");
}

void errors(char *text, int err)
/***
* char *text - ukazatel na zprávu/chybu/pøebyteèná data, která se mají tisknout
*
***/
{
    if(text!=NULL) // Tisk přebyteèných dat na vstupních parametrech
    {
        fprintf(stderr, "Error text - 1%100s1\n", text);
    }
    switch(err) // Chybová hlášení - lze doplnit
    {
        case EINPUT:
            printf("error: invalid arguments\n");
            break;
        case ENAN:
            fprintf(stderr, "Cannot find solution (NAN).\n");
            break;
        case EINPUT3:
            fprintf(stderr ,"\nI cannot do this!\n\n");
            break;
        default:
            break;
    }
}

double diode(double U_0, double R, double (*function)(double), double eps)  // Obalovací funkce - slouží pouze k lepšímu splnění zadání, jinak je zbytečná.
{
    return bisekce(0, U_0, U_0, R, function, eps);
}

int main(int argc, char *argv[])
{
    if(argc<4 || strcmp(argv[1], "-h")==0) // Nápověda
    {
        help();
        return 0;
    }
    char *pom=NULL;
    double U_0=strtod(argv[1], &pom); // Načtení dat ze vstupních argumentù - strtod přetypovává řetìzce na čísla typu double
    if(*pom!='\0') // Testování správného načtení argumentù
    {
        errors(pom, EINPUT);
        return EINPUT1;
    }
    double R=strtod(argv[2], &pom);
    if(*pom!='\0')
    {
        errors(pom, EINPUT);
        return EINPUT2;
    }
    double eps=strtod(argv[3], &pom);
    if(*pom!='\0')
    {
        errors(pom, EINPUT);
        return EINPUT3;
    }
    if(U_0<=0 || R<=0) // Pro takovéto hodnoty nemá smysl počítat
    {
	errors(NULL, EINPUT);
	return EINPUT;
    }
    if(eps<=0) // Dokonalá přesnost není v tomto programu povolena
    {
        errors("Are you crazy?!\n", EINPUT3);
        return EINPUT3;
    }
    double U_p=diode(U_0, R, &Xfunction, eps); // Výpočet pracovního napětí na diodě
    printf("Up=%g V\nIp=%g A\n", U_p, Xfunction(U_p)); // Tisk pracovního bodu
    //U_p = diode2(U_0, R, eps, &Xfunction); Alternativni funkce pro vypočet pracovniho bodu diody, podle mě lepší, podle zadání neplatný
    //printf("Up=%g V\nIp=%g A\n", U_p, Xfunction(U_p));
    return EOK;
}
