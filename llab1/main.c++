#include <iostream>
#include "func.h"

int main()
{
  char input[100];
  std::cout << "Введите строку:";
  std::cin.getline(input, 100);

  std::cout << "результат: " << Numbers(input) << std::endl;

  return 0;
}