#include "controller.h"
#include <string.h>
#include <gtest/gtest.h>


/*Definition of pi*/
#define PI acos(-1.0)


/** Name - test code for mathematical functions in calculator
 *  File - test.cpp
 *  Date - 29. 3. 2020
 *  Date of most recent edit - 2. 4. 2020
 *  Author - Stefan Mracna
 *  Last edited by - Stefan Mracna
 *  Description - Code for testing basic mathematical functions
 */

 /* Class for testing */
class Numbers : public ::testing::Test
{
protected:
    controller testController;
    Numbers()
    {
        testController.Clear();
        testController.SetInRadian(0);

    }
    ~Numbers()
    {
        testController.Clear();
    }
};

/*Tests for addition*/
TEST_F(Numbers, Addition)
{
    EXPECT_EQ(72, calc::Addition(54, 18)); //Test of positive numbers
    EXPECT_EQ(-53, calc::Addition(-42, -11)); //Test of negative numbers
    EXPECT_EQ(72, calc::Addition(18, 54)); //Test of comutativity
    EXPECT_EQ(-53, calc::Addition(-11, -42));
    EXPECT_EQ(12, calc::Addition(54, -42)); //Tests of addition between positive and negative numbers
    EXPECT_EQ(12, calc::Addition(-42, 54));
    EXPECT_EQ(54, calc::Addition(54, 0)); //Test of neutrality of 0
    EXPECT_EQ(-42, calc::Addition(-42, 0));
}

/*Tests for subtraction*/
TEST_F(Numbers, Subtraction)
{
    EXPECT_EQ(36, calc::Subtraction(54, 18)); //Positive numbers
    EXPECT_EQ(-31, calc::Subtraction(-42, -11)); //Negative numbers
    EXPECT_EQ(-36, calc::Subtraction(18, 54)); //Test of comutativity
    EXPECT_EQ(96, calc::Subtraction(54, -42)); //Positive and negative numbers
    EXPECT_EQ(-96, calc::Subtraction(-42, 54));
    EXPECT_EQ(54, calc::Subtraction(54, 0)); //Test of neutrality of 0
    EXPECT_EQ(-42, calc::Subtraction(-42, 0));
}

/*Tests for multiplication*/
TEST_F(Numbers, Multiply)
{
    EXPECT_EQ(972, calc::Multiply(54, 18)); //Positive Numbers
    EXPECT_EQ(462, calc::Multiply(-42, -11)); //Negative numbers
    EXPECT_EQ(972, calc::Multiply(18, 54)); //Comutativity test
    EXPECT_EQ(462, calc::Multiply(-11, -42));
    EXPECT_EQ(-2268, calc::Multiply(54, -42)); //Positive and negative of numbers
    EXPECT_EQ(-2268, calc::Multiply(-42, 54));
    EXPECT_EQ(0, calc::Multiply(54, 0)); //Test of aggresivity of 0
    EXPECT_EQ(0, calc::Multiply(-42, 0));
    EXPECT_EQ(54, calc::Multiply(54, 1)); //Test of neutraity of 1
    EXPECT_EQ(-42, calc::Multiply(-42, 1));
}

/*Tests for division*/
TEST_F(Numbers, Divide)
{
    EXPECT_EQ(3, calc::Divide(54, 18)); //Positive numbers
    EXPECT_EQ(2, calc::Divide(-42, -21)); //Negative numbers
    EXPECT_EQ(0.5, calc::Divide(-21, -42)); //Comutativity test
    EXPECT_EQ(-4, calc::Divide(168, -42)); //Positive and negative numbers
    EXPECT_EQ(-0.25, calc::Divide(-42, 168));
    ASSERT_ANY_THROW(calc::Divide(54, 0)); //Test of division by 0
    ASSERT_ANY_THROW(calc::Divide(-42, 0));
    EXPECT_EQ(54, calc::Divide(54, 1)); //Test of neutrality of 1
    EXPECT_EQ(-42, calc::Divide(-42, 1));
}

