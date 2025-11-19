#pragma once
#include <cstddef>
#include <list>
#include <memory_resource>
#include <string>

class DynamicListMemoryResource : public std::pmr::memory_resource {
private:
    struct Block {
        void* ptr;
        size_t size;
        Block(void* p, size_t s) : ptr(p), size(s) {}
    };

    std::list<Block> allocated_;
    std::list<Block> free_pool_;

protected:
    void* do_allocate(size_t bytes, size_t alignment) override;
    void do_deallocate(void* p, size_t bytes, size_t alignment) override;
    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override;

public:
    ~DynamicListMemoryResource() override;

    size_t allocated_count() const { return allocated_.size(); }
    size_t free_pool_count() const { return free_pool_.size(); }
};

template <typename T>
class DynamicArrayIterator {
    T* ptr_;
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type        = T;
    using difference_type   = std::ptrdiff_t;
    using pointer           = T*;
    using reference         = T&;

    explicit DynamicArrayIterator(T* p = nullptr) : ptr_(p) {}

    reference operator*() const { return *ptr_; }
    pointer operator->() const { return ptr_; }

    DynamicArrayIterator& operator++() { ++ptr_; return *this; }
    DynamicArrayIterator operator++(int) { auto tmp = *this; ++(*this); return tmp; }

    friend bool operator==(const DynamicArrayIterator& a, const DynamicArrayIterator& b) {
        return a.ptr_ == b.ptr_;
    }
    friend bool operator!=(const DynamicArrayIterator& a, const DynamicArrayIterator& b) {
        return !(a == b);
    }
};

template <typename T>
class DynamicArray {
    using Allocator = std::pmr::polymorphic_allocator<T>;

    T* data_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;
    Allocator alloc_;

public:
    using value_type = T;
    using iterator = DynamicArrayIterator<T>;
    using const_iterator = DynamicArrayIterator<const T>;

    explicit DynamicArray(std::pmr::memory_resource* mr = std::pmr::get_default_resource())
        : alloc_(mr) {}

    DynamicArray(const DynamicArray& other) = delete;
    DynamicArray& operator=(const DynamicArray& other) = delete;

    DynamicArray(DynamicArray&& other) noexcept
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_),
          alloc_(other.alloc_.resource())
    {
        other.data_ = nullptr;
        other.size_ = other.capacity_ = 0;
    }

    DynamicArray& operator=(DynamicArray&& other) noexcept {
        if (this != &other) {
            clear();
            if (data_) {
                alloc_.deallocate(data_, capacity_);
            }
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.data_ = nullptr;
            other.size_ = other.capacity_ = 0;
        }
        return *this;
    }

    ~DynamicArray() {
        clear();
        if (data_) {
            alloc_.deallocate(data_, capacity_);
        }
    }

    void push_back(const T& value);
    void push_back(T&& value);
    void reserve(size_t new_cap);
    void resize(size_t new_size);
    void clear();

    size_t size() const { return size_; }
    size_t capacity() const { return capacity_; }
    bool empty() const { return size_ == 0; }

    T& operator[](size_t i) { return data_[i]; }
    const T& operator[](size_t i) const { return data_[i]; }

    T* data() noexcept { return data_; }
    const T* data() const noexcept { return data_; }

    iterator begin() { return iterator(data_); }
    iterator end() { return iterator(data_ + size_); }
    const_iterator begin() const { return const_iterator(data_); }
    const_iterator end() const { return const_iterator(data_ + size_); }
};

struct Point {
    int x = 0, y = 0;
    std::string name;
    Point() = default;
    Point(int x_, int y_, std::string n) : x(x_), y(y_), name(std::move(n)) {}
};