#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP

#include <iostream>
#include <memory>
#include <initializer_list>
#include <vector>
#include <exception>
#include <stdexcept>

namespace mephi {

template<typename T>
class DynamicArray {
public:
    struct iterator {
        using value_type = T;
        using pointer =  T*;
        using reference = T&;
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
    
        iterator() : ptr_(nullptr) {}
        iterator(T* rhs) : ptr_(rhs) {}
        iterator(const iterator &rhs) : ptr_(rhs.ptr_) {}
        /* inline iterator& operator=(Type* rhs) {ptr_ = rhs; return *this;} */
        /* inline iterator& operator=(const iterator &rhs) {ptr_ = rhs.ptr_; return *this;} */
        inline iterator& operator+=(difference_type rhs) {ptr_ += rhs; return *this;}
        inline iterator& operator-=(difference_type rhs) {ptr_ -= rhs; return *this;}
        // inline T operator*() const {return *ptr_;}
        inline T& operator*() const {return *ptr_;}
        inline T* operator->() const {return ptr_;}
        inline T& operator[](difference_type rhs) const {return ptr_[rhs];}
        
        inline iterator& operator++() {++ptr_; return *this;}
        inline iterator& operator--() {--ptr_; return *this;}
        inline iterator operator++(int) {iterator tmp(*this); ++ptr_; return tmp;}
        inline iterator operator--(int) {iterator tmp(*this); --ptr_; return tmp;}
        /* inline iterator operator+(const iterator& rhs) {return iterator(ptr_+rhs.ptr);} */
        inline difference_type operator-(const iterator& rhs) const {return ptr_-rhs.ptr_;}
        inline iterator operator+(difference_type rhs) const {return iterator(ptr_+rhs);}
        inline iterator operator-(difference_type rhs) const {return iterator(ptr_-rhs);}
        friend inline iterator operator+(difference_type lhs, const iterator& rhs) {return iterator(lhs+rhs.ptr_);}
        friend inline iterator operator-(difference_type lhs, const iterator& rhs) {return iterator(lhs-rhs.ptr_);}
        
        inline bool operator==(const iterator& rhs) const {return ptr_ == rhs.ptr_;}
        inline bool operator!=(const iterator& rhs) const {return ptr_ != rhs.ptr_;}
        inline bool operator>(const iterator& rhs) const {return ptr_ > rhs.ptr_;}
        inline bool operator<(const iterator& rhs) const {return ptr_ < rhs.ptr_;}
        inline bool operator>=(const iterator& rhs) const {return ptr_ >= rhs.ptr_;}
        inline bool operator<=(const iterator& rhs) const {return ptr_ <= rhs.ptr_;}

        // friend bool operator==(const iterator& rhs, const iterator& lhs) { return rhs.index_ == lhs.index_; };
        // friend bool operator!=(const iterator& rhs, const iterator& lhs) { return rhs.index_ != lhs.index_; };  
    private:
        T* ptr_;
    };

    DynamicArray() : data_(new T[8]), size_(0), capacity_(8) {}

    DynamicArray(size_t size)
    : data_(new T[2 * size]), size_(size), capacity_(2 * size) {}
    
    DynamicArray(const DynamicArray<T>& other)
    : data_(new T[other.size_]), size_(other.size_), capacity_(other.capacity_) {}
    
    DynamicArray(DynamicArray<T>&& other)
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
    }

    DynamicArray(std::initializer_list<T> list) : DynamicArray(list.size()) {
        for (auto it = list.begin(); it != list.end(); ++it) {
            data_[it - list.begin()] = *it;
        }
    }