/*Tests for trigonometric functions*/
TEST_F(Numbers, Sinus)
{
    /*Tests with degrees*/
    EXPECT_EQ(0, calc::Sinus(0, 0));
    EXPECT_TRUE(0.4999995 < calc::Sinus(30, 0) && calc::Sinus(30, 0) < 0.5000005);
    EXPECT_TRUE(0.9999995 < calc::Sinus(90, 0) && calc::Sinus(90, 0) < 1.0000005);
    EXPECT_TRUE(-0.0000005 < calc::Sinus(180, 0) && calc::Sinus(180, 0) < 0.0000005);
    EXPECT_TRUE(-1.0000005 < calc::Sinus(630, 0) && calc::Sinus(630, 0) < -0.9999995); //Tests of sinuses with additions of 2k*PI
    EXPECT_TRUE(0.9999995 < calc::Sinus(-270, 0) && calc::Sinus(-270, 0) < 1.0000005); //Tests of sunuses with negative numbers
    /*Tests with radians*/
    EXPECT_TRUE(0.707106 < calc::Sinus((PI / 4), 1) && calc::Sinus((PI / 4), 1) < 0.707107);
    EXPECT_TRUE(0.866025 < calc::Sinus((PI / 3), 1) && calc::Sinus((PI / 3), 1) < 0.866026);
    EXPECT_TRUE(-1.0000005 < calc::Sinus((3 * PI / 2), 1) && calc::Sinus((3 * PI / 2), 1) < -0.9999995);
    EXPECT_TRUE(-0.0000005 < calc::Sinus((2 * PI), 1) && calc::Sinus((2 * PI), 1) < 0.0000005);
    EXPECT_TRUE(-0.0000005 < calc::Sinus(-PI, 1) && calc::Sinus(-PI, 1) < 0.0000005); //Tests of sunuses with negative numbers
}

TEST_F(Numbers, Cosinus)
{
    /*Tests with degrees*/
    EXPECT_EQ(1, calc::Cosinus(0, 0));
    EXPECT_TRUE(0.866025 < calc::Cosinus(30, 0) && calc::Cosinus(30, 0) < 0.866026);
    EXPECT_TRUE(-0.0000005 < calc::Cosinus(90, 0) && calc::Cosinus(90, 0) < 0.0000005);
    EXPECT_TRUE(-1.0000005 < calc::Cosinus(180, 0) && calc::Cosinus(180, 0) < -0.9999995);
    EXPECT_TRUE(-0.0000005 < calc::Cosinus(630, 0) && calc::Cosinus(630, 0) < 0.0000005); //Test of cosinuses with additions of 2k*PI
    EXPECT_TRUE(-0.0000005 < calc::Cosinus(-270, 0) && calc::Cosinus(-270, 0) < 0.0000005); //Test of cosinuses with negative numbers
    /*Tests with radians*/
    EXPECT_TRUE(0.707106 < calc::Cosinus((PI / 4), 1) && calc::Cosinus((PI / 4), 1) < 0.707107);
    EXPECT_TRUE(0.4999995 < calc::Cosinus((PI / 3), 1) && calc::Cosinus((PI / 3), 1) < 0.5000005);
    EXPECT_TRUE(-0.0000005 < calc::Cosinus((3 * PI / 2), 1) && calc::Cosinus((3 * PI / 2), 1) < 0.0000005);
    EXPECT_TRUE(0.9999995 < calc::Cosinus((2 * PI), 1) && calc::Cosinus((2 * PI), 1) < 1.0000005);
    EXPECT_TRUE(-1.0000005 < calc::Cosinus(-PI, 1) && calc::Cosinus(-PI, 1) < -0.9999995); //Test of cosinuses with negative numbers
}

TEST_F(Numbers, Tangents)
{
    /*Tests with degrees*/
    EXPECT_EQ(0, calc::Tangents(0, 0));
    EXPECT_TRUE(0.577350 < calc::Tangents(30, 0) && calc::Tangents(30, 0) < 0.577351);
    ASSERT_ANY_THROW(calc::Tangents(90, 0)); //tan(90+k*180) is undefined
    EXPECT_TRUE(-0.0000005 < calc::Tangents(180, 0) && calc::Tangents(180, 0) < 0.0000005);
    ASSERT_ANY_THROW(calc::Tangents(630, 0)); //Test of tangenses with additions of k*PI
    ASSERT_ANY_THROW(calc::Tangents(-270, 0)); //Test of tangenses with negative numbers
    /*Tests with radians*/
    EXPECT_TRUE(0.9999995 < calc::Tangents((PI / 4), 1) && calc::Tangents((PI / 4), 1) < 1.0000005);
    EXPECT_TRUE(1.732050 < calc::Tangents((PI / 3), 1) && calc::Tangents((PI / 3), 1) < 1.732051);
    ASSERT_ANY_THROW(calc::Tangents((3 * PI / 2), 1));
    EXPECT_TRUE(-0.0000005 < calc::Tangents((2 * PI), 1) && calc::Tangents((2 * PI), 1) < 0.0000005);
    EXPECT_TRUE(-0.0000005 < calc::Tangents(-PI, 1) && calc::Tangents(-PI, 1) < 0.0000005); //Test of tangenses with negative numbers
}

