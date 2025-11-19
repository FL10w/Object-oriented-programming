#include "func.h"
#include <gtest/gtest.h>

TEST(MemoryResourceTest, ReuseBlocks) {
    DynamicListMemoryResource mr;
    std::pmr::polymorphic_allocator<char> alloc(&mr);

    auto* p1 = alloc.allocate(64);
    auto* p2 = alloc.allocate(32);
    EXPECT_EQ(mr.allocated_count(), 2);
    EXPECT_EQ(mr.free_pool_count(), 0);

    alloc.deallocate(p1, 64);
    EXPECT_EQ(mr.allocated_count(), 1);
    EXPECT_EQ(mr.free_pool_count(), 1);

    auto* p3 = alloc.allocate(64);
    EXPECT_EQ(p3, p1); 
    EXPECT_EQ(mr.allocated_count(), 2);
    EXPECT_EQ(mr.free_pool_count(), 0);

    alloc.deallocate(p2, 32);
    alloc.deallocate(p3, 64);
}
TEST(MemoryResourceTest, CleanupOnDestruction) {
    size_t before = 0;
    {
        DynamicListMemoryResource mr;
        std::pmr::polymorphic_allocator<int> alloc(&mr);
        auto* p = alloc.allocate(10);
        (void)p;
        before = mr.allocated_count(); // 1
    }
    SUCCEED();
}
TEST(DynamicArrayTest, PushBackInt) {
    DynamicListMemoryResource mr;
    DynamicArray<int> arr(&mr);
    arr.push_back(10);
    arr.push_back(20);
    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 20);
    EXPECT_NE(arr.data(), nullptr);
}
TEST(DynamicArrayTest, IteratorInt) {
    DynamicListMemoryResource mr;
    DynamicArray<int> arr(&mr);
    arr.push_back(1);
    arr.push_back(2);
    arr.push_back(3);

    int sum = 0;
    for (auto it = arr.begin(); it != arr.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 6);
}
TEST(DynamicArrayTest, PushBackPoint) {
    DynamicListMemoryResource mr;
    DynamicArray<Point> arr(&mr);
    arr.push_back(Point{1, 2, "A"});
    arr.push_back(Point{3, 4, "B"});
    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr[0].x, 1);
    EXPECT_EQ(arr[1].name, "B");
}
TEST(DynamicArrayTest, MoveConstructor) {
    DynamicListMemoryResource mr;
    DynamicArray<int> a(&mr);
    a.push_back(100);
    a.push_back(200);

    DynamicArray<int> b = std::move(a);
    EXPECT_EQ(b.size(), 2);
    EXPECT_EQ(b[0], 100);
    EXPECT_EQ(a.size(), 0);
    EXPECT_EQ(a.data(), nullptr);
}
TEST(DynamicArrayTest, MoveAssignment) {
    DynamicListMemoryResource mr;
    DynamicArray<int> a(&mr), b(&mr);
    a.push_back(1);
    b.push_back(10);
    b.push_back(20);

    b = std::move(a);
    EXPECT_EQ(b.size(), 1);
    EXPECT_EQ(b[0], 1);
    EXPECT_EQ(a.size(), 0);
    EXPECT_EQ(a.data(), nullptr);
}