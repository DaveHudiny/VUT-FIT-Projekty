//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - Tests suite
//
// $NoKeywords: $ivs_project_1 $white_box_code.cpp
// $Author:     David Hudak <xhudak03@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author David Hudak
 *
 * @brief Implementace testu prace s maticemi.
 */

#include "gtest/gtest.h"
#include "white_box_code.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy operaci nad maticemi. Cilem testovani je:
// 1. Dosahnout maximalniho pokryti kodu (white_box_code.cpp) testy.
// 2. Overit spravne chovani operaci nad maticemi v zavislosti na rozmerech
//    matic.
//============================================================================//

class MatrixTest : public ::testing::Test
{
protected:
    std::vector<std::vector<double>> vektor3x3= {{10,20,30},{40,50,60},{70,80,90}};
    std::vector<std::vector<double>> vektor2x2= {{10,20},{40,50}};
    std::vector<double> vektor = {3, 2, 1};
    std::vector<std::vector<double>> vektorEq= {{1, 0, 0},{0, 1, 0},{0, 0, 1}};
    Matrix *NonEmptyMatrix= new Matrix(3, 3); // Původně plánováno kvůli destruktoru...
    Matrix EmptyMatrix;
};


TEST_F(MatrixTest, Constructor) // Test konstruktoru(ů)
{
    EXPECT_NO_THROW(new Matrix(1, 1)); // Klasická matice
    EXPECT_NO_THROW(new Matrix); // Matice bez parametrů
    EXPECT_ANY_THROW(new Matrix(1, 0)); // Nečtvercová matice - ilegální
    EXPECT_ANY_THROW(new Matrix(0, 1));
    EXPECT_ANY_THROW(new Matrix(0, 0)); // Moc malá matice
    EXPECT_EQ(0, EmptyMatrix.get(0, 0)); // Definováno na počátku na 0
    Matrix pom(1,1); // Test nuly v pozici 0, 0 při inicializaci.
    EXPECT_EQ(0, pom.get(0, 0));
}

TEST_F(MatrixTest, Get) // Test funkce get
{
    NonEmptyMatrix->set({{0,1,2},{3,4,5},{6,7,8}});
    EXPECT_ANY_THROW(EmptyMatrix.get(2, 0));  // Nelze vstoupit - malá matice
    EXPECT_ANY_THROW(EmptyMatrix.get(0, 2));
    EXPECT_ANY_THROW(EmptyMatrix.get(2, 2));
    EXPECT_EQ(0, EmptyMatrix.get(0, 0)); // Redundantní test, přesto zařazen do této kategorie.
    EXPECT_EQ(8.0, NonEmptyMatrix->get(2,2)); // Test vkládání
    EXPECT_ANY_THROW(NonEmptyMatrix->get(4, 4)); // Špatná pozice u matice větší než 1x1
    EXPECT_ANY_THROW(NonEmptyMatrix->get(4, 0));
    EXPECT_ANY_THROW(NonEmptyMatrix->get(0, 4));
}

TEST_F(MatrixTest, Set) // Testy funkce set
{
    EXPECT_TRUE(EmptyMatrix.set(0, 0, 5.0)); // Testy nastavení jedné proměnné
    EXPECT_EQ(EmptyMatrix.get(0, 0), 5.0);
    EXPECT_FALSE(EmptyMatrix.set(1, 1, 5.0));
    EXPECT_FALSE(EmptyMatrix.set(0, 1, 5.0));
    EXPECT_FALSE(EmptyMatrix.set(1, 0, 5.0));
    EXPECT_TRUE(NonEmptyMatrix->set(1, 1, 10.0));
    EXPECT_EQ(NonEmptyMatrix->get(1, 1), 10.0);
}

TEST_F(MatrixTest, Set_vector) // Testy funkce set s parametrem vektoru
{
    EXPECT_TRUE(NonEmptyMatrix->set(vektor3x3)); // Testy vektoru do matice
    EXPECT_FALSE(EmptyMatrix.set(vektor3x3)); // Test vložení špatného vektoru do špatné matice
    EXPECT_EQ(10, NonEmptyMatrix->get(0, 0));
    EXPECT_FALSE(NonEmptyMatrix->set({{0, 0, 0}, {1, 2, 3}})); // Špatný vektor do správné matice
}