TEST_F(Numbers, Cotangents)
{
    /*Tests with degrees*/
    ASSERT_ANY_THROW(calc::Cotangents(0, 0)); //cotan(k*180) is undefined
    EXPECT_TRUE(1.732050 < calc::Cotangents(30, 0) && calc::Cotangents(30, 0) < 1.732051);
    EXPECT_TRUE(-0.0000005 < calc::Cotangents(90, 0) && calc::Cotangents(90, 0) < 0.0000005);
    ASSERT_ANY_THROW(calc::Cotangents(180, 0));
    EXPECT_TRUE(-0.0000005 < calc::Cotangents(630, 0) && calc::Cotangents(630, 0) < 0.0000005); //Test of cotangenses with additions of k*PI
    EXPECT_TRUE(-0.0000005 < calc::Cotangents(-270, 0) && calc::Cotangents(-270, 0) < 0.0000005); //Test of cotangenses with negative numbers
    /*Tests with radians*/
    EXPECT_TRUE(0.9999995 < calc::Cotangents((PI / 4), 1) && calc::Cotangents((PI / 4), 1) < 1.0000005);
    EXPECT_TRUE(0.577350 < calc::Cotangents((PI / 3), 1) && calc::Cotangents((PI / 3), 1) < 0.577351);
    EXPECT_TRUE(-0.0000005 < calc::Cotangents((3 * PI / 2), 1) && calc::Cotangents((3 * PI / 2), 1) < 0.0000005);
    ASSERT_ANY_THROW(calc::Cotangents((2 * PI), 1));
    ASSERT_ANY_THROW(calc::Cotangents(-PI, 1)); //Test of cotangenses with negative numbers
}

/*Test for arcsin*/
TEST_F(Numbers, ArcSin)
{
    /*Tests with degrees*/
    EXPECT_EQ(0, calc::ArcSin(0, 0));
    EXPECT_TRUE(29 < calc::ArcSin(0.5, 0) && calc::ArcSin(0.5, 0) < 31);
    EXPECT_TRUE(59 < calc::ArcSin(0.86603, 0) && calc::ArcSin(0.86603, 0) < 61);
    EXPECT_TRUE(-91 < calc::ArcSin(-1, 0) && calc::ArcSin(-1, 0) < -89);
    EXPECT_TRUE(-61 < calc::ArcSin(-0.86603, 0) && calc::ArcSin(-0.86603, 0) < -59);
    EXPECT_TRUE(-31 < calc::ArcSin(-0.5, 0) && calc::ArcSin(-0.5, 0) < -29);
    EXPECT_TRUE(-46 < calc::ArcSin(-0.707107, 0) && calc::ArcSin(-0.707107, 0) < -44);
    EXPECT_ANY_THROW(calc::ArcSin(-1.5, 0));
    EXPECT_ANY_THROW(calc::ArcSin(1.5, 0));
    /*Tests with radians*/
    EXPECT_TRUE(((PI/4)-0.5) < calc::ArcSin(0.707107, 1) && calc::ArcSin(0.707107, 1) < ((PI / 4) + 0.5));
    EXPECT_TRUE(((PI / 2) - 0.5) < calc::ArcSin(1, 1) && calc::ArcSin(1, 1) < ((PI / 2) + 0.5));
}

TEST_F(Numbers, ArcCos)
{
    /*Tests with degrees*/
    EXPECT_TRUE(89 < calc::ArcCos(0, 0) && calc::ArcCos(0, 0) < 91);
    EXPECT_TRUE(59 < calc::ArcCos(0.5, 0) && calc::ArcCos(0.5, 0) < 61);
    EXPECT_TRUE(29 < calc::ArcCos(0.86603, 0) && calc::ArcCos(0.86603, 0) < 31);
    EXPECT_TRUE(179 < calc::ArcCos(-1, 0) && calc::ArcCos(-1, 0) < 181);
    EXPECT_TRUE(149 < calc::ArcCos(-0.86603, 0) && calc::ArcCos(-0.86603, 0) < 151);
    EXPECT_TRUE(119 < calc::ArcCos(-0.5, 0) && calc::ArcCos(-0.5, 0) < 121);
    EXPECT_TRUE(134 < calc::ArcCos(-0.707107, 0) && calc::ArcCos(-0.707107, 0) < 136);
    EXPECT_ANY_THROW(calc::ArcCos(-1.5, 0));
    EXPECT_ANY_THROW(calc::ArcCos(1.5, 0));
    /*Tests with radians*/
    EXPECT_TRUE(((PI / 4) - 0.5) < calc::ArcSin(0.707107, 1) && calc::ArcSin(0.707107, 1) < ((PI / 4) + 0.5));
    EXPECT_TRUE(0 < calc::ArcSin(1, 1) && calc::ArcSin(1, 1) < ((PI / 2) + 0.5));
}

