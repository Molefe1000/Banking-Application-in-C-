#include "BankSystem.h"
#include <iostream>

int main() {
    std::cout << "\n============================================\n";
    std::cout << "  WELCOME TO STANDARD BANK SYSTEM\n";
    std::cout << "  Multi-Branch Banking Management\n";
    std::cout << "============================================\n";

    try {
        BankSystem system;
        system.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}