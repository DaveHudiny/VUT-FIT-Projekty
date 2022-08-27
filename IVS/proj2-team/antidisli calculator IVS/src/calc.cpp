#include "calc.h"
#include "math.h"

#define PI 3.14159265359

/* Name - code for mathematical functions in calculator
 *  Date - 8. 4. 2020
 *  Date of most recent edit - 8. 4. 2020
 *  Author - David Hudak
 *  Last edited by - David Hudak
 *  Description - Code for mathematical functions
 */

 /**
  *	@file calc.cpp
  *
  *	@brief Implementation of functions from header (class) file calc.h - mathematical library.
  *	@author David Hudak
  */

double calc::Round(double number, int floatingLength)
{
    if (floatingLength > 0)
    {
        number = round(number);
        int pomnumber = (int)number; // Getting rid of part after floating point
        if (pomnumber % (int)pow(10, floatingLength) >= (int)5 * pow(10, floatingLength - 1))
        {
            number += pow(10, floatingLength); // (wanted part + 1) due the rest of number
        }
        number -= (double)(pomnumber % (int)pow(10, floatingLength)); // Getting rid of unwanted part of number
        return number;
    }
    else if (floatingLength == 0)
    {
        return round(number); // number.thrash -> number.000..
    }
    else
    {
        double pomnumber = number * pow(10, -floatingLength); // integerpart.floatingpart -> integer part flo.atingpart (moving with floating point)
        pomnumber = round(pomnumber); // wanted part.unwanted part -> wanted part.000...
        number = pomnumber / pow(10, -floatingLength);
        return number;
    }

}

double calc::Pow(double number, double quotient)
{
    if (number < 0)
    {
        double res = pow(number, quotient);
        if (isnan(res))
        {
            throw "NaN";
        }
        return res;
    }
    return pow(number, quotient);
}

double calc::Sqrt(double number, double quotient)
{
    if (quotient == 0)
    {
        throw "Invalid quotient of nth root.";
    }
    if (number < 0)
    {
        if (int(quotient) % 2 == 0) // Few operations which starts with multiplication of 2 (ex: 2.1) could be counted, but there had to be be more difficult conditions and user is not stupid. :)
        {
            throw "Invalid";
        }
        return -Pow(-number, 1.0 / quotient);
    }
    return Pow(number, 1. / quotient);
}

double calc::Sinus(double number, bool inRadian)
{
    if (inRadian)
    {
        return sin(number);
    }
    else
    {
        return sin(number * PI / 180);
    }
}

double calc::Cosinus(double number, bool inRadian)
{
    if (inRadian)
    {
        return cos(number);
    }
    else
    {
        return cos(number * PI / 180);
    }
}

double calc::Tangents(double number, bool inRadian)
{
    double err = Cosinus(number, inRadian); // Tangens is defined as sin/cos -> cosinus cannot be zero.
    if (err < 0.0000001 && err > -0.0000001)
    {
        throw 'NaN';
    }
    if (inRadian)
    {
        return tan(number);
    }
    else
    {
        return tan(number * PI / 180);
    }
}

double calc::Cotangents(double number, bool inRadian)
{
    double err = Sinus(number, inRadian); // Cotangens is defined as cos/sin -> sinus cannot be zero.
    if (err < 0.0000001 && err > -0.0000001)
    {
        throw 'NaN';
    }
    if (inRadian)
    {
        return cos(number) / sin(number);
    }
    else
    {
        return cos(number * PI / 180) / sin(number * PI / 180);
    }
}

double calc::Multiply(double number1, double number2)
{
    return number1 * number2;
}

double calc::Divide(double number, double divider)
{
    if (divider == 0.0)
    {
        throw "Division by zero!";
    }
    return number / divider;
}

double calc::Addition(double number1, double number2)
{
    return number1 + number2;
}

double calc::Subtraction(double number1, double number2)
{
    return number1 - number2;
}

double calc::Factorial(int number)
{
    if (number < 0)
    {
        throw "NaN";
    }
    if (number == 0)
    {
        return 1;
    }
    int pom = 1;
    for (int i = 2; i <= number; i++)
    {
        pom *= i;
    }
    return (double)pom;
}

double calc::ArcSin(double number, bool inRadian)
{
    if (number > 1.0 || number < -1.0)
    {
        throw "Invalid input";
    }
    double result = asin(number);
    if (inRadian)
    {
        return result;
    }
    else
    {
        return result * 180 / PI;
    }
}

double calc::ArcCos(double number, bool inRadian)
{
    if (number > 1.0 || number < -1.0)
    {
        throw "Invalid input";
    }
    double result = acos(number);
    if (inRadian)
    {
        return result;
    }
    else
    {
        return result * 180 / PI;
    }
}

double calc::ArcTan(double number, bool inRadian)
{
    double result = atan(number);
    if (inRadian)
    {
        return result;
    }
    else
    {
        return result * 180 / PI;
    }
}

double calc::ArcCotg(double number, bool inRadian)
{
    double result = atan(number);
    result = (PI / 2) - result; // Relation between atan and arcus cotangens
    if (inRadian)
    {
        return result;
    }
    else
    {
        return result * 180 / PI;
    }
}




//
//int main() {
//	std::cout << calc::Subtraction(0, 0) << std::endl;
//	std::cout << "Sample calculator source code" << std::endl;
//	return 0;
//}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

/*** Konec souboru calc.cpp ***/
