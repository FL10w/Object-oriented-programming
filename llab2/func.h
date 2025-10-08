#ifndef FUNC_H
#define FUNC_H

#include <string>
#include <vector>
#include <stdexcept>

class Octal {
private:
    std::vector<unsigned char> digits;
    void Zeros();
    
public:
    Octal();
    Octal(const size_t& n, unsigned char t = 0);
    Octal(const std::initializer_list<unsigned char>& t);
    Octal(const std::string& t);
    Octal(const Octal& other);
    Octal(Octal&& other) noexcept;
    virtual ~Octal() noexcept;
    // Арифметические операции
    Octal add(const Octal& other) const;
    Octal subtract(const Octal& other) const;
    Octal copy() const;  
    Octal addAssign(const Octal& other);
    Octal subtractAssign(const Octal& other);
    // Операции сравнения
    bool greaterThan(const Octal& other) const;
    bool lessThan(const Octal& other) const;
    bool equals(const Octal& other) const;  
    // методы
    std::string toString() const;
    size_t size() const;
    bool empty() const;
    // Оператор присваивания
    Octal& operator=(const Octal& other);
    Octal& operator=(Octal&& other) noexcept;
};

Octal read(const std::string& prompt);
void demonstrate();

#endif