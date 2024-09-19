#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

#include <iostream>
#include <functional>

namespace mephi {

template<typename T>
class Sequence {
public:
    virtual ~Sequence() {}

    virtual T getFirst() const = 0;
    virtual T getLast() const = 0;
    virtual T get(size_t index) const = 0;
    virtual size_t getLength() const = 0;
    virtual void append(const T& item) = 0;
    virtual void prepend(const T& item) = 0;
    virtual void insertAt(const T& item, size_t index) = 0;
    virtual void set(const T& item, size_t index) = 0;
    virtual void swapElements(size_t index1, size_t index2) = 0;
    virtual Sequence<T>* getSubsequence(size_t startIndex, size_t endIndex) const = 0;
    virtual Sequence<T>* concat(const Sequence<T>& other) const = 0;
    virtual Sequence<T>* map(std::function<T(T)> mapper) const = 0;
    virtual Sequence<T>* filter_out(std::function<bool(T)> filter) const = 0;
    virtual T reduce(std::function<T(T&, T)> combiner, const T& startValue) const = 0;
    virtual void print() const = 0;
};

}

#endif