TEST_F(Numbers, ArcTan)
{
    /*Tests with degrees*/
    EXPECT_EQ(0, calc::ArcTan(0, 0));
    EXPECT_TRUE(29 < calc::ArcTan(0.577350, 0) && calc::ArcTan(0.577350, 0) < 31);
    EXPECT_TRUE(59 < calc::ArcTan(1.73205, 0) && calc::ArcTan(1.73205, 0) < 61);
    EXPECT_TRUE(-46 < calc::ArcTan(-1, 0) && calc::ArcTan(-1, 0) < -44);
    EXPECT_TRUE(-61 < calc::ArcTan(-1.73205, 0) && calc::ArcTan(-1.73205, 0) < -59);
    EXPECT_TRUE(-31 < calc::ArcTan(-0.577350, 0) && calc::ArcTan(-0.577350, 0) < -29);
    /*Tests with radians*/
    EXPECT_TRUE((PI / 4)-0.5 < calc::ArcTan(1, 1) && calc::ArcTan(1, 1) < (PI / 4) + 0.5);
}
TEST_F(Numbers, ArcCotg)
{
    /*Tests with degrees*/
    EXPECT_TRUE(89 < calc::ArcCotg(0, 0) && calc::ArcCotg(0, 0) < 91);
    EXPECT_TRUE(59 < calc::ArcCotg(0.577350, 0) && calc::ArcCotg(0.577350, 0) < 61);
    EXPECT_TRUE(29 < calc::ArcCotg(1.73205, 0) && calc::ArcCotg(1.73205, 0) < 31);
    EXPECT_TRUE(134 < calc::ArcCotg(-1, 0) && calc::ArcCotg(-1, 0) < 136);
    EXPECT_TRUE(149 < calc::ArcCotg(-1.73205, 0) && calc::ArcCotg(-1.73205, 0) < 151);
    EXPECT_TRUE(119 < calc::ArcCotg(-0.577350, 0) && calc::ArcCotg(-0.577350, 0) < 121);
    /*Tests with radians*/
    EXPECT_TRUE((PI / 4) - 0.5 < calc::ArcCotg(1, 1) && calc::ArcCotg(1, 1) < (PI / 4) + 0.5);
}



/*Tests for powers*/
TEST_F(Numbers, Pow)
{
    EXPECT_EQ(4, calc::Pow(2, 2)); //Test with positive base and positive quotient
    EXPECT_EQ(0.5, calc::Pow(2, -1)); //Tests with positive base and negative quotient
    EXPECT_EQ(0.125, calc::Pow(2, -3));
    EXPECT_EQ(2, calc::Pow(2, 1)); //Test of neutrality of 1 quotient
    EXPECT_EQ(1, calc::Pow(2, 0)); //Test of aggressivity of 0 quotient
    EXPECT_EQ(25, calc::Pow(5, 2)); //Test of different base than 2
    EXPECT_EQ(1, calc::Pow(1, 1992)); //Test of base of 1
    EXPECT_EQ(0, calc::Pow(0, 1992)); //Test of base of 0
    EXPECT_EQ(1, calc::Pow(-1, 2)); //Tests of base of -1
    EXPECT_EQ(-1, calc::Pow(-1, 3));
    EXPECT_EQ(2, calc::Pow(4, 0.5)); //Tests of decimal quotients
    EXPECT_EQ(0.5, calc::Pow(4, -0.5));
    EXPECT_ANY_THROW(calc::Pow(-1, 2.5)); //Tests of negative bases and decimal quotients
    EXPECT_ANY_THROW(calc::Pow(-1, -2.5));
    EXPECT_EQ(-1, calc::Pow(-1, 0.2)); //Test of odd roots of negative base

}

