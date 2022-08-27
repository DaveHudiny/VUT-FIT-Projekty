/**
 *	@file controller.cpp
 *
 *	@brief File where are implemented all functions from controller.h
 *	@author David Hudak
 */


#include "controller.h"
#include <string>
#include <iostream>

#define NOTFOUND -1

controller::controller()
{
    inRadian = 0;
    Clear();
}

controller::~controller()
{
    Clear();
}

/*double controller::_solve(std::string equation, int l, int r)
{
    if (l > r)
    {
        return 0;
    }
    int inside = 0;
    int prior = NOTFOUND;
    int insideness = INT_MAX;
    bool addsub = false, powb=false;
    std::string lower = "+-";
    std::string higher = "/*";
    for (unsigned i = l; i <= r; i++)
    {
        if (trigonometry.find(equation[i])!= std::string::npos)
        {
            if (equation[i + 1] != '(')
            {
                throw "Missing bracket";
            }
            int rbracket = i + 2;
            int insidetrig = 1;
            while (rbracket <= r && insidetrig != 0)
            {
                if (equation[rbracket] == '(')
                {
                    insidetrig++;
                }
                if (equation[rbracket] == ')')
                {
                    insidetrig--;
                }
                rbracket++;
            }
            if (insidetrig != 0)
            {
                throw "Missing ending bracket.";
            }
            double trires;
            switch (equation[i])
            {
            case 's':
                trires = calc::Sinus(_solve(equation, i + 2, rbracket-1), inRadian);
                break;
            case 'c':
                trires = calc::Cosinus(_solve(equation, i + 2, rbracket-1), inRadian);
                break;
            case 't':
                trires = calc::Tangents(_solve(equation, i + 2, rbracket-1), inRadian);
                break;
            case 'g':
                trires = calc::Cotangents(_solve(equation, i + 2, rbracket-1), inRadian);
                break;
            case '!':
                trires = calc::Factorial(_solve(equation, i + 2, rbracket - 1));
            }
            equation.erase(i, rbracket);
            equation.insert(i, std::to_string(trires));
        }
        if (equation[i]=='q')
        {
            if (equation[i+1] != '(')
            {
                throw "Missing bracket.";
            }
            int half=i+2;
            int isqrt = 0;
            while (half <= r && equation[half] != ',')
            {
                if ('(' == equation[half])
                {
                    isqrt++;
                }
                if (')' == equation[half])
                {
                    isqrt--;
                }
                half++;
            }
            if (equation[half] != ',' || isqrt != 0)
            {
                if (isqrt != 0)
                {
                    throw "Debil.";
                }
                throw "Missing comma.";
            }
            int afhalf = half + 1;
            int isqrt2 = 1;
            while(equation[afhalf] <= r && isqrt != 0)
            {
                if ('(' == equation[half])
                {
                    isqrt++;
                }
                if (')' == equation[half])
                {
                    isqrt--;
                }
                afhalf++;
            }
            if (isqrt != 0)
            {
                throw "Missing ending bracket";
            }
            double ressqrt = calc::Sqrt(_solve(equation, i + 1, half - 1), _solve(equation, half + 1, afhalf - 1));
            equation.erase(i, afhalf);
            equation.insert(i, std::to_string(ressqrt));
        }
        if (equation[i] == '(')
        {
            inside++;
        }
        if (equation[i] == ')')
        {
            inside--;
        }
        if (equation[i] == '^' && (inside < insideness || (inside == insideness && !powb)))
        {
            powb = true;
            insideness = inside;
            prior = i;
        }
        if (lower.find(equation[i]) != std::string::npos && (inside < insideness || (inside == insideness && !addsub)))
        {
            prior = i;
            insideness = inside;
            addsub = true;
        }
        if (higher.find(equation[i]) != std::string::npos)
        {
            if (prior == NOTFOUND || (inside < insideness))
            {
                prior = i;
                insideness = inside;
            }
        }
    }
    if (prior != NOTFOUND)
    {
        int newl = l;
        int newM = prior - 1;
        /*while(newl<equation.size() && newM > 0 && equation[newl]=='(' && equation[newM]==')')
        {
            newl++;
            newM--;
            if(newl>newM)
            {
                return 0;
            }
        }
        switch (equation[prior])
        {
        case '+':
            return calc::Addition(_solve(equation, newl, newM), _solve(equation, prior + 1, r));
        case '-':
            return calc::Subtraction(_solve(equation, newl, newM), _solve(equation, prior + 1, r));
        case '*':
            return calc::Multiply(_solve(equation, newl, newM), _solve(equation, prior + 1, r));
        case '/':
            return calc::Divide(_solve(equation, newl, newM), _solve(equation, prior + 1, r));
        case '^':
            return calc::Pow(_solve(equation, newl, newM), _solve(equation, prior + 1, r));
        default:
            throw "Wut?";
        }
    }
    else
    {
        while (equation[l] == '(')
        {
            l++;
        }
        while (equation[r] == ')')
        {
            r--;
        }
        double result;
        std::string subequation = equation.substr(l, r);
        if (subequation == "")
        {
            return 0;
        }
        result = std::stod(subequation);
        return result;
    }
}*/ // Out of service - totally broken.

