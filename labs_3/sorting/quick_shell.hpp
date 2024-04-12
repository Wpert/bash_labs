#ifndef QUICK_SHELL_HPP
#define QUICK_SHELL_HPP

#include "sort_utilities.hpp"

namespace mephi {

template <class RandIt>
inline void quickSort(RandIt Begin, RandIt End, ComparatorT<RandIt> comp = cmp<RandIt>) {
    if (End <= Begin) return;
    
    RandIt pivot = Begin;
    RandIt middle = Begin + 1;
    for (RandIt i = Begin + 1; i < End; ++i) {
        if (comp(*pivot, *i)) {
            std::iter_swap(i, middle);
            ++middle;
        }
    }
    std::iter_swap(Begin, middle - 1);
    quickSort(Begin, middle - 1);
    quickSort(middle, End);
}

template <class RandIt>
void shellSort(RandIt Begin, RandIt End, ComparatorT<RandIt> comp = cmp<RandIt>) {
    if (End <= Begin) return;
    ptrdiff_t size = static_cast<ptrdiff_t>(End - Begin);
    for (ptrdiff_t gap = size / 2; gap > 0; gap /= 2) {
        for (RandIt i = Begin + gap; i != End; ++i){
            T<RandIt> key = *i;
            RandIt j = i;

            while (j >= Begin + gap && !comp(key, *(j - gap))) {
                *j = *(j - gap);
                j -= gap;
            }

            *j = key;
        }
    }
}

}

#endif
