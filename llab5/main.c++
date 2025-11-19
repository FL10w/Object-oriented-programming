#include "func.h"
#include <iostream>

int main() {
    std::cout << "=== Демонстрация ===\n";
    DynamicListMemoryResource mr;
    DynamicArray<int> arr(&mr);
    for (int i = 1; i <= 4; ++i) arr.push_back(i * 5);
    std::cout << "int: ";
    for (auto x : arr) std::cout << x << " ";
    std::cout << "\nsize=" << arr.size() << ", cap=" << arr.capacity() << "\n";
    DynamicArray<Point> pts(&mr);
    pts.push_back({0, 0, "O"});
    pts.push_back({1, 2, "A"});
    std::cout << "Points:\n";
    for (const auto& p : pts)
        std::cout << "  (" << p.x << ", " << p.y << ") \"" << p.name << "\"\n";
    std::cout << "Память: allocated=" << mr.allocated_count()
              << ", free=" << mr.free_pool_count() << "\n";
    return 0;
}