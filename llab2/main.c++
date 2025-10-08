#include <iostream>
#include "func.h"

int main() {
    char continueChoice;
    
    do {
        demonstrate();
        
        std::cout << "\nХотите продолжить? (y/other): ";
        std::cin >> continueChoice;
        
    } while (continueChoice == 'y' || continueChoice == 'Y');
    
    std::cout << "Программа завершена." << std::endl;
    return 0;
}