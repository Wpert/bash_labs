#include <iostream>
#include "sequence/DynamicArray.hpp"
#include "sequence/LinkedList.hpp"
#include "sorting/bubble_merge.hpp"
#include "sorting/quick_shell.hpp"

int main() {
    mephi::DynamicArray<int> arr = {12, 10, 99, 7, 239, 246, 1, 3, 5};
    // std::vector<int> arr = {12, 10, 99, 7, 239, 246, 1, 3, 5};
    arr.push_back(1);
    mephi::shellSort(arr.begin(), arr.end());

    std::cout << arr.size() << std::endl;
    
    for (auto& elem : arr) {
        std::cout << elem << ' ';
    }

    std::cout << std::endl;

    mephi::LinkedList<int> list;
    list.push_back(7);
    list.push_back(9);
    list.push_back(5);
    std::cout << list.size() << std::endl;

    return 0;
}