#include <gtest/gtest.h>
#include "func.h"

TEST(OctalTest, DefaultConstructor) {
    Octal num;
    EXPECT_EQ(num.toString(), "0");
}
TEST(OctalTest, SizeConstructor) {
    Octal num(3, 5);
    EXPECT_EQ(num.toString(), "555");
}
TEST(OctalTest, InitializerListConstructor) {
    Octal num({1, 2, 3});
    EXPECT_EQ(num.toString(), "123");
}
TEST(OctalTest, StringConstructor) {
    Octal num("456");
    EXPECT_EQ(num.toString(), "456");
}
TEST(OctalTest, CopyConstructor) {
    Octal num1("123");
    Octal num2(num1);
    EXPECT_EQ(num2.toString(), "123");
}
TEST(OctalTest, MoveConstructor) {
    Octal num1("123");
    Octal num2(std::move(num1));
    EXPECT_EQ(num2.toString(), "123");
    EXPECT_TRUE(num1.empty() || num1.toString() == "0");
}
TEST(OctalTest, AddOperation) {
    Octal num1("123");
    Octal num2("456");
    Octal result = num1.add(num2);
    EXPECT_EQ(result.toString(), "601");
}
TEST(OctalTest, SubtractOperation) {
    Octal num1("756");
    Octal num2("123");
    Octal result = num1.subtract(num2);
    EXPECT_EQ(result.toString(), "633");
}
TEST(OctalTest, SubtractNegative) {
    Octal num1("123");
    Octal num2("456");
    EXPECT_THROW(num1.subtract(num2), std::invalid_argument);
}
TEST(OctalTest, ComparisonOperations) {
    Octal num1("123");
    Octal num2("456");
    Octal num3("123");
    
    EXPECT_TRUE(num2.greaterThan(num1));
    EXPECT_TRUE(num1.lessThan(num2));
    EXPECT_TRUE(num1.equals(num3));
    EXPECT_FALSE(num1.equals(num2));
}
TEST(OctalTest, AddAssignOperation) {
    Octal num1("123");
    Octal num2("456");
    num1.addAssign(num2);
    EXPECT_EQ(num1.toString(), "601");
}
TEST(OctalTest, SubtractAssignOperation) {
    Octal num1("756");
    Octal num2("123");
    num1.subtractAssign(num2);
    EXPECT_EQ(num1.toString(), "633");
}
TEST(OctalTest, CopyOperation) {
    Octal num1("123");
    Octal num2 = num1.copy();
    EXPECT_EQ(num2.toString(), "123");
    EXPECT_TRUE(num1.equals(num2));
}
TEST(OctalTest, InvalidDigit) {
    EXPECT_THROW(Octal num({1, 8, 3}), std::invalid_argument);
    EXPECT_THROW(Octal num("189"), std::invalid_argument);
}
TEST(OctalTest, RemoveLeadingZeros) {
    Octal num({0, 0, 1, 2, 3});
    EXPECT_EQ(num.toString(), "123");
    
    Octal num2(3, 0);
    EXPECT_EQ(num2.toString(), "0");
}
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}