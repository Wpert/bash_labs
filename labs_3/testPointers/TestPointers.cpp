#include <iostream>
#include <cassert>
#include "../pointers/unique.hpp"
#include "../pointers/shared.hpp"

using namespace mephi;

void testUnique_ptr() {
    std::cout << "Checking TUnique_ptr..." << std::endl;

    UniquePtr<int> ptr1;
    assert(ptr1.get() == nullptr);

    int* rawPtr = new int(101);
    UniquePtr<int> ptr2(rawPtr);
    assert(ptr2.get() == rawPtr);

    UniquePtr<int> ptr3(std::move(ptr2));
    assert(ptr2.get() == nullptr);
    assert(ptr3.get() == rawPtr);

    UniquePtr<int> ptr4;
    ptr4 = std::move(ptr3);
    assert(ptr3.get() == nullptr);
    assert(ptr4.get() == rawPtr);

    ptr4.reset();
    assert(ptr4.get() == nullptr);

    UniquePtr<int> ptr5(new int(101));
    assert(*ptr5 == 101);
    assert(*ptr5.get() == 101);

    UniquePtr<int> ptr6(new int(10));
    UniquePtr<int> ptr7(new int(20));
    ptr6.swap(ptr7);
    assert(ptr6.get() != nullptr);
    assert(*ptr6 == 20);
    assert(ptr7.get() != nullptr);
    assert(*ptr7 == 10);
    std::cout << "All tests were passed!" << std::endl;

}

void testShared_ptr() {
    std::cout << "Checking SharedPtr..." << std::endl;
    SharedPtr<int> ptr1;
    assert(ptr1.get() == nullptr);
    assert(ptr1.use_count() == 0);

    int* rawPtr = new int(101);
    SharedPtr<int> ptr2(rawPtr);
    assert(ptr2.get() == rawPtr);
    assert(ptr2.use_count() == 1);
    assert(ptr2.unique());

    SharedPtr<int> ptr3(ptr2);
    assert(ptr3.get() == rawPtr);
    assert(ptr3.use_count() == 2);
    assert(!ptr3.unique());

    SharedPtr<int> ptr4;
    ptr4 = ptr3;
    assert(ptr4.get() == rawPtr);
    assert(ptr4.use_count() == 3);

    assert(*ptr4 == 101);
    assert(*ptr4.get() == 101);

    ptr4.reset();
    assert(ptr4.get() == nullptr);
    assert(ptr4.use_count() == 0);

    SharedPtr<int> ptr5(new int(10));
    SharedPtr<int> ptr6(new int(20));
    ptr5.swap(ptr6);
    assert(ptr5.get() != nullptr);
    assert(*ptr5 == 20);
    assert(ptr6.get() != nullptr);
    assert(*ptr6 == 10);
    std::cout << "All tests were passed!" << std::endl;

}

int main() {
    testUnique_ptr();
    testShared_ptr();
    return 0;
}
