#include "../sequence/LinkedList.hpp"
#include <cassert>
#include <list>
#include <random>
// im not the fan of implementing basic data structures

bool testList() {
    for (size_t round = 0; round < 100; ++round) {
        mephi::LinkedList<int> llist;
        std::list<int> stdlist;
        size_t size = std::rand() % 256;
        for (size_t i = 0; i < size; ++i) {
            llist.push_back(static_cast<int>(i));
            stdlist.push_back(static_cast<int>(i));
        }


        auto llcurrent = llist.begin();
        auto stdcurrent = stdlist.begin();
        for (size_t i = 0; i < size; ++i) {
            if (*llcurrent != *stdcurrent)
                return false;
            
            ++llcurrent;
            ++stdcurrent;
        }

        size_t pop_size = std::rand() % size;

        for (size_t i = 0; i < pop_size; ++i) {
            llist.pop_back();
            stdlist.pop_back();
        }

        if (llist.size() != stdlist.size())
            return false;

        int llsum = 0;
        int stdsum = 0;
        for (auto& elem : llist)
            llsum += elem;
        for (auto& elem : stdlist)
            stdsum += elem;

        if (llsum != stdsum)
            return false;
    }

    return true;
}

int main() {
    mephi::LinkedList<int> list;

    if (!testList())
        return 1;

    return 0;
}