#include "func.h"
#include <algorithm>
#include <stdexcept>
#include <iostream>

Octal::Octal() : digits({0}) {}

Octal::Octal(const size_t& n, unsigned char t) {
    if (n == 0) {
        digits.push_back(0);
        return;
    }
    
    digits.resize(n);
    for (size_t i = 0; i < n; ++i) {
        if (t > 7) {
            throw std::invalid_argument("Должен быть между 0 и 7");
        }
        digits[i] = t;
    }
    Zeros();
}

Octal::Octal(const std::initializer_list<unsigned char>& t) {
    if (t.size() == 0) {
        digits.push_back(0);
        return;
    }
    
    for (auto it = t.begin(); it != t.end(); ++it) {
        if (*it > 7) {
            throw std::invalid_argument("Должен быть между 0 и 7");
        }
    }
    
    digits.assign(t.begin(), t.end());
    Zeros();
}

Octal::Octal(const std::string& t) {
    if (t.empty()) {
        digits.push_back(0);
        return;
    }
    
    for (char c : t) {
        if (c < '0' || c > '7') {
            throw std::invalid_argument("Символы должны быть в диапазоне от 0 до 7");
        }
        digits.push_back(static_cast<unsigned char>(c - '0'));
    }
    Zeros();
}

// Копирует все цифры в новый объект
Octal::Octal(const Octal& other) : digits(other.digits) {}

// Перемещает ресурсы из временного объекта, избегает ненужного копирования
Octal::Octal(Octal&& other) noexcept : digits(std::move(other.digits)) {
    other.digits = {0};
}

// Освобождает ресурсы объекта
Octal::~Octal() noexcept {}

void Octal::Zeros() {
    while (digits.size() > 1 && digits[0] == 0) {
        digits.erase(digits.begin());
    }
}

Octal Octal::add(const Octal& other) const {
    Octal result = *this;
    return result.addAssign(other);
}

Octal Octal::subtract(const Octal& other) const {
    Octal result = *this;
    return result.subtractAssign(other);
}

Octal Octal::copy() const {
    return Octal(*this);
}

bool Octal::greaterThan(const Octal& other) const {
    if (digits.size() != other.digits.size()) {
        return digits.size() > other.digits.size();
    }
    
    for (size_t i = 0; i < digits.size(); ++i) {
        if (digits[i] != other.digits[i]) {
            return digits[i] > other.digits[i];
        }
    }
    return false;
}

bool Octal::lessThan(const Octal& other) const {
    if (digits.size() != other.digits.size()) {
        return digits.size() < other.digits.size();
    }
    
    for (size_t i = 0; i < digits.size(); ++i) {
        if (digits[i] != other.digits[i]) {
            return digits[i] < other.digits[i];
        }
    }
    return false;
}

bool Octal::equals(const Octal& other) const {
    if (digits.size() != other.digits.size()) {
        return false;
    }
    
    for (size_t i = 0; i < digits.size(); ++i) {
        if (digits[i] != other.digits[i]) {
            return false;
        }
    }
    return true;
}

Octal Octal::addAssign(const Octal& other) {
    size_t maxSize = std::max(digits.size(), other.digits.size());
    std::vector<unsigned char> result(maxSize + 1, 0);
    
    int carry = 0;
    for (int i = 0; i < maxSize || carry; ++i) {
        int sum = carry;
        
        if (i < digits.size()) {
            sum += digits[digits.size() - 1 - i];
        }
        
        if (i < other.digits.size()) {
            sum += other.digits[other.digits.size() - 1 - i];
        }
        
        carry = sum / 8;
        if (maxSize - i - 1 >= 0) {
            result[maxSize - i] = sum % 8;
        } else {
            result.insert(result.begin(), sum % 8);
        }
    }
    
    digits = result;
    Zeros();
    return *this;
}

Octal Octal::subtractAssign(const Octal& other) {
    if (lessThan(other)) {
        throw std::invalid_argument("Нельзя вычесть большее число из меньшего");
    }
    
    std::vector<unsigned char> result = digits;
    int borrow = 0;
    
    for (int i = 0; i < other.digits.size() || borrow; ++i) {
        int diff = result[result.size() - 1 - i] - borrow;
        
        if (i < other.digits.size()) {
            diff -= other.digits[other.digits.size() - 1 - i];
        }
        
        if (diff < 0) {
            borrow = 1;
            diff += 8;
        } else {
            borrow = 0;
        }
        
        result[result.size() - 1 - i] = diff;
    }
    
    digits = result;
    Zeros();
    return *this;
}

