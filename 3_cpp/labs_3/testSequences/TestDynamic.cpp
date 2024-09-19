#include "../sequence/DynamicArray.hpp"
#include <cassert>
#include <iostream>
#include <random>

// using namespace mephi;

bool testPushBack() {
    for (size_t rounds = 0; rounds < 100; rounds++) {
        size_t size = std::rand() % 256;    
        mephi::DynamicArray<int> arr;
        
        for (size_t i = 0; i < size; ++i) {
            arr.push_back(static_cast<int>(i));
        }

        if (arr.size() != size) {
            return false;
        }

        for (size_t i = 0; i < size; ++i) {
            if (arr[i] != static_cast<int>(i)) {
                return false;
            }
        }
    }
    return true;
}

bool testConvesion() {
    {
    std::vector<int> v = {1, 2, 3, 4, 5};
    mephi::DynamicArray<int> d = {1, 2, 3, 4, 5};
    if (d != v)
        return false;
    }
    
    {
    std::vector<int> v = {1, 2, 3, 4, 5};
    mephi::DynamicArray<int> d = {2, 4, 6, 8, 10};
    if (d == v)
        return false;
    }

    for (size_t rounds = 0; rounds < 100; ++rounds) {
        size_t size = std::rand() % 256;
        std::vector<int> v;
        mephi::DynamicArray<int> d;

        for (size_t i = 0; i < size; ++i) {
            v.push_back(static_cast<int>(i));
            d.push_back(static_cast<int>(i));
        }

        if (d.size() != v.size())
            return false;

        for (size_t i = 0; i < size; ++i) {
            if (d[i] != v[i])
                return false;
        }

        if (d != v)
            return false;
    }

    return true;
}

bool testResize() {
    for (size_t rounds = 0; rounds < 100; rounds++) {
        size_t size = std::rand() % 256;
        mephi::DynamicArray<int> d(size);
        std::vector<int> v(size);

        for (size_t i = 0; i < size / 2; ++i) {
            d.push_back(static_cast<int>(i));
            v.push_back(static_cast<int>(i));
        }

        size_t pop_back_size = std::rand() % size + 1;

        for (size_t i = 0; i < pop_back_size; ++i) {
            d.pop_back();
            v.pop_back();
        }
\
        if (d.size() != v.size())
            return false;

        d.resize(2 * size);
        v.resize(2 * size);

        if (d != v)
            return false;
    }
    return true;
}

/*
// to debug:
// valgrind --leak-check=full \
//          --show-leak-kinds=all \
//          --track-origins=yes \
//          --verbose \
//          --log-file=valgrind-out.txt \
//          ./executable exampleParam1
*/

int main() {
    if (!testPushBack())
        return 1;
    if (!testConvesion())
        return 1;
    
    return 0;
}