#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <type_traits> 

template <typename T>
concept ScalarType = std::is_scalar_v<T>;

template <ScalarType T>
class Point {
public:
    T x, y;
    Point(T _x = T{}, T _y = T{}) : x(_x), y(_y) {}
    Point(const Point&) = default;
    Point& operator=(const Point&) = default;
    Point(Point&&) noexcept = default;
    Point& operator=(Point&&) noexcept = default;
};

template <ScalarType T>
class Figure {
public:
    using PointPtr = std::unique_ptr<Point<T>>;
    using FigurePtr = std::unique_ptr<Figure<T>>;
    
    Figure() = default;
    Figure(const Figure& other); 
    Figure& operator=(const Figure& other) = delete; 
    Figure(Figure&&) noexcept = default;
    Figure& operator=(Figure&&) noexcept = default;

    virtual Point<T> center() const; 
    
    virtual void print_coords() const = 0;
    virtual double area() const = 0;
    virtual operator double() const { return area(); } 
    virtual bool operator==(const Figure& other) const = 0; 

    virtual FigurePtr clone() const = 0;

    virtual ~Figure() = default;

protected:
    std::vector<PointPtr> vertices;
};


template <class T>
class Array {
private:
    std::shared_ptr<T[]> data; 
    size_t capacity = 0;
    size_t size = 0;
    void resize(size_t new_capacity);

public:
    Array() = default;
    Array(const Array&) = default;
    Array& operator=(const Array&) = default;
    Array(Array&&) noexcept = default;
    Array& operator=(Array&&) noexcept = default;

    void push_back(T item);
    void remove_at(size_t index); 
    T& operator[](size_t index);
    const T& operator[](size_t index) const;
    size_t get_size() const { return size; }
};


template <ScalarType T>
class Trapezoid : public Figure<T> {
public:
    Trapezoid(T x1, T y1, T x2, T y2, T x3, T y3, T x4, T y4);
    Trapezoid(const Trapezoid& other) : Figure<T>(other) {}

    Point<T> center() const override; 
    void print_coords() const override;
    double area() const override;
    bool operator==(const Figure<T>& other) const override;
    
    typename Figure<T>::FigurePtr clone() const override {
        return std::make_unique<Trapezoid<T>>(*this); 
    }
};

template <ScalarType T>
class Rhombus : public Figure<T> {
public:
    Rhombus(T x1, T y1, T x2, T y2, T x3, T y3, T x4, T y4); 
    Rhombus(const Rhombus& other) : Figure<T>(other) {}

    Point<T> center() const override; 
    void print_coords() const override;
    double area() const override;
    bool operator==(const Figure<T>& other) const override;
    
    typename Figure<T>::FigurePtr clone() const override {
        return std::make_unique<Rhombus<T>>(*this);
    }
};

template <ScalarType T>
class Pentagon : public Figure<T> {
public:
    Pentagon(T x1, T y1, T x2, T y2, T x3, T y3, T x4, T y4, T x5, T y5);
    Pentagon(const Pentagon& other) : Figure<T>(other) {}

    Point<T> center() const override; 
    void print_coords() const override;
    double area() const override;
    bool operator==(const Figure<T>& other) const override;
    
    typename Figure<T>::FigurePtr clone() const override {
        return std::make_unique<Pentagon<T>>(*this);
    }
};

#endif 