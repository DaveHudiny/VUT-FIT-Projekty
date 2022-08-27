#include <iostream>
#include "controller.h"

#define DEBUG false

//@return sum of elements in buffer
double GetResult(double* buffer, int countOfData)
{
    //sum x
    double sumX = 0;
    double sumX2 = 0;
    for (int i = 0; i < countOfData; ++i)
    {
        sumX = calc::Addition(sumX, buffer[i]);
        sumX2 = calc::Pow(calc::Addition(sumX, buffer[i]), 2);
    }

    if (DEBUG)
    {
        std::cout << "countOfData: " << countOfData << std::endl;
        std::cout << "sumX: " << sumX << std::endl;
        std::cout << "sumX2: " << sumX2 << std::endl;
        std::cout << "sumX*(1/n): "
            << calc::Multiply(calc::Divide(1, countOfData), sumX) << std::endl;
        std::cout << "sumX*(1/n) ^2: "
            << calc::Pow(calc::Multiply(calc::Divide(1, countOfData), sumX), 2) << std::endl;
    }

    // n * (((1 / n) * [sumX]) ^ 2)
    const double N_X2 = calc::Multiply(countOfData, 
        calc::Pow(calc::Multiply(calc::Divide(1, countOfData), sumX), 2));

    if (DEBUG)
        std::cout << "N_X2: " << N_X2 << std::endl;

    // ([sumX2] - [n * (((1 / n) * sumX) ^ 2)])
    double tmp = calc::Subtraction(sumX2, N_X2);

    if (DEBUG)
        std::cout << "tmp: " << tmp << std::endl;

    // sqrt((1 / (n - 1)) * [sumX2 - (n * (((1 / n) * sumX) ^ 2))])
    double s = calc::Sqrt(calc::Multiply(calc::Divide(1, 
        calc::Subtraction(countOfData, 1)), tmp), 2);

    if (DEBUG)
        std::cout << "s: " << s << std::endl;

    return s;
}

int main() {
    int bufferSize = 1000;
    double* buffer = static_cast<double*>(malloc(bufferSize * sizeof(double)));
    if (buffer == nullptr)
        throw;

    int countOfData = 0;
    while (scanf("%lf", &buffer[countOfData]) != EOF)
    {
        ++countOfData;
        if (countOfData >= bufferSize)
        {
            bufferSize += 1000000;
            buffer = static_cast<double*>(realloc(buffer, bufferSize * sizeof(double)));
            if (buffer == nullptr)
                throw;
        }
    }

    const double result = GetResult(buffer, countOfData);

    free(buffer);

    std::cout << result << std::endl;

	return 0;
}

