#include "figure.h"
#include <iostream>
#include <cmath>
#include <limits>
#include <sstream>
#include <string>

void showMenu() {
    std::cout << "1. Добавить пятиугольник" << std::endl;
    std::cout << "2. Добавить шестиугольник" << std::endl;
    std::cout << "3. Добавить восьмиугольник" << std::endl;
    std::cout << "4. Показать все фигуры" << std::endl;
    std::cout << "5. Вычислить общую площадь" << std::endl;
    std::cout << "6. Удалить фигуру по индексу" << std::endl;
    std::cout << "0. Выход" << std::endl;
    std::cout << "Выберите опцию: ";
}

RegularPolygon::RegularPolygon(const std::vector<Point>& vertices) 
    : vertices(vertices) {
    calculateCenter();
}

void RegularPolygon::calculateCenter() {
    double sum_x = 0, sum_y = 0;
    for (const auto& vertex : vertices) {
        sum_x += vertex.x;
        sum_y += vertex.y;
    }
    center_.x = sum_x / vertices.size();
    center_.y = sum_y / vertices.size();
}

double RegularPolygon::area() const {
    double area = 0.0;
    int n = vertices.size();
    
    // Формула площади Гаусса (формула шнуровки)
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        area += vertices[i].x * vertices[j].y - vertices[j].x * vertices[i].y;
    }
    
    return std::abs(area) / 2.0;
}

void RegularPolygon::print(std::ostream& os) const {
    os << "Вершины: ";
    for (const auto& vertex : vertices) {
        os << "(" << vertex.x << ", " << vertex.y << ") ";
    }
}

void RegularPolygon::read(std::istream& is) {
    vertices.clear();
    std::string line;
    
    std::cout << "Введите " << sides << " вершин (x y для каждой):" << std::endl;
    
    for (int i = 0; i < sides; ++i) {
        std::cout << "Вершина " << (i + 1) << ": ";
        std::getline(is, line);
        
        std::istringstream iss(line);
        double x, y;
        iss >> x >> y;
        
        char extra;
        if (iss.fail() || iss.get(extra)) {
            std::cout << "Ошибка: для вершины нужно ровно два числа!" << std::endl;
            is.setstate(std::ios::failbit);
            return;
        }
        
        vertices.push_back(Point(x, y));
    }
    calculateCenter();
}

Point RegularPolygon::center() const {
    return center_;
}

// Pentagon
Pentagon::Pentagon(const std::vector<Point>& vertices) 
    : RegularPolygon(vertices) {
    sides = 5;
}
Pentagon::Pentagon() {
    sides = 5;
}
void Pentagon::read(std::istream& is) {
    std::cout << "~~~ Пятиугольник ~~~" << std::endl;
    sides = 5;
    RegularPolygon::read(is);
}

// Hexagon 
Hexagon::Hexagon(const std::vector<Point>& vertices) 
    : RegularPolygon(vertices) {
    sides = 6;
}
Hexagon::Hexagon() {
    sides = 6;
}
void Hexagon::read(std::istream& is) {
    std::cout << "~~~ Шестиугольник ~~~" << std::endl;
    sides = 6;
    RegularPolygon::read(is);
}

// Octagon
Octagon::Octagon(const std::vector<Point>& vertices) 
    : RegularPolygon(vertices) {
    sides = 8;
}
Octagon::Octagon() {
    sides = 8;
}
void Octagon::read(std::istream& is) {
    std::cout << "~~~ Восьмиугольник ~~~" << std::endl;
    sides = 8;
    RegularPolygon::read(is);
}

std::ostream& operator<<(std::ostream& os, const Figure& figure) {
    figure.print(os);
    Point center = figure.center();
    os << " | Геом. центр: (" << center.x << ", " << center.y << ")";
    os << " | Площадь: " << figure.area();
    return os;
}

std::istream& operator>>(std::istream& is, Figure& figure) {
    figure.read(is);
    return is;
}
void FigureArray::addFigure(std::shared_ptr<Figure> figure) {
    figures.push_back(figure);
}
void FigureArray::removeFigure(int index) {
    if (index >= 0 && index < static_cast<int>(figures.size())) {
        figures.erase(figures.begin() + index);
    } else {
        std::cout << "Ошибка: индекс " << index << " вне диапазона!" << std::endl;
    }
}
void FigureArray::printAll() const {
    for (size_t i = 0; i < figures.size(); ++i) {
        std::cout << "Фигура " << i << ": " << *figures[i] << std::endl;
    }
}
double FigureArray::totalArea() const {
    double total = 0;
    for (const auto& figure : figures) {
        total += figure->area();
    }
    return total;
}
std::shared_ptr<Figure> FigureArray::operator[](size_t index) const {
    if (index < figures.size()) {
        return figures[index];
    }
    return nullptr;
}
size_t FigureArray::size() const {
    return figures.size();
}