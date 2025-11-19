#include "func.h"
#include <new>
#include <algorithm>

void* DynamicListMemoryResource::do_allocate(size_t bytes, size_t alignment) {
    (void)alignment;
    for (auto it = free_pool_.begin(); it != free_pool_.end(); ++it) {
        if (it->size >= bytes) {
            void* p = it->ptr;
            allocated_.emplace_back(p, bytes);
            free_pool_.erase(it);
            return p;
        }
    }
    void* p = ::operator new(bytes);
    allocated_.emplace_back(p, bytes);
    return p;
}
void DynamicListMemoryResource::do_deallocate(void* p, size_t bytes, size_t alignment) {
    (void)alignment;
    for (auto it = allocated_.begin(); it != allocated_.end(); ++it) {
        if (it->ptr == p && it->size == bytes) {
            free_pool_.emplace_back(p, bytes);
            allocated_.erase(it);
            return;
        }
    }
}
bool DynamicListMemoryResource::do_is_equal(const std::pmr::memory_resource& other) const noexcept {
    return this == &other;
}
DynamicListMemoryResource::~DynamicListMemoryResource() {
    for (auto& b : allocated_) ::operator delete(b.ptr);
    for (auto& b : free_pool_) ::operator delete(b.ptr);
}
template <typename T>
void DynamicArray<T>::reserve(size_t new_cap) {
    if (new_cap <= capacity_) return;
    T* new_data = alloc_.allocate(new_cap);
    for (size_t i = 0; i < size_; ++i) {
        alloc_.construct(&new_data[i], std::move(data_[i]));
        alloc_.destroy(&data_[i]);
    }
    if (data_) {
        alloc_.deallocate(data_, capacity_);
    }
    data_ = new_data;
    capacity_ = new_cap;
}
template <typename T>
void DynamicArray<T>::resize(size_t new_size) {
    if (new_size < size_) {
        for (size_t i = new_size; i < size_; ++i)
            alloc_.destroy(&data_[i]);
        size_ = new_size;
    } else if (new_size > size_) {
        reserve(std::max(capacity_ * 2, new_size));
        for (size_t i = size_; i < new_size; ++i)
            alloc_.construct(&data_[i]);
        size_ = new_size;
    }
}
template <typename T>
void DynamicArray<T>::clear() {
    for (size_t i = 0; i < size_; ++i)
        alloc_.destroy(&data_[i]);
    size_ = 0;
}
template <typename T>
void DynamicArray<T>::push_back(const T& value) {
    if (size_ == capacity_)
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    alloc_.construct(&data_[size_], value);
    ++size_;
}
template <typename T>
void DynamicArray<T>::push_back(T&& value) {
    if (size_ == capacity_)
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    alloc_.construct(&data_[size_], std::move(value));
    ++size_;
}
template class DynamicArray<int>;
template class DynamicArray<Point>;