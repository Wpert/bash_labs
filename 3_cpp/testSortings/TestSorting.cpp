#include "../sorting/bubble_merge.hpp"
#include "../sorting/quick_shell.hpp"
#include <algorithm>
#include <random>

using namespace mephi;

bool testSorting() {
    for (size_t round = 0; round < 100; ++round) {
        std::vector<int> v;
        size_t size = std::rand() % 2048 + 1;
        v.reserve(size);
        
        for (size_t i = 0; i < size; ++i) {
            v.push_back(std::rand() % 2048);
        }

        if (round % 4 == 0)
            bubbleSort(v.begin(), v.end());
        else if (round % 4 == 1)
            mergeSort(v.begin(), v.end());
        else if (round % 4 == 2)
            quickSort(v.begin(), v.end());
        else if (round % 4 == 3)
            shellSort(v.begin(), v.end());
        
        bool sortedFlag = std::is_sorted(v.begin(), v.end());
        
        if (!sortedFlag) {
            return false;
        }
    }
    return true;
}

int main() {
    if (!testSorting())
        return 1;

    return 0;
}