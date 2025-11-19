#include "Geometry.h"
#include <stdexcept>
#include <iostream>

template <ScalarType T>
Figure<T>::Figure(const Figure& other) {
    this->vertices.reserve(other.vertices.size());
    for (const auto& point_ptr : other.vertices) {
        this->vertices.emplace_back(std::make_unique<Point<T>>(*point_ptr));
    }
}
template <ScalarType T>
Point<T> Figure<T>::center() const {
    T sum_x = 0;
    T sum_y = 0;
    for (const auto& p : this->vertices) {
        sum_x += p->x;
        sum_y += p->y;
    }
    size_t n = this->vertices.size();
    if (n == 0) return Point<T>{};
    return Point<T>{sum_x / static_cast<T>(n), sum_y / static_cast<T>(n)};
}

template <class T>
void Array<T>::resize(size_t new_capacity) {
    if (new_capacity <= capacity) return;
    std::shared_ptr<T[]> new_data = std::shared_ptr<T[]>(new T[new_capacity]);
    for (size_t i = 0; i < size; ++i) {
        new_data[i] = std::move(data[i]);
    }
    data = std::move(new_data); 
    capacity = new_capacity;
}
template <class T>
void Array<T>::push_back(T item) {
    if (size == capacity) {
        resize(capacity == 0 ? 1 : capacity * 2); 
    }
    data[size++] = std::move(item); 
}
template <class T>
void Array<T>::remove_at(size_t index) {
    if (index >= size) {
        throw std::out_of_range("Index out of bounds");
    }
    for (size_t i = index; i < size - 1; ++i) {
        data[i] = std::move(data[i + 1]); 
    }
    size--;
}
template <class T>
T& Array<T>::operator[](size_t index) {
    if (index >= size) {
        throw std::out_of_range("Index out of bounds");
    }
    return data[index];
}
template <class T>
const T& Array<T>::operator[](size_t index) const {
    if (index >= size) {
        throw std::out_of_range("Index out of bounds");
    }
    return data[index];
}
template <ScalarType T>
double calculate_polygon_area(const std::vector<typename Figure<T>::PointPtr>& vertices) {
    size_t n = vertices.size();
    if (n < 3) return 0.0;
    double area_val = 0.0;
    
    for (size_t i = 0; i < n; ++i) {
        const Point<T>& p1 = *vertices[i];
        const Point<T>& p2 = *vertices[(i + 1) % n]; 
        area_val += (static_cast<double>(p1.x) * static_cast<double>(p2.y) - 
                    static_cast<double>(p2.x) * static_cast<double>(p1.y));
    }
    return std::abs(area_val) / 2.0;
}

template <ScalarType T>
Trapezoid<T>::Trapezoid(T x1, T y1, T x2, T y2, T x3, T y3, T x4, T y4) {
    this->vertices.emplace_back(std::make_unique<Point<T>>(x1, y1));
    this->vertices.emplace_back(std::make_unique<Point<T>>(x2, y2));
    this->vertices.emplace_back(std::make_unique<Point<T>>(x3, y3));
    this->vertices.emplace_back(std::make_unique<Point<T>>(x4, y4));
}

template <ScalarType T>
Point<T> Trapezoid<T>::center() const { return Figure<T>::center(); }

template <ScalarType T>
double Trapezoid<T>::area() const { return calculate_polygon_area<T>(this->vertices); }

template <ScalarType T>
void Trapezoid<T>::print_coords() const {
    std::cout << "  Trapezoid coordinates: ";
    for (size_t i = 0; i < this->vertices.size(); ++i) {
        std::cout << "(" << this->vertices[i]->x << ", " << this->vertices[i]->y << ") ";
    }
    std::cout << std::endl;
}

template <ScalarType T>
bool Trapezoid<T>::operator==(const Figure<T>& other) const {
    return std::abs(this->area() - other.area()) < 1e-6;
}

template <ScalarType T>
Rhombus<T>::Rhombus(T x1, T y1, T x2, T y2, T x3, T y3, T x4, T y4) : Figure<T>() {
    this->vertices.emplace_back(std::make_unique<Point<T>>(x1, y1));
    this->vertices.emplace_back(std::make_unique<Point<T>>(x2, y2));
    this->vertices.emplace_back(std::make_unique<Point<T>>(x3, y3));
    this->vertices.emplace_back(std::make_unique<Point<T>>(x4, y4));
}

template <ScalarType T> 
Point<T> Rhombus<T>::center() const { return Figure<T>::center(); } 
template <ScalarType T> 
double Rhombus<T>::area() const { return calculate_polygon_area<T>(this->vertices); }
template <ScalarType T> 
void Rhombus<T>::print_coords() const { 
    std::cout << "  Rhombus coordinates: ";
    for (size_t i = 0; i < this->vertices.size(); ++i) {
        std::cout << "(" << this->vertices[i]->x << ", " << this->vertices[i]->y << ") ";
    }
    std::cout << std::endl;
}

template <ScalarType T> 
bool Rhombus<T>::operator==(const Figure<T>& other) const { 
    return std::abs(this->area() - other.area()) < 1e-6; 
}

template <ScalarType T>
Pentagon<T>::Pentagon(T x1, T y1, T x2, T y2, T x3, T y3, T x4, T y4, T x5, T y5) : Figure<T>() {
    this->vertices.emplace_back(std::make_unique<Point<T>>(x1, y1));
    this->vertices.emplace_back(std::make_unique<Point<T>>(x2, y2));
    this->vertices.emplace_back(std::make_unique<Point<T>>(x3, y3));
    this->vertices.emplace_back(std::make_unique<Point<T>>(x4, y4));
    this->vertices.emplace_back(std::make_unique<Point<T>>(x5, y5));
}

template <ScalarType T> 
Point<T> Pentagon<T>::center() const { return Figure<T>::center(); } 
template <ScalarType T> 
double Pentagon<T>::area() const { return calculate_polygon_area<T>(this->vertices); }
template <ScalarType T> 
void Pentagon<T>::print_coords() const { 
    std::cout << "  Pentagon coordinates: ";
    for (size_t i = 0; i < this->vertices.size(); ++i) {
        std::cout << "(" << this->vertices[i]->x << ", " << this->vertices[i]->y << ") ";
    }
    std::cout << std::endl;
}

template <ScalarType T> 
bool Pentagon<T>::operator==(const Figure<T>& other) const { 
    return std::abs(this->area() - other.area()) < 1e-6; 
}

template class Figure<double>;
template class Trapezoid<double>;
template class Rhombus<double>;
template class Pentagon<double>;
template class Point<double>;
template class Figure<int>;
template class Trapezoid<int>;
template class Rhombus<int>;
template class Pentagon<int>;
template class Point<int>;
template class Figure<float>;
template class Trapezoid<float>;
template class Rhombus<float>;
template class Pentagon<float>;
template class Point<float>;
template class Array<std::shared_ptr<Figure<double>>>;
template class Array<std::shared_ptr<Figure<int>>>;
template class Array<std::shared_ptr<Figure<float>>>;
template class Array<std::shared_ptr<Trapezoid<double>>>;
template class Array<std::shared_ptr<Rhombus<double>>>;
template class Array<std::shared_ptr<Rhombus<int>>>;
template class Array<std::shared_ptr<Rhombus<float>>>;
template class Array<std::shared_ptr<Trapezoid<float>>>;