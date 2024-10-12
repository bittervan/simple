#include <iostream>

extern "C" {
void calc_write(int result) {
    std::cout << "The result is: " << result << std::endl;
}

int calc_read(const char *variabelName) {
    std::cout << "Enter the value of " << variabelName << ": ";

    int value;
    std::cin >> value;
    return value;
}
}