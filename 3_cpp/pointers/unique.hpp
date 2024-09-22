#ifndef UNIQUE_HPP
#define UNIQUE_HPP

#include <utility>

namespace mephi {

template <typename T>
class UniquePtr {
public:
    explicit UniquePtr(T* ptr = nullptr) : ptr_(ptr) {}
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr(UniquePtr&& other) noexcept : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }
    ~UniquePtr() {
        delete ptr_;
    }

    UniquePtr& operator=(const UniquePtr&) = delete;
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            delete ptr_;
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }
        return *this;
    }

    void reset(T* new_ptr = nullptr) noexcept{
        T* old = std::exchange(ptr_, new_ptr);
        delete old;
    }

    T* release() noexcept{
        return std::exchange(ptr_, nullptr);
    }

    T* get() const noexcept{ return ptr_; }

    T& operator*() const noexcept{ return *ptr_; }

    T* operator->() const noexcept{ return ptr_; }

    explicit operator bool() const noexcept { return ptr_ != nullptr; }

    void swap(UniquePtr<T>& other) noexcept{
        auto curr_ptr = this->release();
        auto other_ptr = other.release();
        this->reset(other_ptr);
        other.reset(curr_ptr);
    }

private:
    T* ptr_;
};

}

#endif
