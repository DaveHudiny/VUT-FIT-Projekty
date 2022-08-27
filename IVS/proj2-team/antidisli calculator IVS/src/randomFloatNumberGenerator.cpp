#include <iostream>
#include <cstdlib>
#include <ctime>

int main() {

    std::cout << "write number of float number to generate" << std::endl;
    int count = 0;

    std::cin >> count;

    srand(static_cast <unsigned> (time(0)));

    FILE* f = fopen("randomFloatNumbers.txt", "w");

    for (int i = 0; i < count; ++i)
        fprintf(f, "%f ", static_cast<float>(rand()) / static_cast<float>(rand()) + static_cast<float>(rand()));

    return 0;
}