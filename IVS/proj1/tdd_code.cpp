//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - priority queue code
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     David Hudák <xhudak03@stud.fit.vutbr.cz>
// $Date:       $2020-02-20
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author David Hudák
 *
 * @brief Implementace metod tridy prioritni fronty.
 */

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <time.h>

using namespace std;
#include "tdd_code.h"

//============================================================================//
// ** ZDE DOPLNTE IMPLEMENTACI **
//
// Zde doplnte implementaci verejneho rozhrani prioritni fronty (Priority Queue)
// 1. Verejne rozhrani fronty specifikovane v: tdd_code.h (sekce "public:")
//    - Konstruktor (PriorityQueue()), Destruktor (~PriorityQueue())
//    - Metody Insert/Remove/Find a GetHead
//    - Pripadne vase metody definovane v tdd_code.h (sekce "protected:")
//
// Cilem je dosahnout plne funkcni implementace prioritni fronty implementovane
// pomoci tzv. "double-linked list", ktera bude splnovat dodane testy
// (tdd_tests.cpp).
//============================================================================//

/**
* Konstruktor
* Alokuje novy kořen fronty
* Nastaví frontu jako prázdnou
**/
PriorityQueue::PriorityQueue()
{
    root=new Root;
    root->first=NULL;
}

/**
* Destruktor
* Uvolňuje paměť zabranou frontou.
**/
PriorityQueue::~PriorityQueue()
{
    if(root->first==NULL) // Strom je prazdny
    {
        delete root;
        return;
    }
    Element_t *nicitel;
    Element_t *pom=root->first;
    while(pom->pNext!=NULL) // Přesune pom na konec fronty.
    {
        pom=pom->pNext;
    }
    while(pom!=NULL) // Cyklus procházející frontu od konce, postupně ničí prvky.
    {
        nicitel=pom;
        pom=pom->pPrev;
        delete nicitel;
    }
    delete root;
}

/**
* Funkce pro přístup ke kořeni fronty.
**/
PriorityQueue::Element_t *PriorityQueue::GetHead()
{
    return root->first;
}

/**
* Funkce na hledání ve frontě.
* Vrací NULL, pokud nebylo nic nalezeno, jinak ukazatel na nalezený uzel.
**/
PriorityQueue::Element_t *PriorityQueue::Find(int value)
{
    if(root->first==NULL) // Prazdná fronta
    {
        return NULL;
    }
    Element_t *pom;
    pom=root->first;
    if(pom->value==value)// Hledaná je první hodnota
    {
        return pom;
    }
    while(pom!=NULL && value>pom->value) // Hledá, dokud nenarazí na větší prvek, nebo nedorazí na konec.
    {
        pom=pom->pNext;
    }
    if(pom==NULL) // Nebylo nic nalezeno
    {
        return NULL;
    }
    if(value==pom->value) // V pom je hledaná hodnota
    {
        return pom;
    }
    return NULL;
}

/**
* Pomocná funkce pro tvorbu nového prvku
* Alokuje paměť pro nový prvek a vrací na něj ukazatel.
*
**/
PriorityQueue::Element_t *PriorityQueue::NewElement(int value)
{
    Element_t *novy=new Element_t;
    /** if(novy==NULL) Možné ošetření chybových stavů.
    {
        throw(std::runtime_error);
        return NULL;
    }*/
    novy->pNext=NULL;
    novy->pPrev=NULL;
    novy->value=value;
    return novy;
}


/**
* Funkce na vkládání prvků do fronty.
* Vkládaná hodnota je zařazena dle pořadí.
**/
void PriorityQueue::Insert(int value)
{
    Element_t *novy=NewElement(value);
    /**
    * Možnost ošetřit situaci, že novy ukazuje na NULL.
    **/
    if(root->first==NULL)
    {
        root->first=novy;
        return;
    }
    Element_t *pom=root->first;
    while(pom->value<value && pom->pNext!=NULL) // Hledá místo, kam zařadit prvek
    {
        pom=pom->pNext;
    }
    if(pom->pNext==NULL && pom->value<value) // Jsme na konci
    {
        pom->pNext=novy;
        novy->pPrev=pom;
        return;
    }
    novy->pNext=pom; // Novy vkládáme před pom.
    if(pom->pPrev==NULL) // Začátek fronty.
    {
        root->first=novy;
    }
    else // Někde ve frontě
    {
        pom->pPrev->pNext=novy;
    }
    novy->pPrev=pom->pPrev;
    pom->pPrev=novy;
    return;
}

/**
* Funkce odebírá prvek z fronty.
* Pokud prvek nebyl nalezen, vrací se false, jinak true
**/
bool PriorityQueue::Remove(int value)
{
    Element_t *pom=root->first; // Půlka funkce by se dala obejít s pomocí pom=queue.find(), ale z důvodu testování jednotek toto řešení nebude použito (poznámka autora - tuto funkci jsem debuggoval opravdu dlouho).
    if(pom==NULL)
    {
        return false;
    }
    while(pom!=NULL && value>pom->value) // Hledání prvku
    {
        pom=pom->pNext;
    }
    if(pom==NULL) return false; // Prvek ve frontě není.
    if(pom->value==value)
    {
        if(pom->pPrev==NULL) // První prvek
        {
            if(pom->pNext!=NULL) // Je prvek ve frontě sám?
            {
                root->first=pom->pNext;
                pom->pNext->pPrev=NULL;
            }
            else // Je
            {
                root->first=NULL;
            }
        }
        else if(pom->pNext==NULL) // Odebíraný prvek je poslední ve frontě.
        {
            pom->pPrev->pNext=NULL;
        }
        else // Odebíraný prvek je někde uvnitř fronty.
        {
            pom->pPrev->pNext=pom->pNext;
            pom->pNext->pPrev=pom->pPrev;
        }
        delete pom;
        return true;
    }
    return false;
}

/*** Konec souboru tdd_code.cpp ***/
