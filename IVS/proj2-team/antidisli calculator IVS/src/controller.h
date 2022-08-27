/**
 *	@file controller.h
 *
 *	@brief Controller class file
 *	@author David Hudak
 */

#include "calc.h"
#include <string>

class controller
{
public:


    controller();

    ~controller();

	// call calc.h with buffer and return result of equation
    /** 
    * Function calls functions from mathematical library (calc.h) and solves given expression.
    * @param equation Input expression which is solved.
    * @return Returns result of expression.
    */
	double Solve(std::string equation);

	// will add number or floating point to equation and return string for GUI
    /**
    * Function adds numbers to buffer.
    * @param _number Number which is added.
    */
    std::string AddNumberToEquation(std::string _number);

	// will add operator to equation and return string for GUI 
	// Can add s, c, q, t, g, !...
    /**
    * Function adds elements to buffer, which aren't numbers.
    * @param _operator String of operator. Mostly used with just one element.
    * @return Return new version of buffer.
    */
	std::string AddOperatorToEquation(std::string _operator);

	/**
    * Function deletes (clears) all elements from buffer.
    */
	void Clear();

	/**
    * Setter for inRadian value
    * @param If choose is true, functions will work with radians, if not, functions will work with degrees.
    */
	void SetInRadian(bool choose);

	/**
    * Getter for inRadian value which says whehter trigonometry functions work with degrees or radians. 
    * @return Returns value of attribute inRadian
    */
	bool GetInRadian();

	/**
    * Getter of buffer.
    * @return Returns real version of buffer.
    */
	std::string GetBuffer();

	/**
    * Function makes from buffer its expanded version.
    * @return Improved version of string for printing.
    */
	std::string GetImprovedBuffer();

	/**
    * Function deletes last element of buffer.
    */
	void DeleteChar();

	
private:
    /**
    * Rewrites all PI to numbers in input.
    * @return Returns new version of input.
    */
    std::string _rewriteConstants(std::string rewrite);

	std::string listOfChars = "+-*/sctg^q()., PI!";
    std::string trigonometry = "sctg!";

    /**
    * Function which is recursively called for solving expression.
    * @param equation Expression to solve.
    * @param l Left border of expression.
    * @param r Right border of expression.
    */
	double _solve(std::string equation);

	// contains current equation
	std::string buffer;

	// Switch for function calculate
	//	if inRadian == true, then calculate with radians. 
	bool inRadian=false;

    // Operators with lower priority.
    std::string lower = "+-";

    // Operators with higher priority.
    std::string higher = "*/";
};