TEST_F(MatrixTest, Destructor)
{
    EXPECT_NO_THROW(delete (new Matrix));
}

TEST_F(MatrixTest, OperatorEq)
{
    EXPECT_ANY_THROW(EmptyMatrix == *NonEmptyMatrix); // Matice s různými velikostmi
    Matrix matice1(3, 3), matice2(3,3);
    matice1.set(vektor3x3);
    matice2.set(vektor3x3);
    EXPECT_TRUE(matice1 == matice2); // Stejne matice se stejnými parametry.
    matice1.set(1, 1, 0);
    EXPECT_FALSE(matice1 == matice2); // Stejně velké matice s různými parametry.
}

TEST_F(MatrixTest, OperatorPlus)
{
    EXPECT_ANY_THROW(EmptyMatrix + *NonEmptyMatrix); // Součet dvou rozdílně velkých matic.
    Matrix matice1(3, 3), matice2(3,3);
    matice1.set(vektor3x3);
    matice2.set(vektor3x3);
    EXPECT_NO_THROW(matice1 = matice1 + matice2);
    EXPECT_EQ(20, matice1.get(0, 0)); // Kontrola výsledku
}

TEST_F(MatrixTest, OperatorMatrixMultiplication) // Násobení matic
{
    EXPECT_ANY_THROW(EmptyMatrix * *NonEmptyMatrix); // Dvě rozdílně velké matice
    Matrix matice1(2, 2), matice2(2,2);
    matice1.set(vektor2x2);
    matice2.set(vektor2x2);
    EXPECT_NO_THROW(matice1 = matice1*matice2);
    EXPECT_EQ(3300, matice1.get(1,1));
}

TEST_F(MatrixTest, OperatorConstMultiplication) // Násobení konstantou
{
    Matrix matice1(2, 2);
    matice1.set(vektor2x2);
    EXPECT_NO_THROW(matice1 = matice1 * 5);
    EXPECT_EQ(50, matice1.get(0, 0));
}

TEST_F(MatrixTest, SolveEquation) // Test řešení rovnic
{
    std::vector<double> pomvec= {0, 0, 0};
    EXPECT_ANY_THROW(EmptyMatrix.solveEquation(pomvec)); // Špatná velikost vektoru.
    EXPECT_ANY_THROW((new Matrix(2, 3))->solveEquation(pomvec)); // Špatná velikost matice.
    NonEmptyMatrix->set(vektor3x3);
    EXPECT_ANY_THROW(NonEmptyMatrix->solveEquation(pomvec)); // Neřešitelná matice - determinant je 0.
    NonEmptyMatrix->set(vektorEq);
    EXPECT_NO_THROW(pomvec=NonEmptyMatrix->solveEquation(vektor)); // Řešitelné
    EXPECT_EQ(3, pomvec[0]); // Kontrola výsledků
    EXPECT_EQ(2, pomvec[1]);
    EXPECT_EQ(1, pomvec[2]);
    Matrix mat2x2(2,2);
    mat2x2.set({{1,0},{0,1}});
    EXPECT_NO_THROW((mat2x2).solveEquation({0, 0})); // Jednoduchá rovnice
    Matrix mat5x5(5,5);
    mat5x5.set({{1,0,0,0,0},{0,1,0,0,0},{0,0,1,0,0},{0,0,0,1,0},{0,0,0,0,1}});
    EXPECT_NO_THROW(mat5x5.solveEquation({0, 0, 0, 0, 0})); // Determinant matice větši než 3 - speciální případ.
    EmptyMatrix.set(0, 0, 1);
    EXPECT_NO_THROW(EmptyMatrix.solveEquation({0})); // Jednoduchá rovnice
}

/*** Konec souboru white_box_tests.cpp ***/