/*Tests for roots*/
TEST_F(Numbers, Sqrt)
{
    EXPECT_EQ(2, calc::Sqrt(4, 2)); //Tests of square roots
    EXPECT_EQ(5, calc::Sqrt(25, 2));
    EXPECT_EQ(0, calc::Sqrt(0, 1888)); //Test of root of 0
    EXPECT_EQ(1, calc::Sqrt(1, 1997)); //Test of root of 1
    EXPECT_EQ(2, calc::Sqrt(256, 8)); //Tests of higher-level roots
    EXPECT_EQ(3, calc::Sqrt(81, 4));
    EXPECT_ANY_THROW(calc::Sqrt(-1, 2)); //Tests whether the function will throw an error on a negative base
    EXPECT_ANY_THROW(calc::Sqrt(5, 0)); //Tests whether the function will throw an error on a 0-quotient root
    EXPECT_EQ(-1, calc::Sqrt(-1, 3)); //Tests whether the function will count the root of a negative base with an odd number
    EXPECT_EQ(0.5, calc::Sqrt(4, -2)); //Test of negative-quotient root
    EXPECT_EQ(4, calc::Sqrt(2, 0.5)); //Tests of decimal-quotient roots
    EXPECT_EQ(0.25, calc::Sqrt(2, -0.5));

}

/*Tests for rounding of numbers*/
TEST_F(Numbers, Round)
{
    EXPECT_EQ(100, calc::Round(121, 2)); //Rounding of numbers to a positive floatingLength
    EXPECT_EQ(100, calc::Round(81, 2));
    EXPECT_EQ(500, calc::Round(497, 2));
    EXPECT_EQ(500, calc::Round(450, 2));
    EXPECT_EQ(1, calc::Round(1.421, 0)); //Rounding of numbers to a floatingLength of 0
    EXPECT_EQ(0.5, calc::Round(0.522, -1)); //Rounding of numbers to a negative floatingLength
    EXPECT_EQ(0.006, calc::Round(0.00594, -3));

}

/*Tests of factorial calculation*/
TEST_F(Numbers, Factorial)
{
    EXPECT_EQ(1, calc::Factorial(1));
    EXPECT_EQ(2, calc::Factorial(2));
    EXPECT_EQ(24, calc::Factorial(4));
    EXPECT_EQ(1, calc::Factorial(0));
    EXPECT_ANY_THROW(calc::Factorial(-1));
}

