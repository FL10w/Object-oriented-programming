#include "figure.h"
#include <gtest/gtest.h>
#include <sstream>
#include <vector>
#include <cmath>

// Вспомогательная функция для создания правильного пятиугольника
std::vector<Point> createRegularPentagon(Point center, double radius) {
    std::vector<Point> vertices;
    for (int i = 0; i < 5; ++i) {
        double angle = 2 * 3.1415 * i / 5;
        double x = center.x + radius * cos(angle);
        double y = center.y + radius * sin(angle);
        vertices.push_back(Point(x, y));
    }
    return vertices;
}

// Вспомогательная функция для создания правильного шестиугольника
std::vector<Point> createRegularHexagon(Point center, double radius) {
    std::vector<Point> vertices;
    for (int i = 0; i < 6; ++i) {
        double angle = 2 * 3.1415 * i / 6;
        double x = center.x + radius * cos(angle);
        double y = center.y + radius * sin(angle);
        vertices.push_back(Point(x, y));
    }
    return vertices;
}

// Вспомогательная функция для создания правильного восьмиугольника
std::vector<Point> createRegularOctagon(Point center, double radius) {
    std::vector<Point> vertices;
    for (int i = 0; i < 8; ++i) {
        double angle = 2 * 3.1415 * i / 8;
        double x = center.x + radius * cos(angle);
        double y = center.y + radius * sin(angle);
        vertices.push_back(Point(x, y));
    }
    return vertices;
}

TEST(PentagonTest, ConstructorAndArea) {
    auto vertices = createRegularPentagon(Point(0, 0), 1.0);
    Pentagon pentagon(vertices);
    
    EXPECT_NEAR(pentagon.area(), 0.5 * 5 * 1.0 * 1.0 * sin(2 * 3.1415 / 5), 0.01);
    
    Point center = pentagon.center();
    EXPECT_NEAR(center.x, 0.0, 0.01);
    EXPECT_NEAR(center.y, 0.0, 0.01);
}

TEST(HexagonTest, ConstructorAndArea) {
    auto vertices = createRegularHexagon(Point(1, 1), 2.0);
    Hexagon hexagon(vertices);
    
    EXPECT_NEAR(hexagon.area(), 0.5 * 6 * 2.0 * 2.0 * sin(2 * 3.1415 / 6), 0.01);
    
    Point center = hexagon.center();
    EXPECT_NEAR(center.x, 1.0, 0.01);
    EXPECT_NEAR(center.y, 1.0, 0.01);
}

TEST(OctagonTest, ConstructorAndArea) {
    auto vertices = createRegularOctagon(Point(-1, -1), 1.5);
    Octagon octagon(vertices);
    
    EXPECT_NEAR(octagon.area(), 0.5 * 8 * 1.5 * 1.5 * sin(2 * 3.1415 / 8), 0.01);
    
    Point center = octagon.center();
    EXPECT_NEAR(center.x, -1.0, 0.01);
    EXPECT_NEAR(center.y, -1.0, 0.01);
}

TEST(FigureArrayTest, AddAndRemove) {
    FigureArray array;
    
    auto pentagon_vertices = createRegularPentagon(Point(0, 0), 1.0);
    auto hexagon_vertices = createRegularHexagon(Point(1, 1), 2.0);
    
    array.addFigure(std::make_shared<Pentagon>(pentagon_vertices));
    array.addFigure(std::make_shared<Hexagon>(hexagon_vertices));
    
    EXPECT_EQ(array.size(), 2);
    
    array.removeFigure(0);
    EXPECT_EQ(array.size(), 1);
    
    array.removeFigure(0);
    EXPECT_EQ(array.size(), 0);
    
    // Тест удаления с неверным индексом
    testing::internal::CaptureStdout();
    array.removeFigure(5);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Ошибка") != std::string::npos);
}

TEST(FigureArrayTest, TotalArea) {
    FigureArray array;
    
    auto pentagon_vertices = createRegularPentagon(Point(0, 0), 1.0);
    auto hexagon_vertices = createRegularHexagon(Point(0, 0), 1.0);
    
    array.addFigure(std::make_shared<Pentagon>(pentagon_vertices));
    array.addFigure(std::make_shared<Hexagon>(hexagon_vertices));
    
    double total = array.totalArea();
    double expected = 0.5 * 5 * 1.0 * 1.0 * sin(2 * 3.1415 / 5) +
                     0.5 * 6 * 1.0 * 1.0 * sin(2 * 3.1415 / 6);
    
    EXPECT_NEAR(total, expected, 0.01);
}


TEST(OperatorOverloadTest, OutputOperator) {
    auto vertices = createRegularPentagon(Point(0, 0), 1.0);
    Pentagon pentagon(vertices);
    
    std::stringstream ss;
    ss << pentagon;
    
    std::string output = ss.str();
    EXPECT_TRUE(output.find("Вершины") != std::string::npos);
    EXPECT_TRUE(output.find("Геом. центр") != std::string::npos);
    EXPECT_TRUE(output.find("Площадь") != std::string::npos);
}

TEST(GeometricCenterTest, CenterCalculation) {
    // Тест с квадратом (4 точки)
    std::vector<Point> square_vertices = {
        Point(0, 0), Point(2, 0), Point(2, 2), Point(0, 2)
    };
    
    // Используем базовый класс RegularPolygon для теста
    RegularPolygon square(square_vertices);
    
    Point center = square.center();
    EXPECT_NEAR(center.x, 1.0, 0.001);
    EXPECT_NEAR(center.y, 1.0, 0.001);
    
    // Проверяем площадь квадрата
    EXPECT_NEAR(square.area(), 4.0, 0.001);
}

TEST(AreaTest, IrregularPolygon) {
    // Тест площади для неправильного многоугольника
    std::vector<Point> triangle_vertices = {
        Point(0, 0), Point(4, 0), Point(0, 3)
    };
    
    RegularPolygon triangle(triangle_vertices);
    
    // Площадь треугольника: (4 * 3) / 2 = 6
    EXPECT_NEAR(triangle.area(), 6.0, 0.001);
    
    // Центр треугольника: ( (0+4+0)/3, (0+0+3)/3 ) = (1.333, 1.0)
    Point center = triangle.center();
    EXPECT_NEAR(center.x, 4.0/3.0, 0.001);
    EXPECT_NEAR(center.y, 1.0, 0.001);
}

TEST(FigureArrayTest, IndexOperator) {
    FigureArray array;
    
    auto pentagon_vertices = createRegularPentagon(Point(0, 0), 1.0);
    array.addFigure(std::make_shared<Pentagon>(pentagon_vertices));
    
    auto figure = array[0];
    EXPECT_NE(figure, nullptr);
    EXPECT_NEAR(figure->area(), 0.5 * 5 * 1.0 * 1.0 * sin(2 * 3.1415 / 5), 0.01);
    
    // Тест с неверным индексом
    auto invalid_figure = array[5];
    EXPECT_EQ(invalid_figure, nullptr);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}