double controller::_solve(std::string equation)
{
    size_t i = 0; // size of equation
    double result = 0, trires, subresult;
    char operation;

    for (i = 0; i < equation.size(); i++) // Cycle for removing all unwanted characters.
    {
        if (trigonometry.find(equation[i]) != std::string::npos) // If the character is s, c, t, g or !, replace it with number (trigonometry and factorial section).
        {
            if (equation[i + 1] != '(') // Controlling existence of bracket (ex: s( )
            {
                throw "Missing bracket";
            }
            size_t endptr;
            trires = std::stod(equation.substr(i + 2, equation.size()), &endptr); // Getting parameter from brackets (ex: s(number... -> substr=number..., stod then gets the number).
            operation = equation[i];
            equation.erase(i, endptr + 2); // Erases function and parameter from equation.
            switch (operation) // Chooses operation (function) to use.
            {
            case 's':
                trires = calc::Sinus(trires, inRadian);
                break;
            case 'c':
                trires = calc::Cosinus(trires, inRadian);
                break;
            case 't':
                trires = calc::Tangents(trires, inRadian);
                break;
            case 'g':
                trires = calc::Cotangents(trires, inRadian);
                break;
            case '!':
                trires = calc::Factorial((int)trires);
            }
            equation.insert(i, std::to_string(trires)); // Inserts the result of function to equation.
        } // end of trigonometry and factorial section.

        if (equation[i] == '(' || equation[i] == ')') // Deletes bracket.
        {
            equation.erase(i, 1);
            i--;
            continue;
        }
        if (equation[i] == 'q') // Replacing sqrt with result of n-th root.
        {
            if (equation[i + 1] != '(') // Controlls existence of first bracket.
            {
                throw "Missing bracket";
            }
            equation.erase(i, 2); // Erases q(
            size_t endptr;
            double base = std::stod(equation.substr(i, equation.size()), &endptr); // Gets substring and it's value with first parameter of sqrt function.
            equation.erase(i, endptr); // Erases first parameter.
            if (equation[i] == ',') // Less ambitious deleting of comma
            {
                equation.erase(i, 1);
            }
            if (equation[i] == ' ') // Deleting blank space.
            {
                equation.erase(i, 1);
            }
            double quotient = std::stod(equation.substr(i, equation.size()), &endptr); // Getting second parameter of sqrt function.
            equation.erase(i, endptr); // Erases second parameter of sqrt.
            subresult = calc::Sqrt(base, quotient);
            equation.insert(i, std::to_string(subresult));
        } // End of q section.
    } // End of for cycle.
    size_t endptr;
    result = std::stod(equation, &endptr); // Reading first number of equation.
    equation.erase(0, endptr); // Erasing first number of equation.
    while (equation.size() > 0) // Cycle for processing equation.
    {
        operation = equation[0]; // Getting operator.
        equation.erase(0, 1);  // erases operator
        switch (operation) // Switching various operation.
        {
        case '+':
            result = calc::Addition(result, std::stod(equation, &endptr)); // Does result + next number.
            break;
        case '-':
            result = calc::Subtraction(result, std::stod(equation, &endptr)); // result - next number
            break;
        case '*':
            result = calc::Multiply(result, std::stod(equation, &endptr)); // result * next number
            break;
        case '/':
            result = calc::Divide(result, std::stod(equation, &endptr)); // result / next number
            break;
        case '^':
            result = calc::Pow(result, std::stod(equation, &endptr)); // result ^ next number
            break;
        default: // How could someone get there...
            break;
        }
        equation.erase(0, endptr); // Erases number.
    }// end of while cycle
    return result;
} // End of _solve() function.

std::string controller::_rewriteConstants(std::string rewrite) // Probably useless function, because someone forgot to add PI to GUI...
{
    size_t i = 0;
    while ((i = rewrite.find("PI", i)) != std::string::npos) // Finds every evidence of PI in string.
    {
        if (i > rewrite.size()) // Solving some strange bug.
        {
            break;
        }
        rewrite.erase(i, 2); //Erases PI from rewrite.
        rewrite.insert(i, std::to_string(3.14159265359)); // Adds PI to rewrite.
    }
    return rewrite;
}// End of _rewriteConstants

