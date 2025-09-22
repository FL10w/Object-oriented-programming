#include "func.h"
#include <cctype>
#include <cstring>

const char* Numbers(const char* input) {
    static char result[100];
    int count = 0;
    result[0] = '\0';
    
    for (int i = 0; input[i] != '\0' && count < 99; i++) {
        if (std::isdigit(input[i])) {
            result[count] = input[i];
            count++;
        }
    }
    
    result[count] = '\0';
    return result;
}