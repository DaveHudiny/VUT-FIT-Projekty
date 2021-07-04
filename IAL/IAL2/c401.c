
/* c401.c: **********************************************************}
{* Téma: Rekurzivní implementace operací nad BVS
**                                         Vytvořil: Petr Přikryl, listopad 1994
**                                         Úpravy: Andrea Němcová, prosinec 1995
**                                                      Petr Přikryl, duben 1996
**                                                   Petr Přikryl, listopad 1997
**                                  Převod do jazyka C: Martin Tuček, říjen 2005
**                                         Úpravy: Bohuslav Křena, listopad 2009
**                                                 Karel Masařík, říjen 2013
**                                                 Radek Hranický 2014-2018
**
** Implementujte rekurzivním způsobem operace nad binárním vyhledávacím
** stromem (BVS; v angličtině BST - Binary Search Tree).
**
** Klíčem uzlu stromu je jeden znak (obecně jím může být cokoliv, podle
** čeho se vyhledává). Užitečným (vyhledávaným) obsahem je zde integer.
** Uzly s menším klíčem leží vlevo, uzly s větším klíčem leží ve stromu
** vpravo. Využijte dynamického přidělování paměti.
** Rekurzivním způsobem implementujte následující funkce:
**
**   BSTInit ...... inicializace vyhledávacího stromu
**   BSTSearch .... vyhledávání hodnoty uzlu zadaného klíčem
**   BSTInsert .... vkládání nové hodnoty
**   BSTDelete .... zrušení uzlu se zadaným klíčem
**   BSTDispose ... zrušení celého stromu
**
** ADT BVS je reprezentován kořenovým ukazatelem stromu (typ tBSTNodePtr).
** Uzel stromu (struktura typu tBSTNode) obsahuje klíč (typu char), podle
** kterého se ve stromu vyhledává, vlastní obsah uzlu (pro jednoduchost
** typu int) a ukazatel na levý a pravý podstrom (LPtr a RPtr). Přesnou definici typů
** naleznete v souboru c401.h.
**
** Pozor! Je třeba správně rozlišovat, kdy použít dereferenční operátor *
** (typicky při modifikaci) a kdy budeme pracovat pouze se samotným ukazatelem
** (např. při vyhledávání). V tomto příkladu vám napoví prototypy funkcí.
** Pokud pracujeme s ukazatelem na ukazatel, použijeme dereferenci.
**/

#include "c401.h"
int solved;

void BSTInit (tBSTNodePtr *RootPtr) {
/*   -------
** Funkce provede počáteční inicializaci stromu před jeho prvním použitím.
**
** Ověřit, zda byl již strom předaný přes RootPtr inicializován, nelze,
** protože před první inicializací má ukazatel nedefinovanou (tedy libovolnou)
** hodnotu. Programátor využívající ADT BVS tedy musí zajistit, aby inicializace
** byla volána pouze jednou, a to před vlastní prací s BVS. Provedení
** inicializace nad neprázdným stromem by totiž mohlo vést ke ztrátě přístupu
** k dynamicky alokované paměti (tzv. "memory leak").
**
** Všimněte si, že se v hlavičce objevuje typ ukazatel na ukazatel.
** Proto je třeba při přiřazení přes RootPtr použít dereferenční operátor *.
** Ten bude použit i ve funkcích BSTDelete, BSTInsert a BSTDispose.
**/



	 *RootPtr=NULL;		  /* V případě řešení smažte tento řádek! */

}

int BSTSearch (tBSTNodePtr RootPtr, char K, int *Content)	{
/*  ---------
** Funkce vyhledá uzel v BVS s klíčem K.
**
** Pokud je takový nalezen, vrací funkce hodnotu TRUE a v proměnné Content se
** vrací obsah příslušného uzlu.´Pokud příslušný uzel není nalezen, vrací funkce
** hodnotu FALSE a obsah proměnné Content není definován (nic do ní proto
** nepřiřazujte).
**
** Při vyhledávání v binárním stromu bychom typicky použili cyklus ukončený
** testem dosažení listu nebo nalezení uzlu s klíčem K. V tomto případě ale
** problém řešte rekurzivním volání této funkce, přičemž nedeklarujte žádnou
** pomocnou funkci.
**/
    if(RootPtr==NULL)
    {
        return FALSE;
    }
    else
    {
        if(RootPtr->Key==K)
        {
            *Content=RootPtr->BSTNodeCont;
            return TRUE;
        }
        if(RootPtr->Key>K)
        {
            return BSTSearch(RootPtr->LPtr, K, Content);
        }
        else
        {
            return BSTSearch(RootPtr->RPtr, K, Content);
        }
    }


        /* V případě řešení smažte tento řádek! */

}


