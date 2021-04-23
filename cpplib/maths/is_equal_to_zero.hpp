#pragma once
#include <cstdlib>
#include <type_traits>

#include "base/constants.hpp"

// todo[c++17] use is_floating_point_v
template<typename T, typename std::enable_if_t<std::is_floating_point<T>::value>* = nullptr>
inline bool is_equal_to_zero(const T& a) {
    return abs(a) < EPS;
}

template<typename T, typename std::enable_if_t<!std::is_floating_point<T>::value>* = nullptr>
inline bool is_equal_to_zero(const T& a) {
    return a == 0;
}
