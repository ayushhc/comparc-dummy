#include <iostream>
#include <string>
#include <sstream>

int main() {
    std::string test1 = "0.25";
    std::string test2 = ".25";
    double d1 = std::stod(test1);
    double d2 = std::stod(test2);
    std::cout << "0.25 -> " << d1 << std::endl;
    std::cout << ".25 -> " << d2 << std::endl;
    return 0;
}
