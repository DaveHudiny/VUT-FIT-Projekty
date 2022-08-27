//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     David Hudak <xhudak03@stud.fit.vutbr.cz>
// $Date:       $2020-02-22
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author David Hudák
 *
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"

#include "red_black_tree.h"

/** //============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy Red-Black Tree, testujte nasledujici:
// 1. Verejne rozhrani stromu
//    - InsertNode/DeleteNode a FindNode
//    - Chovani techto metod testuje pro prazdny i neprazdny strom.
// 2. Axiomy (tedy vzdy platne vlastnosti) Red-Black Tree:
//    - Vsechny listove uzly stromu jsou *VZDY* cerne.
//    - Kazdy cerveny uzel muze mit *POUZE* cerne potomky.
//    - Vsechny cesty od kazdeho listoveho uzlu ke koreni stromu obsahuji
//      *STEJNY* pocet cernych uzlu.
//============================================================================//*/

class NonEmptyTree:public::testing::Test
{
    protected:
        BinaryTree binaryTree;

        virtual void SetUp() // Nastavení neprázdného stromu.
        {
            int hodnoty[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
            for(int i = 0; i < sizeof(hodnoty); ++i)
            {
                binaryTree.InsertNode(hodnoty[i]);
            }
        }
};

class EmptyTree:public::testing::Test
{
    protected:
        BinaryTree emptyTree;
};

class TreeAxioms:public::testing::Test
{
    protected:
        BinaryTree axiomTree;

        virtual void SetUp()
        {
            int hodnoty[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}; //
            for(int i = 0; i < sizeof(hodnoty); ++i)
            {
                axiomTree.InsertNode(hodnoty[i]);
            }
        }

        std::vector<Node_t *> pole;
};

TEST_F(EmptyTree, FindNode)
{
    EXPECT_TRUE(emptyTree.FindNode(1) == NULL); // V ničem se nedá najít něco.
}

TEST_F(EmptyTree, RemoveNode)
{
    EXPECT_FALSE(emptyTree.DeleteNode(0)); // Z ničeho nejde odebrat něco.
}

TEST_F(EmptyTree, InsertNode) // Test vkládání
{
    std::pair<bool, BinaryTree::Node_t *> vlozeno = emptyTree.InsertNode(0);
    std::pair<bool, BinaryTree::Node_t *> nelze = emptyTree.InsertNode(0);
    EXPECT_TRUE(vlozeno.first);
    EXPECT_FALSE(nelze.first);
    EXPECT_EQ(vlozeno.second, nelze.second); // Ukazetele ukazují na shodný prvek ve stromě, jelikož prvek nelze nešlo vložit.
}

TEST_F(NonEmptyTree, InsertNode) // Test vkládání
{
    std::pair<bool, BinaryTree::Node_t *> vlozeno = binaryTree.InsertNode(100); // Prvek tam ještě není.
    std::pair<bool, BinaryTree::Node_t *> nelze = binaryTree.InsertNode(0); // 0 tam už je.
    EXPECT_TRUE(vlozeno.first);
    EXPECT_FALSE(nelze.first);
}

TEST_F(NonEmptyTree, FindNode) // Test hledání.
{
    EXPECT_TRUE(binaryTree.FindNode(1));
    EXPECT_FALSE(binaryTree.FindNode(100)); // 100 není ve stromě.
}

TEST_F(NonEmptyTree, RemoveNode) // Test odebrání
{
    EXPECT_TRUE(binaryTree.DeleteNode(1));
    EXPECT_FALSE(binaryTree.DeleteNode(1)); // Nelze odebrat stejnou věc dvakrát.
    EXPECT_FALSE(binaryTree.DeleteNode(100));
}



TEST_F(TreeAxioms, Axiom1)
{
    axiomTree.GetLeafNodes(pole); // Zisk listů,
    for(int i=0; i<sizeof(pole); i++) // Průchod všemi listy.
    {
        EXPECT_EQ(pole[i]->color,BLACK);
    }
}

TEST_F(TreeAxioms, Axiom2)
{
    axiomTree.GetAllNodes(pole); // Zisk všech prvků.
    for(int i=0; i<sizeof(pole); i++) // Průchod všemi prvky.
    {
        if(pole[i]->color==RED) // Červené prvky mají jen černé potomky.
        {
            if(pole[i]->pLeft!=NULL)
            {
                EXPECT_EQ(pole[i]->pLeft->color,BLACK);
            }
            if(pole[i]->pRight!=NULL)
            {
                EXPECT_EQ(pole[i]->pRight->color,BLACK);
            }
        }
    }
}

TEST_F(TreeAxioms, Axiom3)
{
    int pocet=0, pocetKonkr=0;
    bool prvni=true;
    axiomTree.GetLeafNodes(pole); // Zisk listů.
    Node_t *pom;
    for(int i=0; i<sizeof(pole); i++) // Průchod všemi listy
    {
        pom=pole[i];
        pocetKonkr=0;
        while(pom!=axiomTree.GetRoot()) // Průchod až ke kořeni
        {
            if(pom->color==BLACK)
            {
                if(prvni)
                {
                    pocet++;
                }
                pocetKonkr++;
            }
            pom=pom->pParent;
        }
        prvni=false;
        EXPECT_EQ(pocet, pocetKonkr);
    }
}
/*** Konec souboru black_box_tests.cpp ***/