/*Tests of conversion of a string to mathematical expressions*/
TEST_F(Numbers, Solve)
{
    EXPECT_EQ(3, testController.Solve("q(9, 2)"));
    EXPECT_EQ(47.5, testController.Solve("1+6-8/4+5*10")); //Test of generic arithmetic string
    EXPECT_EQ(54.5, testController.Solve("1.1+6.7-8/4+5.5*10")); //Test of generic arithmetic decimal string
    EXPECT_EQ(64, testController.Solve("8*4/2*4")); //Priority tests of / and * operations
    EXPECT_EQ(16, testController.Solve("8*4/4*2"));
    EXPECT_EQ(4, testController.Solve("1+6-8+5")); //Test of + and - operations
    EXPECT_EQ(1, testController.Solve("1")); //Test of strings without operations
    EXPECT_EQ(0.0, testController.Solve("0"));
    EXPECT_EQ(12, testController.Solve("012")); //Test of zeroes in front of numbers
    EXPECT_EQ(1.1, testController.Solve("1.1"));
    EXPECT_EQ(1.1, testController.Solve("1.10")); //Test of zeroes after decimal numbers
    EXPECT_EQ(1.1, testController.Solve("(5/10)*16/1+3/10")); //Tests of bracket priorities in a string
    EXPECT_EQ(9, testController.Solve("5/(1+4)"));
    EXPECT_EQ(4, testController.Solve("(1+6)-(8+5)"));
    EXPECT_EQ(-2, testController.Solve("(1-6)+(8-5)"));
    EXPECT_EQ(64, testController.Solve("(8*4)/(2*4)"));
    EXPECT_EQ(1, testController.Solve("(8/4)*(2/4)"));
    EXPECT_EQ(4, testController.Solve("2^2")); //Tests of pow and sqrt functions
    EXPECT_EQ(2, testController.Solve("q(4, 2)"));
    EXPECT_EQ(4, testController.Solve("2^q(4, 2)"));
    EXPECT_EQ(4, testController.Solve("q(4, 2)^2"));
    EXPECT_EQ(0.5, testController.Solve("q(4, 2)/q(16, 2)"));
    EXPECT_EQ(256, testController.Solve("2^2*4^2")); //Tests of pow and sqrt functions in combination with other operations
    EXPECT_EQ(-1, testController.Solve("q(4, 2)-q(9, 2)"));
    EXPECT_ANY_THROW(testController.Solve("plz calculator gib me 30k ventilators")); //Tests of gibberish strings
    EXPECT_ANY_THROW(testController.Solve("2+2 is 4, minus 1 that´s 3 quick maths"));
    EXPECT_ANY_THROW(testController.Solve("()")); //Tests of incorrectly inserted brackets
    EXPECT_ANY_THROW(testController.Solve("("));
    EXPECT_ANY_THROW(testController.Solve(")"));
    EXPECT_ANY_THROW(testController.Solve("4+(4+(2^2)"));
    EXPECT_ANY_THROW(testController.Solve("4+(4+(2^2)))"));
    EXPECT_NO_THROW(testController.Solve("4+(4+(2^2))"));
    EXPECT_ANY_THROW(testController.Solve("(4+4)+2)^2"));
    EXPECT_ANY_THROW(testController.Solve("(((4+4)+2)^2"));
    EXPECT_NO_THROW(testController.Solve("((4+4)+2)^2"));
    /*Tests of goniometric functions with degrees as entry values*/
    EXPECT_TRUE(0.99995 < testController.Solve("s(90)") && testController.Solve("s(90)") < 1.00005); //Simple tests of strings with goniometric functions
    EXPECT_TRUE(-0.000005 < testController.Solve("c(90)") && testController.Solve("c(90)") < 0.000005);
    EXPECT_TRUE(-0.000005 < testController.Solve("t(0)") && testController.Solve("t(0)") < 0.000005);
    EXPECT_TRUE(-0.000005 < testController.Solve("g(90)") && testController.Solve("g(90)") < 0.000005);
    EXPECT_TRUE(-0.000005 < testController.Solve("s(50*0)") && testController.Solve("s(50*0)") < 0.000005); //Tests of goniometric functions in combination with other operations
    EXPECT_TRUE(0.9999995 < testController.Solve("s(89+c(0))") && testController.Solve("s(89+c(0))") < 1.0000005);
    EXPECT_TRUE(0.24445 < testController.Solve("s(30)^2") && testController.Solve("s(30)^2") < 0.25005);
    /* EXPECT_TRUE(0.4999995 < testController.Solve("s((3^3)+3)") && testController.Solve("s((3^3)+3)") < 0.5000005);*/
    EXPECT_ANY_THROW(testController.Solve("s()")); //Tests of incorrectly inserted goniometric functions
    EXPECT_ANY_THROW(testController.Solve("s"));
    testController.SetInRadian(true); //Tests of goniometric functions with radians as entry values
    EXPECT_TRUE(0.99995 < testController.Solve("s(1.57)") && testController.Solve("s(1.57)") < 1.00005);
    EXPECT_TRUE(-0.005 < testController.Solve("c(1.57)") && testController.Solve("c(1.57)") < 0.005);
    EXPECT_EQ(0.0, testController.Solve("t(0)"));
    EXPECT_TRUE(-0.005 < testController.Solve("g(1.57)") && testController.Solve("g(1.57)") < 0.005);
    EXPECT_EQ(0.0, testController.Solve("s(PI*0)"));
    EXPECT_TRUE(-0.0000005 < testController.Solve("t(2.14159+c(0))") && testController.Solve("t(2.14159+c(0))") < 0.0000005);
    EXPECT_TRUE(0.2444445 < testController.Solve("c(1.05)^2") && testController.Solve("c(1.05)^2") < 0.25000005);
}

