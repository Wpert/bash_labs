#ifndef BUBBLE_MERGE_HPP
#define BUBBLE_MERGE_HPP

#include "sort_utilities.hpp"
#include <functional>

namespace mephi {

template <typename RandomIt>
void bubbleSort(const RandomIt& Begin, const RandomIt& End, ComparatorT<RandomIt> comp = cmp<RandomIt>) {
    for (auto it = Begin; it != End; ++it)
        for (auto jt = Begin; jt != End - (it - Begin) - 1; ++jt)
            if (comp(*jt, *std::next(jt)))
                std::swap(*jt, *std::next(jt));
}

template <typename RandomIt>
void merge(const RandomIt& Begin, const RandomIt& Mid, const RandomIt& End, ComparatorT<RandomIt> comp = cmp<RandomIt>) {
    size_t it1 = 0;
    size_t it2 = 0;
    
    size_t Size = std::distance(Begin, End);
    std::vector< typename RandomIt::value_type > result(Size);
    // sure i can write just a C array :)
  
    while ((Begin + it1 < Mid) && (Mid + it2 < End)) {
        if (comp( *(Mid + it2), *(Begin + it1) )) {
            result[it1 + it2] = *(Begin + it1);
            it1 += 1;
        }
        else {
            result[it1 + it2] = *(Mid + it2);
            it2 += 1;
        }
    }
  
    while (Begin + it1 < Mid) {
        result[it1 + it2] = *(Begin + it1);
        it1 += 1;
    }
    while (Mid + it2 < End) {
        result[it1 + it2] = *(Mid + it2);
        it2 += 1;
    }
    for (size_t i = 0; i <  it1 + it2; ++i)
        *(Begin + i) = result[i];
}

template <typename RandomIt>
void mergeSort(const RandomIt& Begin, const RandomIt& End, ComparatorT<RandomIt> comp = cmp<RandomIt>) {
    size_t Size = std::distance(Begin, End);
    if (Size == 1)
        return;

    auto Mid = std::next(Begin, Size / 2);
    mergeSort(Begin, Mid);
    mergeSort(Mid, End);
    merge(Begin, Mid, End, comp);
}

}

#endif
