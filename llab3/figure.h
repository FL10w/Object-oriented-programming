#ifndef FIGURE_H
#define FIGURE_H

#include <vector>
#include <memory>
#include <iostream>

void showMenu();

struct Point {
    double x, y;
    Point(double x = 0, double y = 0) : x(x), y(y) {}
};

class Figure {
public:
    virtual ~Figure() = default;
    virtual double area() const = 0;
    virtual void print(std::ostream& os) const = 0;
    virtual void read(std::istream& is) = 0;
    virtual Point center() const = 0;
};

class RegularPolygon : public Figure {
protected:
    std::vector<Point> vertices;
    Point center_;
    int sides = 0;

    void calculateCenter();  

public:
    RegularPolygon() = default;
    RegularPolygon(const std::vector<Point>& vertices);
    
    double area() const override;
    void print(std::ostream& os) const override;
    void read(std::istream& is) override;
    Point center() const override;
};

class Pentagon : public RegularPolygon {
public:
    Pentagon();
    Pentagon(const std::vector<Point>& vertices);
    void read(std::istream& is) override;
};

class Hexagon : public RegularPolygon {
public:
    Hexagon();
    Hexagon(const std::vector<Point>& vertices);
    void read(std::istream& is) override;
};

class Octagon : public RegularPolygon {
public:
    Octagon();
    Octagon(const std::vector<Point>& vertices);
    void read(std::istream& is) override;
};

class FigureArray {
private:
    std::vector<std::shared_ptr<Figure>> figures;

public:
    void addFigure(std::shared_ptr<Figure> figure);
    void removeFigure(int index);
    void printAll() const;
    double totalArea() const;
    std::shared_ptr<Figure> operator[](size_t index) const;
    size_t size() const;
};

std::ostream& operator<<(std::ostream& os, const Figure& figure);
std::istream& operator>>(std::istream& is, Figure& figure);

#endif