/*Tests the addition of numbers and operands to the string which will be calculated*/
TEST_F(Numbers, AddToEquation)
{
    testController.Clear();
    std::string testString = testController.AddNumberToEquation("5");
    EXPECT_EQ("5", testString); //Basic test, which adds a number to string
    testString = testController.AddOperatorToEquation("+");
    EXPECT_EQ("5+", testString); //Basic test, which adds a binary operator to string
    testString = testController.AddNumberToEquation("2");
    EXPECT_EQ("5+2", testString);
    testString = testController.AddOperatorToEquation("-");
    EXPECT_EQ("5+2-", testString);
    testString = testController.AddOperatorToEquation("s");
    EXPECT_EQ("5+2-s(", testString); //Test of addition of unary operator
    testString = testController.AddNumberToEquation("4");
    EXPECT_EQ("5+2-s(4", testString);
    testString = testController.AddOperatorToEquation(")");
    EXPECT_EQ("5+2-s(4)", testString);
    testString = testController.AddOperatorToEquation("*");
    EXPECT_EQ("5+2-s(4)*", testString);
    testString = testController.AddOperatorToEquation("(");
    EXPECT_EQ("5+2-s(4)*(", testString); //Test of addition of brackets
    testString = testController.AddOperatorToEquation("c");
    EXPECT_EQ("5+2-s(4)*(c(", testString);
    testString = testController.AddNumberToEquation("7");
    testString = testController.AddOperatorToEquation(".");
    EXPECT_EQ("5+2-s(4)*(c(7.", testString);
    testString = testController.AddNumberToEquation("5");
    testString = testController.AddOperatorToEquation(")");
    EXPECT_EQ("5+2-s(4)*(c(7.5)", testString);
    testString = testController.AddOperatorToEquation("/");
    EXPECT_EQ("5+2-s(4)*(c(7.5)/", testString);
    testString = testController.AddOperatorToEquation("t");
    EXPECT_EQ("5+2-s(4)*(c(7.5)/t(", testString);
    testString = testController.AddNumberToEquation("1"); //Tests of multiple-digit numbers
    EXPECT_EQ("5+2-s(4)*(c(7.5)/t(1", testString);
    testString = testController.AddNumberToEquation("2");
    EXPECT_EQ("5+2-s(4)*(c(7.5)/t(12", testString);
    testString = testController.AddNumberToEquation("0");
    EXPECT_EQ("5+2-s(4)*(c(7.5)/t(120", testString);
    testString = testController.AddOperatorToEquation(")");
    EXPECT_EQ("5+2-s(4)*(c(7.5)/t(120)", testString);
    testString = testController.AddOperatorToEquation(")");
    EXPECT_EQ("5+2-s(4)*(c(7.5)/t(120))", testString);
    testString = testController.AddOperatorToEquation("^");
    EXPECT_EQ("5+2-s(4)*(c(7.5)/t(120))^", testString);
    testString = testController.AddOperatorToEquation("q");
    EXPECT_EQ("5+2-s(4)*(c(7.5)/t(120))^q(", testString); //Tests of combinations of unary and binary operands with prefix notations
    testString = testController.AddNumberToEquation("3");
    EXPECT_EQ("5+2-s(4)*(c(7.5)/t(120))^q(3", testString);
    testController.AddOperatorToEquation(",");
    testString = testController.AddOperatorToEquation("r");
    EXPECT_EQ("5+2-s(4)*(c(7.5)/t(120))^q(3, r(", testString);
    testString = testController.AddOperatorToEquation("!");
    EXPECT_EQ("5+2-s(4)*(c(7.5)/t(120))^q(3, r(!(", testString);
    testString = testController.AddNumberToEquation("6");
    EXPECT_EQ("5+2-s(4)*(c(7.5)/t(120))^q(3, r(!(6", testString);
    testString = testController.AddOperatorToEquation(")");
    testString=testController.AddOperatorToEquation(",");
    EXPECT_EQ("5+2-s(4)*(c(7.5)/t(120))^q(3, r(!(6), ", testString);
    testString = testController.AddNumberToEquation("3");
    EXPECT_EQ("5+2-s(4)*(c(7.5)/t(120))^q(3, r(!(6), 3", testString);
    testString = testController.AddOperatorToEquation(")");
    EXPECT_EQ("5+2-s(4)*(c(7.5)/t(120))^q(3, r(!(6), 3)", testString);
    testString = testController.AddOperatorToEquation(")");
    EXPECT_EQ("5+2-s(4)*(c(7.5)/t(120))^q(3, r(!(6), 3))", testString);
    //End of tests of combinations of unary and binary operands with prefix notations
    testController.Clear();
    EXPECT_NO_THROW(testController.AddOperatorToEquation("c")); //Checks if an unary operand throws errors
    testController.Clear();
    EXPECT_ANY_THROW(testController.AddOperatorToEquation("*")); //Checks if a binary operand with a infix notation throws an error
    testController.Clear();
    EXPECT_NO_THROW(testController.AddOperatorToEquation("-")); //Checks if a minus operand throws an error
    testController.Clear();
    testController.AddNumberToEquation("8");
    testController.AddNumberToEquation("9");
    //Checks if consecutively entered binary operands with a infix notation throw an error (excludes - and + operands)
    testController.AddOperatorToEquation("*");
    EXPECT_ANY_THROW(testController.AddOperatorToEquation("/"));
    testController.Clear();
    testController.AddNumberToEquation("8");
    testController.AddNumberToEquation("9");
    //Checks if consecutively entered - and + operands throw an error
    testController.AddOperatorToEquation("+");
    testString = testController.AddOperatorToEquation("-");
    EXPECT_EQ("89-", testString);
    testString = testController.AddOperatorToEquation("-");
    EXPECT_EQ("89-", testString);
    testString = testController.AddOperatorToEquation("+");
    EXPECT_EQ("89+", testString);

}


