#ifndef ARRAYSEQUENCE_HPP
#define ARRAYSEQUENCE_HPP

#include "Sequence.hpp"
#include "DynamicArray.hpp"

namespace mephi {

template<typename T>
class ArraySequence : public virtual Sequence<T> {
public:
    ArraySequence() = default;
    ArraySequence(const ArraySequence<T>&) = default;
    ArraySequence(ArraySequence<T>&&) = default;
    ArraySequence(const DynamicArray<T>& other) : data_(other) {}
    ArraySequence(DynamicArray<T>&& other) : data_(std::move(other)) {}
    ArraySequence(ArraySequence* other) = delete;
    ~ArraySequence() = default;

    ArraySequence<T>& operator=(const ArraySequence<T>&) = default;
    ArraySequence<T>& operator=(ArraySequence<T>&&) = default;
    ArraySequence<T>& operator=(const DynamicArray<T>& other) {
        data_ = other;
        return *this;
    }
    ArraySequence<T>& operator=(DynamicArray<T>&& other) {
        data_ = std::move(other);
        return *this;
    }

    bool operator==(const ArraySequence<T>&) const = default;

    void set(const T& item, size_t index) override { data_[index] = item; }
    T get(size_t index) const override { return data_[index]; }
    T getFirst() const override { return get(0); }
    T getLast() const override { return get(getLength() - 1); }
    size_t getLength() const override { return data_.size(); }
    void insertAt(const T& item, size_t index) override { data_.insert(index, item); }
    void append(const T& item) override { data_.push_back(item); }
    void prepend(const T& item) override { data_.insert(0, item); }
    
    void swapElements(size_t index1, size_t index2) override { std::swap(data_[index1], data_[index2]); }
    
    Sequence<T>* getSubsequence(size_t startIndex, size_t endIndex) const override {
        ArraySequence<T>* result = new ArraySequence<T>;
        result->data_.resize(endIndex - startIndex);
        for (size_t i = startIndex; i < endIndex; i++) {
            result->data_[i - startIndex] = data_[i];
        }

        return dynamic_cast<Sequence<T>*>(result);
    }

    Sequence<T>* concat(const Sequence<T>& other) const override {
        ArraySequence<T>* result = new ArraySequence<T>;
        result->data_.resize(data_.size() + other.getLength());
        
        for (size_t i = 0; i < data_.size(); ++i) {
            result->set(data_[i], i);
        }
        for (size_t i = 0; i < other.getLength(); ++i) {
            result->set(other.get(i), data_.size() + i);
        }
        return dynamic_cast<Sequence<T>*>(result);
    }

    Sequence<T>* map(std::function<T(T)> mapper) const override {
        ArraySequence<T>* result = new ArraySequence<T>;
        size_t size = getLength();
        result->data_.reserve(2 * size);

        for (size_t i = 0; i < size; ++i) {
            result->append(mapper(get(i)));
        }
        return dynamic_cast<Sequence<T>*>(result);
    
    }
    Sequence<T>* filter_out(std::function<bool(T)> filter) const override {
        ArraySequence<T>* result = new ArraySequence<T>;
        size_t size = getLength();
        result->data_.reserve(2 * size);

        for (size_t i = 0; i < size; i++) {
            if (filter(get(i))) {
                result->append(get(i));
            }
        }
        return dynamic_cast<Sequence<T>*>(result);
    
    }
    
    T reduce(std::function<T(T&, T)> combiner, const T& startValue) const override {
        T result = startValue;
        for (size_t i = 0; i < getLength(); ++i) {
            result = combiner(result, get(i));
        }

        return result;
    }

    void print() const override {
        for (size_t i = 0; i < getLength(); ++i) {
            std::cout << get(i) << " ";
        }
        std::cout << std::endl;
    }
    
private:
    DynamicArray<T> data_;
};


}


#endif
