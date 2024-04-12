#ifndef SORT_UTILITIES_HPP
#define SORT_UTILITIES_HPP

#include <vector>
#include <functional>

namespace mephi {

template <typename RandomIt>
using T = typename RandomIt::value_type;

template <typename RandomIt>
const inline bool cmp(T<RandomIt>& a, T<RandomIt>& b) {
    return (a > b);
}

template <typename RandomIt>
using ComparatorT = typename std::function<bool(T<RandomIt>&,T<RandomIt>&)>;

}

#endif