double controller::Solve(std::string equation) // Function for solving equation (expression is more accurate).
{
    int brackets = 0;
    char pom = equation[0];
    for (unsigned long i = 0; i < equation.size(); i++) // Cycle for searching invalid characters or missing brackets.
    {
        pom = equation[i]; // Getting current character.
        if (pom == '(')
        {
            brackets++;
        }
        if (pom == ')')
        {
            brackets--;
            if (brackets < 0) //ex: equation = "()))"
            {
                throw "Invalid input - too much right brackets before left.";
            }
        }
        if (!isdigit(pom) && (listOfChars.find(pom) == std::string::npos)) // Current character isn't number or anything from string "+-/*sctgq^..."
        {
            throw "Invalid character.";
        }
    }// End of for cycle
    if (brackets > 0)  // Wrong amount of brackets.
    {
        throw "Invalid input - too much left brackets.";
    }
    return _solve(_rewriteConstants(equation)); // Calling rewriteConstants(), but honestly - it's useless.
} // End of solve function.

std::string controller::AddNumberToEquation(std::string _number)
{
    buffer = buffer + _number; // Can add string, but mostly used with single character.
    return buffer;
} // End of AddNumberToEquation function.

std::string controller::AddOperatorToEquation(std::string _operator) // Function for adding operators to equation with correct syntax.
{
    if (buffer.size() == 0) // Buffer is empty, but user wants to add *, / or ^ - that's illegal!
    {
        if (_operator[0] == '*' || _operator[0] == '/' || _operator[0] == '^')
        {
            throw "Invalid first operator";
        }
    } //end of if()

    if (buffer.size() > 1) // Some special cases which is good to handle. (Honestly, worst part of my work - needs refactoring, maybe adding some features.)
    {
        if (_operator[0] == '+') // Added operator is +.
        {
            if (buffer[buffer.size() - 1] == '+' || buffer[buffer.size() - 1] == '-') //
            {
                buffer[buffer.size() - 1] = '+'; // Changes previous operator.
                return buffer;
            }
        }
        if (_operator[0] == '-') // Added operator is -.
        {
            if (buffer[buffer.size() - 1] == '+' || buffer[buffer.size() - 1] == '-')
            {
                buffer[buffer.size() - 1] = '-'; // Changes previous operator.
                return buffer;
            }
        }
        if (_operator[0] == '/' || _operator[0] == '^' || _operator[0] == '*') // That's terrible, but it controlls if there aren't two operators in a row.
        {
            if (buffer[buffer.size() - 1] == '/' || buffer[buffer.size() - 1] == '*' || buffer[buffer.size() - 1] == '^')
            {
                throw "Two operators in a row!";
            }
        }
    } // End of special cases (if()).

    buffer = buffer + _operator; // Adds operator to expression.
    std::string trigonometry = "sctgq!r"; // Not only trigonometry functions, but also sqrt, pow etc.
    if (trigonometry.find(_operator[0]) != std::string::npos) // Adds bracket (, if it's needed.
    {
        buffer = buffer + "(";
    }
    if (_operator[0] == ',')
    {
        buffer = buffer + " ";
    }
    return buffer;
} // End of AddOperatorToEquation().

void controller::Clear()
{
    buffer.clear();
}

void controller::SetInRadian(bool choose)
{
    this->inRadian = choose;
}

bool controller::GetInRadian()
{
    return this->inRadian;
}

std::string controller::GetBuffer()
{
    return this->buffer;
}

std::string controller::GetImprovedBuffer() // Improves buffer for printing in GUI.
{
    std::string newString = ""; // Starting with empty string.
    for (int i = 0; i < buffer.size(); i++) // Cycle for controlling operators and adding all the stuff.
    {
        switch (buffer[i]) // Adding strings to newString due to actual character.
        {
        case 's':
            newString += "sin";
            break;
        case 'c':
            newString += "cos";
            break;
        case 't':
            newString += "tan";
            break;
        case 'g':
            newString += "cotg";
            break;
        case 'q':
            newString += "sqrt";
            break;
        case 'r': // Unused...
            newString += "round";
            break;
        default:
            newString.append(1, buffer[i]); // Appends just single character (mostly number characters, but also '.', ','...).
            break;
        }
    }
    return newString;
}

void controller::DeleteChar() // Deletes last character.
{
    buffer.pop_back();
}

/*Konec souboru controller.cpp*/