std::string Octal::toString() const {
    std::string result;
    for (unsigned char digit : digits) {
        result += std::to_string(static_cast<int>(digit));
    }
    return result;
}

size_t Octal::size() const {
    return digits.size();
}

bool Octal::empty() const {
    return digits.empty() || (digits.size() == 1 && digits[0] == 0);
}
//копирование
Octal& Octal::operator=(const Octal& other) {
    if (this != &other) {
        digits = other.digits;
    }
    return *this;
}
// перемещение
Octal& Octal::operator=(Octal&& other) noexcept {
    if (this != &other) {
        digits = std::move(other.digits);
        other.digits = {0};
    }
    return *this;
}

Octal read(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::cin >> input;
        try {
            return Octal(input);
        } catch (const std::exception& e) {
            std::cout << "Ошибка: " << e.what() << std::endl;
            std::cout << "Пожалуйста, введите корректное восьмеричное число, только цифры 0-7: ";
        }
    }
}

void demonstrate() {    
    try {
        Octal num1 = read("Введите первое восьмеричное число: ");
        Octal num2 = read("Введите второе восьмеричное число: ");   
        std::cout << "\nВведенные числа:" << std::endl;
        std::cout << "Число 1: " << num1.toString() << std::endl;
        std::cout << "Число 2: " << num2.toString() << std::endl;
        
        int choice;
        std::cout << "\nВыберите операцию:" << std::endl;
        std::cout << "1 - Сложение" << std::endl;
        std::cout << "2 - Вычитание" << std::endl;
        std::cout << "3 - Сравнение" << std::endl;
        std::cout << "4 - Все операции" << std::endl;
        std::cout << "Ваш выбор: ";
        std::cin >> choice;
        std::cout << "\nРезультаты:" << std::endl;
        
        switch (choice) {
            case 1: {
                Octal sum = num1.add(num2);
                std::cout << num1.toString() << " + " << num2.toString() 
                          << " = " << sum.toString() << std::endl;
                break;
            }
            case 2: {
                try {
                    Octal diff = num1.subtract(num2);
                    std::cout << num1.toString() << " - " << num2.toString() 
                              << " = " << diff.toString() << std::endl;
                } catch (const std::exception& e) {
                    std::cout << "Ошибка вычитания: " << e.what() << std::endl;
                }
                break;
            }
            case 3: {
                std::cout << num1.toString() << " > " << num2.toString() << " : " 
                          << (num1.greaterThan(num2) ? "true" : "false") << std::endl;
                std::cout << num1.toString() << " < " << num2.toString() << " : " 
                          << (num1.lessThan(num2) ? "true" : "false") << std::endl;
                std::cout << num1.toString() << " == " << num2.toString() << " : " 
                          << (num1.equals(num2) ? "true" : "false") << std::endl;
                break;
            }
            case 4: {
                // Сложение
                Octal sum = num1.add(num2);
                std::cout << "Сложение: " << num1.toString() << " + " << num2.toString() 
                          << " = " << sum.toString() << std::endl;
                
                // Вычитание
                try {
                    Octal diff = num1.subtract(num2);
                    std::cout << "Вычитание: " << num1.toString() << " - " << num2.toString() 
                              << " = " << diff.toString() << std::endl;
                } catch (const std::exception& e) {
                    std::cout << "Вычитание: " << e.what() << std::endl;
                }
                
                // Сравнение
                std::cout << "Сравнение:" << std::endl;
                std::cout << "  " << num1.toString() << " > " << num2.toString() << " : " 
                          << (num1.greaterThan(num2) ? "true" : "false") << std::endl;
                std::cout << "  " << num1.toString() << " < " << num2.toString() << " : " 
                          << (num1.lessThan(num2) ? "true" : "false") << std::endl;
                std::cout << "  " << num1.toString() << " == " << num2.toString() << " : " 
                          << (num1.equals(num2) ? "true" : "false") << std::endl;
                
                // Операции с присваиванием
                Octal copy1 = num1.copy();
                copy1.addAssign(num2);
                std::cout << "Сложение с присваиванием: " << num1.toString() << " += " << num2.toString() 
                          << " = " << copy1.toString() << std::endl;
                break;
            }
            default:
                std::cout << "Неверный выбор операции!" << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Произошла ошибка: " << e.what() << std::endl;
    }
}