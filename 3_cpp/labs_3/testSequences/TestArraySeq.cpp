#include "../sequence/ArraySequence.hpp"
#include <iostream>
#include <cassert>

int incr(int a) {
    return a + 1;
}

bool testMap() {
    for (size_t rounds = 0; rounds < 100; ++rounds) {
        mephi::ArraySequence<int> seq;
        for (size_t i = 0; i < 100; ++i) {
            seq.append(i);
        }
        
        auto* new_seq = seq.map(incr);

        for (size_t i = 0; i < 100; ++i) {
            if (new_seq->get(i) != static_cast<int>(i + 1))
                return false;
        }

        delete new_seq;
    }
    return true;
}

bool mod2(int a) {
    return a % 2 == 0;
}

bool testFilter() {
    for (size_t rounds = 0; rounds < 100; ++rounds) {
        mephi::ArraySequence<int> seq;
        for (size_t i = 0; i < 100; ++i) {
            seq.append(i);
        }
        auto* new_seq = seq.filter_out(mod2);
        
        for (size_t i = 0; i < new_seq->getLength(); ++i) {
            if (new_seq->get(i) != static_cast<int>(2 * i))
                return false;
        }

        delete new_seq;
    }
    return true;
}

int sum_int(int a, int b) {
    return a + b;
}

bool testReduce() {
    for (size_t rounds = 0; rounds < 100; ++rounds) {
        size_t size = std::rand() % 1000;
        mephi::ArraySequence<int> seq;

        for (size_t i = 0; i < size; ++i) {
            seq.append(std::rand() % 1000);
        }

        int sum = 0;
        for (size_t i = 0; i < size; ++i) {
            sum += seq.get(i);
        }

        int reduce_sum = seq.reduce(sum_int, 0);
        
        if (sum != reduce_sum)
            return false;

        int reduce_sum_and_1 = seq.reduce(sum_int, 1);

        if (sum + 1 != reduce_sum_and_1)
            return false;
    }
    return true;
}

int main() {
    if (!testMap()) 
        return 1;
    if (!testFilter())
        return 1;
    if (!testReduce())
        return 1;

    return 0;
}