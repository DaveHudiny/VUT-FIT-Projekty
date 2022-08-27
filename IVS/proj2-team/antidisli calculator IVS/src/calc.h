#include <string>

/** 
 *  
 *  
 */

/**
 *	@file calc.h
 *	@brief Class with prototypes of mathematical library
 *	@author Martin Kmenta, comments David Hudak
 */


/**
 *	@brief Class, which implements math-library (calculator).
 */
class calc
{
public:

	/**
	*   Function rounds number and return number due the param floatingLength.
	*	@param number Number to round.
	*	@param floatingLength:
	*		- >0  -> getting rid of numbers before floating point (ex: floatingLength = 2 and number = 12365.678910 -> 12400.00000) \n
	*		- ==0 -> getting rid of part after floating point (ex: 123.456 -> 123.000) \n
	*		- <0  -> getting rid of numbers after floating point (ex: floatingLength = 3 and number = 12345.678912 -> 12345.679) \n
	*	@return Rounded number
	*/
	static double Round(double number, int floatingLength);

	/**
	*	Function powers number (exponentation).
	*	@param number What is powered, base.
	*	@param quotient Quotient of exponentation.
	*	@return Returns powered number.
	*/
	static double Pow(double number, double quotient);

	/**
	*	Function makes nth root of number.
	*	@param number Base of nth root.
	*	@param quotient Exponent.
	*	@return	Returns nth root of number.
	*/
	static double Sqrt(double number, double quotient);

	/**
	*	Function for trigonometrical operation - sinus.
	*	@param number Input of sinus function.
	*	@param inRadian If inRadian is zero, input number is in degrees, if one, input number is in radians.       .
	*	@return	sinus.
	*/
	static double Sinus(double number, bool inRadian);

	/**
	*	Function for trigonometrical operation - cosinus.
	*	@param number Input of cosinus function.
	*	@param inRadian If inRadian is zero, input number is in degrees, if one, input number is in radians.      .
	*	@return	Returns cosinus.
	*/
	static double Cosinus(double number, bool inRadian);

	/**
	*	Function for trigonometrical operation - tangens.
	*	@param number Input of tangens function.
	*	@param inRadian If inRadian is zero, input number is in degrees, if one, input number is in radians.       .
	*	@return	Returns tangens.
	*/
	static double Tangents(double number, bool inRadian);

	/**
	*	Function for trigonometrical operation - cotangens.
	*	@param number Input of cotangens function.
	*	@param inRadian If inRadian is zero, input number is in degrees, if one, input number is in radians.      .
	*	@return	Returns cotangens.
	*/
	static double Cotangents(double number, bool inRadian);

	/**
	*	Function multiplicates two numbers.
	*	@param number1 Multiplicand.
	*	@param number2 Multiplier.
	*	@return	Input1*Input2.
	*/
	static double Multiply(double number1, double number2);

	/**
	*	Function divides two numbers.
	*	@param number1 Divisor.
	*	@param number2 Divider.
	*	@return	Input1/Input2.
	*/
	static double Divide(double number, double divider);

	/**
	*	Function makes addition of two numbers.
	*	@param number1 Addend 1.
	*	@param number2 Addend 2.
	*	@return	Input1+Input2.
	*/
	static double Addition(double number1, double number2);

	/**
	*	Function subtracts two numbers.
	*	@param number1 Subtrahend.
	*	@param number2 Minuend.
	*	@return	Input1-Input2.
	*/
	static double Subtraction(double number1, double number2);

	/**
	*	Function for counting factorial
	*	@param number Input number for factorial
	*	@return Factorial of number (number!)
	*/
	static double Factorial(int number);

    /**
    *   Function for counting arcus sinus.
    *   @param number Input for arcus sinus function.
    *   @param inRadian Chooses output form (degrees = 0, radians = 1).
    *   @return Degrees or radians due to param inRadian.
    */
    static double ArcSin(double number, bool inRadian);

    /**
    *   Function for counting arcus cosinus.
    *   @param number Input for arcus cosinus function.
    *   @param inRadian Chooses output form (degrees = 0, radians = 1).
    *   @return Degrees or radians due to param inRadian.
    */
    static double ArcCos(double number, bool inRadian);

    /**
    *   Function for counting arcus tangens.
    *   @param number Input for arcus tangens function.
    *   @param inRadian Chooses output form (degrees = 0, radians = 1).
    *   @return Degrees or radians due to param inRadian.
    */
    static double ArcTan(double number, bool inRadian);

    /**
    *   Function for counting arcus sinus.
    *   @param number Input for arcus sinus function.
    *   @param inRadian Chooses output form (degrees = 0, radians = 1).
    *   @return Degrees or radians due to param inRadian.
    */
    static double ArcCotg(double number, bool inRadian);

};