void BSTInsert (tBSTNodePtr* RootPtr, char K, int Content)	{
/*   ---------
** Vloží do stromu RootPtr hodnotu Content s klíčem K.
**
** Pokud již uzel se zadaným klíčem ve stromu existuje, bude obsah uzlu
** s klíčem K nahrazen novou hodnotou. Pokud bude do stromu vložen nový
** uzel, bude vložen vždy jako list stromu.
**
** Funkci implementujte rekurzivně. Nedeklarujte žádnou pomocnou funkci.
**
** Rekurzivní implementace je méně efektivní, protože se při každém
** rekurzivním zanoření ukládá na zásobník obsah uzlu (zde integer).
** Nerekurzivní varianta by v tomto případě byla efektivnější jak z hlediska
** rychlosti, tak z hlediska paměťových nároků. Zde jde ale o školní
** příklad, na kterém si chceme ukázat eleganci rekurzivního zápisu.
**/
    if(*RootPtr==NULL)
    {
        struct tBSTNode *pom=malloc(sizeof(struct tBSTNode));
        if(pom==NULL)
        {
            return;
        }
        pom->BSTNodeCont=Content;
        pom->Key=K;
        pom->LPtr=NULL;
        pom->RPtr=NULL;
        *RootPtr=pom;
    }
    if(*RootPtr!=NULL && (*RootPtr)->Key==K)
    {
        (*RootPtr)->BSTNodeCont=Content;
        return;
    }
    if((*RootPtr)->Key>K)
    {
        if((*RootPtr)->LPtr!=NULL)
        {
            BSTInsert(&((*RootPtr)->LPtr), K, Content);
        }
        else
        {
            struct tBSTNode *pom=malloc(sizeof(struct tBSTNode));
            if(pom==NULL)
            {
                return;
            }
            pom->BSTNodeCont=Content;
            pom->Key=K;
            pom->LPtr=NULL;
            pom->RPtr=NULL;
            (*RootPtr)->LPtr=pom;
        }
    }
    else
    {
        if((*RootPtr)->RPtr!=NULL)
        {
            BSTInsert(&((*RootPtr)->RPtr), K, Content);
        }
        else
        {
            struct tBSTNode *pom=malloc(sizeof(struct tBSTNode));
            if(pom==NULL)
            {
                return;
            }
            pom->BSTNodeCont=Content;
            pom->Key=K;
            pom->LPtr=NULL;
            pom->RPtr=NULL;
            (*RootPtr)->RPtr=pom;
        }
    }

		  /* V případě řešení smažte tento řádek! */

}

void ReplaceByRightmost (tBSTNodePtr PtrReplaced, tBSTNodePtr *RootPtr) {
/*   ------------------
** Pomocná funkce pro vyhledání, přesun a uvolnění nejpravějšího uzlu.
**
** Ukazatel PtrReplaced ukazuje na uzel, do kterého bude přesunuta hodnota
** nejpravějšího uzlu v podstromu, který je určen ukazatelem RootPtr.
** Předpokládá se, že hodnota ukazatele RootPtr nebude NULL (zajistěte to
** testováním před volání této funkce). Tuto funkci implementujte rekurzivně.
**
** Tato pomocná funkce bude použita dále. Než ji začnete implementovat,
** přečtěte si komentář k funkci BSTDelete().
**/
    if((*RootPtr)->RPtr!=NULL)
    {
        ReplaceByRightmost(PtrReplaced, &(*RootPtr)->RPtr);
    }
    else
    {
        PtrReplaced->BSTNodeCont=(*RootPtr)->BSTNodeCont;
        PtrReplaced->Key=(*RootPtr)->Key;
        struct tBSTNode *pom;
        pom=*RootPtr;
        *RootPtr=(*RootPtr)->LPtr;
        free(pom);
        *RootPtr=NULL;
    }
            /* V případě řešení smažte tento řádek! */

}

void BSTDelete (tBSTNodePtr *RootPtr, char K) 		{
/*   ---------
** Zruší uzel stromu, který obsahuje klíč K.
**
** Pokud uzel se zadaným klíčem neexistuje, nedělá funkce nic.
** Pokud má rušený uzel jen jeden podstrom, pak jej zdědí otec rušeného uzlu.
** Pokud má rušený uzel oba podstromy, pak je rušený uzel nahrazen nejpravějším
** uzlem levého podstromu. Pozor! Nejpravější uzel nemusí být listem.
**
** Tuto funkci implementujte rekurzivně s využitím dříve deklarované
** pomocné funkce ReplaceByRightmost.
**/
    if(*RootPtr==NULL)
    {
        return;
    }
    else if((*RootPtr)->Key==K)
    {
        if((*RootPtr)->LPtr==NULL && (*RootPtr)->RPtr==NULL)
        {
            free(*(RootPtr));
            *RootPtr = NULL;
        }
        else if((*RootPtr)->LPtr==NULL)
        {
            struct tBSTNode *pom=*RootPtr;
            *RootPtr=pom->RPtr;
            free(pom);
        }
        else if((*RootPtr)->RPtr==NULL)
        {
            struct tBSTNode *pom=*RootPtr;
            *RootPtr=pom->LPtr;
            free(pom);
        }
        else
        {
            if((*RootPtr)->LPtr->RPtr==NULL)
            {
                (*RootPtr)->Key=(*RootPtr)->LPtr->Key;
                (*RootPtr)->BSTNodeCont=(*RootPtr)->LPtr->BSTNodeCont;
                struct tBSTNode *pom=(*RootPtr)->LPtr;
                (*RootPtr)->LPtr=(*RootPtr)->LPtr->LPtr;
                free(pom);
            }
            else
            {
                ReplaceByRightmost(*RootPtr, &(*RootPtr)->LPtr);
            }
        }
    }
    else
    {
        if((*RootPtr)->Key>K)
        {
            BSTDelete(&(*RootPtr)->LPtr, K);
        }
        else
        {
            BSTDelete(&(*RootPtr)->RPtr, K);
        }
    }
    /* V případě řešení smažte tento řádek! */

}

void BSTDispose (tBSTNodePtr *RootPtr) {
/*   ----------
** Zruší celý binární vyhledávací strom a korektně uvolní paměť.
**
** Po zrušení se bude BVS nacházet ve stejném stavu, jako se nacházel po
** inicializaci. Tuto funkci implementujte rekurzivně bez deklarování pomocné
** funkce.
**/
    if((*RootPtr)!=NULL)
    {
        BSTDispose(&(*RootPtr)->RPtr);
        BSTDispose(&(*RootPtr)->LPtr);
        free((*RootPtr));
        *RootPtr=NULL;
    }

	 		  /* V případě řešení smažte tento řádek! */

}

/* konec c401.c */