    DynamicArray(std::vector<T> data) : DynamicArray(data.size()) {
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = data[i];
        }
    }

    ~DynamicArray() { delete[] data_; }

    DynamicArray<T>& operator=(const DynamicArray<T>& other) {
        if (this != &other) {
            delete[] data_;
            data_ = new T(other.size_);
            size_ = other.size_;
            capacity_ = other.capacity_;
        }
        return *this;
    }

    DynamicArray<T>& operator=(DynamicArray<T>&& other) {
        if (this != &other) {
            delete data_;
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;

            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    iterator begin() { return iterator(data_); }
    iterator end() { return iterator(&data_[size_]); }
    iterator rbegin() const { return iterator(&data_[size_ - 1]); }
    iterator rend() { return iterator(&data_[-1]); }

    void push_back() {
        if (size_ == capacity_) {
            resize(capacity_ * 2);
        }
        data_[size_++] = T();
    }

    void push_back(const T& value) {
        if (size_ >= capacity_) {
            reserve(capacity_ * 2);
        }
        data_[size_++] = value;
    }

    void pop_back() {
        if (size_ <= 0) {
            throw std::out_of_range("DynamicArray::pop_back with no elements");
        }
        --size_;
    }

    void insert(size_t index, const T& value) {
        if (index > size_) {
            throw std::out_of_range("DynamicArray::insert with invalid (greater) index");
        }
        if (index < 0) {
            throw std::out_of_range("DynamicArray::insert with negative index");
        }

        if (size_ == capacity_) {
            resize(capacity_ * 2);
        }

        for (size_t i = size_; i > index; --i) {
            data_[i] = data_[i - 1];
        }

        data_[index] = value;
        ++size_;
    }

    void erase(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("DynamicArray::erase with invalid (greater) index");
        }
        if (index < 0) {
            throw std::out_of_range("DynamicArray::erase with negative index");
        }

        for (size_t i = index; i < size_ - 1; ++i) {
            data_[i] = data_[i + 1];
        }

        --size_;
    }

    void resize(size_t size) {
        if (size < size_) {
            throw std::out_of_range("DynamicArray::resize with size smaller than current size");
        }

        if (size > capacity_) {
            reserve(2 * size);
        }
        size_ = size;
    }

    void reserve(size_t capacity) {
        if (capacity < size_) {
            throw std::out_of_range("DynamicArray::reserve with smaller capacity than size");
        }
        if (capacity > capacity_) {
            T* new_data = new T[capacity];

            for (size_t i = 0; i < size_; ++i) {
                new_data[i] = data_[i];
            }
            
            delete[] data_;
            data_ = new_data;
            capacity_ = capacity;
        }
    }

    void clear() {
        size_ = 0;
    }

    T* data() const {
        return data_;
    }

    T& operator[](size_t index) {
        if (index >= size_ || index < 0) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }

    T operator[](size_t index) const {
        if (index >= size_ || index < 0) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }

    T at(size_t index) const {
        if (index >= size_ || index < 0) {
            throw std::out_of_range("Index out of range");
        }

        return data_[index];
    }

    size_t size() const {
        return size_;
    }

    bool empty() const {
        return size_ == 0;
    }

    bool operator==(const DynamicArray& other) const {
        if (size_ != other.size_) {
            return false;
        }

        for (size_t i = 0; i < size_; ++i) {
            if (data_[i] != other.data_[i]) {
                return false;
            }
        }

        return true;
    }

    bool operator==(const std::vector<T>& other) const {
        if (size_ != other.size()) {
            return false;
        }

        for (size_t i = 0; i < size_; ++i) {
            if (data_[i] != other[i]) {
                return false;
            }
        }

        return true;
    }

    bool operator!=(const DynamicArray& other) const {
        return !(*this == other);
    }

    bool operator!=(const std::vector<T>& other) const {
        return !(*this == other);
    }

    operator std::vector<T>() const {
        std::vector<T> vec;
        vec.reserve(size_);
        for (size_t i = 0; i < size_; ++i) {
            vec.push_back(data_[i]);
        }
        return vec;
    }

private:
    T* data_;
    size_t size_;
    size_t capacity_;
};

}

#endif
