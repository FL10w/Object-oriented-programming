#include "Geometry.h"
#include <gtest/gtest.h>
#include <cmath>
#include <typeinfo>
using namespace std;

TEST(PointTest, DefaultConstructor) {
    Point<double> p;
    EXPECT_DOUBLE_EQ(p.x, 0.0);
    EXPECT_DOUBLE_EQ(p.y, 0.0);
}
TEST(PointTest, ParameterizedConstructor) {
    Point<int> p(5, 10);
    EXPECT_EQ(p.x, 5);
    EXPECT_EQ(p.y, 10);
}
TEST(ArrayTest, EmptyArray) {
    Array<std::shared_ptr<Figure<double>>> arr;
    EXPECT_EQ(arr.get_size(), 0);
}
TEST(ArrayTest, PushBack) {
    Array<std::shared_ptr<Figure<double>>> arr;
    auto rhombus = make_shared<Rhombus<double>>(1.0, 0.0, 0.0, 1.0, -1.0, 0.0, 0.0, -1.0);
    arr.push_back(rhombus);
    EXPECT_EQ(arr.get_size(), 1);
}
TEST(ArrayTest, ArrayAccess) {
    Array<std::shared_ptr<Figure<double>>> arr;
    auto trapezoid = make_shared<Trapezoid<double>>(0.0, 0.0, 5.0, 0.0, 4.0, 3.0, 1.0, 3.0);
    arr.push_back(trapezoid);
    
    EXPECT_NO_THROW(arr[0]);
    EXPECT_THROW(arr[1], std::out_of_range);
}
TEST(ArrayTest, RemoveAt) {
    Array<std::shared_ptr<Figure<double>>> arr;
    arr.push_back(make_shared<Rhombus<double>>(1.0, 0.0, 0.0, 1.0, -1.0, 0.0, 0.0, -1.0));
    arr.push_back(make_shared<Trapezoid<double>>(0.0, 0.0, 5.0, 0.0, 4.0, 3.0, 1.0, 3.0));
    
    EXPECT_EQ(arr.get_size(), 2);
    arr.remove_at(0);
    EXPECT_EQ(arr.get_size(), 1);
    EXPECT_THROW(arr.remove_at(5), std::out_of_range);
}
TEST(RhombusTest, AreaCalculation) {
    Rhombus<double> rhombus(1.0, 0.0, 0.0, 1.0, -1.0, 0.0, 0.0, -1.0);
    EXPECT_NEAR(rhombus.area(), 2.0, 1e-6);
}
TEST(RhombusTest, CenterCalculation) {
    Rhombus<double> rhombus(1.0, 0.0, 0.0, 1.0, -1.0, 0.0, 0.0, -1.0);
    Point<double> center = rhombus.center();
    EXPECT_NEAR(center.x, 0.0, 1e-6);
    EXPECT_NEAR(center.y, 0.0, 1e-6);
}
TEST(RhombusTest, DoubleConversion) {
    Rhombus<double> rhombus(1.0, 0.0, 0.0, 1.0, -1.0, 0.0, 0.0, -1.0);
    double area = static_cast<double>(rhombus);
    EXPECT_NEAR(area, 2.0, 1e-6);
}
TEST(TrapezoidTest, AreaCalculation) {
    Trapezoid<double> trapezoid(0.0, 0.0, 5.0, 0.0, 4.0, 3.0, 1.0, 3.0);
    EXPECT_NEAR(trapezoid.area(), 12.0, 1e-6);
}
TEST(TrapezoidTest, CenterCalculation) {
    Trapezoid<double> trapezoid(0.0, 0.0, 5.0, 0.0, 4.0, 3.0, 1.0, 3.0);
    Point<double> center = trapezoid.center();
    EXPECT_NEAR(center.x, 2.5, 1e-6);
    EXPECT_NEAR(center.y, 1.5, 1e-6);
}
TEST(PentagonTest, AreaCalculation) {
    Pentagon<double> pentagon(
        0.0, 1.0, 0.95, 0.31, 0.59, -0.81, -0.59, -0.81, -0.95, 0.31
    );
    double area = pentagon.area();
    EXPECT_NEAR(area, 2.377, 0.01);
}
TEST(PentagonTest, CenterCalculation) {
    Pentagon<double> pentagon(
        0.0, 1.0, 0.95, 0.31, 0.59, -0.81, -0.59, -0.81, -0.95, 0.31
    );
    Point<double> center = pentagon.center();
    EXPECT_NEAR(center.x, 0.0, 1e-6);
    EXPECT_NEAR(center.y, 0.0, 1e-6);
}
TEST(CloneTest, RhombusClone) {
    Rhombus<double> original(1.0, 0.0, 0.0, 1.0, -1.0, 0.0, 0.0, -1.0);
    auto clone = original.clone();
    EXPECT_NEAR(original.area(), clone->area(), 1e-6);
}
TEST(ConceptTest, ScalarTypeValidation) {
    EXPECT_TRUE(ScalarType<int>);
    EXPECT_TRUE(ScalarType<double>);
    EXPECT_TRUE(ScalarType<float>);
}
TEST(TotalAreaTest, MultipleFigures) {
    Array<std::shared_ptr<Figure<double>>> arr;
    arr.push_back(make_shared<Rhombus<double>>(1.0, 0.0, 0.0, 1.0, -1.0, 0.0, 0.0, -1.0));
    arr.push_back(make_shared<Trapezoid<double>>(0.0, 0.0, 5.0, 0.0, 4.0, 3.0, 1.0, 3.0)); 
    double total_area = 0.0;
    for (size_t i = 0; i < arr.get_size(); ++i) {
        total_area += static_cast<double>(*arr[i]);
    }
    EXPECT_NEAR(total_area, 14.0, 1e-6);
}
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}