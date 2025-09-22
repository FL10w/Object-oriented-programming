#include <gtest/gtest.h>
#include "func.h"

TEST(test_01, basic_test_set)
{
    ASSERT_STREQ(Numbers("pupu123"), "123"); 
}

TEST(test_02, basic_test_set)
{
    ASSERT_STREQ(Numbers("p1u2p3u"), "123");
}

TEST(test_03, basic_test_set)
{
    ASSERT_STREQ(Numbers("hello456world"), "456");
}

TEST(test_04, basic_test_set)
{
    ASSERT_STREQ(Numbers("nothing"), "");
}

TEST(test_05, basic_test_set)
{
    ASSERT_STREQ(Numbers("456654"), "456654");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}