/*Tests the clearing of the buffer*/
TEST_F(Numbers, Clear)
{
    testController.Clear();
    EXPECT_EQ("", testController.GetBuffer());
    testController.Clear();
    EXPECT_EQ("", testController.GetBuffer()); //Tests whether a buffer is clear without previous input
    testController.AddNumberToEquation("8");
    testController.AddOperatorToEquation("+");
    testController.AddNumberToEquation("6");
    testController.Clear();
    EXPECT_EQ("", testController.GetBuffer()); //Tests whether a buffer is clear with previous input
}

/*Tests whether the buffer will be fetched correctly*/
TEST_F(Numbers, GetBuffer)
{
    testController.Clear();
    testController.AddNumberToEquation("5");
    testController.AddOperatorToEquation("+");
    testController.AddNumberToEquation("2");
    EXPECT_EQ("5+2", testController.GetBuffer());
    testController.AddOperatorToEquation("/");
    testController.AddOperatorToEquation("g");
    testController.AddNumberToEquation("8");
    testController.AddOperatorToEquation(")");
    EXPECT_EQ("5+2/g(8)", testController.GetBuffer());
    testController.Clear();
}

/*Tests whether the buffer for graphical output will be fetched accordingly*/
TEST_F(Numbers, GetImprovedBuffer)
{
    testController.Clear();
    testController.AddOperatorToEquation("s");
    testController.AddNumberToEquation("9");
    testController.AddNumberToEquation("0");
    testController.AddOperatorToEquation(")");
    ASSERT_EQ("sin(90)", testController.GetImprovedBuffer()); //Tests conversion of strings including sin
    testController.AddOperatorToEquation("+");
    testController.AddOperatorToEquation("c");
    testController.AddNumberToEquation("9");
    testController.AddNumberToEquation("0");
    testController.AddOperatorToEquation(")");
    ASSERT_EQ("sin(90)+cos(90)", testController.GetImprovedBuffer()); //Tests conversion of strings including cos
    testController.AddOperatorToEquation("/");
    testController.AddOperatorToEquation("t");
    testController.AddOperatorToEquation("g");
    testController.AddNumberToEquation("4");
    testController.AddOperatorToEquation(")");
    testController.AddOperatorToEquation(")");
    ASSERT_EQ("sin(90)+cos(90)/tan(cotg(4))", testController.GetImprovedBuffer()); //Tests conversion of strings including tan and cotg
    testController.AddOperatorToEquation("*");
    testController.AddOperatorToEquation("r");
    testController.AddOperatorToEquation("!");
    testController.AddNumberToEquation("1");
    testController.AddOperatorToEquation(")");
    testController.AddOperatorToEquation(",");
    testController.AddOperatorToEquation("q");
    testController.AddNumberToEquation("9");
    testController.AddNumberToEquation("3");
    testController.AddOperatorToEquation(",");
    testController.AddNumberToEquation("6");
    testController.AddOperatorToEquation(")");
    testController.AddOperatorToEquation(")");
    ASSERT_EQ("sin(90)+cos(90)/tan(cotg(4))*round(!(1), sqrt(93, 6))", testController.GetImprovedBuffer());
    //Tests conversion of strings including round and sqrt
    testController.Clear();
}

/*Tests whether the SetInRadian function sets the inRadian attribute accordingly*/
TEST_F(Numbers, RadianTests)
{
    testController.SetInRadian(false); //Tests if a radian with the value "false" will be evaluated as such
    EXPECT_FALSE(testController.GetInRadian());
    testController.SetInRadian(true); //Tests if a radian with the value "false" will be evaluated as such
    EXPECT_TRUE(testController.GetInRadian());
}